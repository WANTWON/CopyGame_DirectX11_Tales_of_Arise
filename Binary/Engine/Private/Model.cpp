#include "..\Public\Model.h"

#include "Texture.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumBones(rhs.m_iNumBones)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eModelType(rhs.m_eModelType)
{
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const char * pModelFilePath, const char* pMaterialPath, _fmatrix PivotMatrix)
{
	m_eModelType = eModelType;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	_uint iFlag = 0;

	if (TYPE_NONANIM == eModelType)
		iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	else
		iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	char szName[MAX_PATH] = "";
	_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, nullptr, 0);

	char ModelFilePath[MAX_PATH] = "../Bin/Data/Binary/";
	char szExt[MAX_PATH] = ".dat";
	strcat_s(ModelFilePath, szName);
	strcat_s(ModelFilePath, szExt);

	_tchar szModelFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, ModelFilePath, strlen(ModelFilePath), szModelFilePath, MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szModelFilePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		_tchar szFailText[MAX_PATH] = TEXT("Failed to Save : ");
		wcscat_s(szFailText, szModelFilePath);
		ERR_MSG(szFailText);
		return E_FAIL;
	}

	WriteFile(hFile, &m_PivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

	_uint m_iNumMeshes = m_pAIScene->mNumMeshes;

	WriteFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		WriteFile(hFile, &m_pAIScene->mMeshes[i]->mName.data, sizeof(char) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &m_pAIScene->mMeshes[i]->mMaterialIndex, sizeof(_uint), &dwByte, nullptr);

		_uint iNumVertices = m_pAIScene->mMeshes[i]->mNumVertices;
		WriteFile(hFile, &iNumVertices, sizeof(_uint), &dwByte, nullptr);

		if (TYPE_NONANIM == m_eModelType)
		{
			for (_uint j = 0; j < iNumVertices; ++j)
			{
				VTXMODEL pVertices;

				memcpy(&pVertices.vPosition, &m_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
				memcpy(&pVertices.vNormal, &m_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));
				memcpy(&pVertices.vTexUV, &m_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
				memcpy(&pVertices.vTangent, &m_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));

				WriteFile(hFile, &pVertices, sizeof(VTXMODEL), &dwByte, nullptr);
			}
		}
		else if (TYPE_ANIM == m_eModelType)
		{
			VTXANIMMODEL* pVertices = new VTXANIMMODEL[iNumVertices];
			ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iNumVertices);

			for (_uint j = 0; j < iNumVertices; ++j)
			{
				memcpy(&pVertices[j].vPosition, &m_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
				memcpy(&pVertices[j].vNormal, &m_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));
				memcpy(&pVertices[j].vTexUV, &m_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
				memcpy(&pVertices[j].vTangent, &m_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));
			} 

			_uint iNumBones = m_pAIScene->mMeshes[i]->mNumBones;

			for (_uint j = 0; j < iNumBones; ++j)
			{
				_uint iNumWeights = m_pAIScene->mMeshes[i]->mBones[j]->mNumWeights;

				for (_uint k = 0; k < iNumWeights; k++)
				{
					_uint iVertexIndex = m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId;

					if (0 == pVertices[iVertexIndex].vBlendWeight.x)
					{
						pVertices[iVertexIndex].vBlendWeight.x = m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
						pVertices[iVertexIndex].vBlendIndex.x = j;
					}
					else if (0 == pVertices[iVertexIndex].vBlendWeight.y)
					{
						pVertices[iVertexIndex].vBlendWeight.y = m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
						pVertices[iVertexIndex].vBlendIndex.y = j;
					}
					else if (0 == pVertices[iVertexIndex].vBlendWeight.z)
					{
						pVertices[iVertexIndex].vBlendWeight.z = m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
						pVertices[iVertexIndex].vBlendIndex.z = j;
					}
					else if (0 == pVertices[iVertexIndex].vBlendWeight.w)
					{
						pVertices[iVertexIndex].vBlendWeight.w = m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight;
						pVertices[iVertexIndex].vBlendIndex.w = j;
					}
				}
			}

			for (_uint j = 0; j < iNumVertices; ++j)
				WriteFile(hFile, &pVertices[j], sizeof(VTXANIMMODEL), &dwByte, nullptr);
			
			WriteFile(hFile, &iNumBones, sizeof(_uint), &dwByte, nullptr);

			Safe_Delete_Array(pVertices);
		}

		_uint iNumPrimitive = m_pAIScene->mMeshes[i]->mNumFaces;
		WriteFile(hFile, &iNumPrimitive, sizeof(_uint), &dwByte, nullptr);

		for (_uint j = 0; j < iNumPrimitive; ++j)
		{
			FACEINDICES32 pIndices;

			pIndices._0 = m_pAIScene->mMeshes[i]->mFaces[j].mIndices[0];
			pIndices._1 = m_pAIScene->mMeshes[i]->mFaces[j].mIndices[1];
			pIndices._2 = m_pAIScene->mMeshes[i]->mFaces[j].mIndices[2];

			WriteFile(hFile, &pIndices, sizeof(FACEINDICES32), &dwByte, nullptr);
		}
	}

	m_iNumMaterials = m_pAIScene->mNumMaterials;
	WriteFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		char szTextureFilePath[AI_TEXTURE_TYPE_MAX][MAX_PATH];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
			{
				strcpy_s(szTextureFilePath[j], "");
				continue;
			}

			char szName[MAX_PATH] = "";
			char szExt[MAX_PATH] = "";
			char szTextureFileName[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTextureFileName, szName);
			strcat_s(szTextureFileName, szExt);

			char szDirectory[MAX_PATH] = "";
			strcpy_s(szDirectory, pMaterialPath);
			char szFullPath[MAX_PATH] = "";

			strcpy_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szTextureFileName);

			strcpy_s(szTextureFilePath[j], szFullPath);
		}

		WriteFile(hFile, szTextureFilePath, sizeof(char) * MAX_PATH * AI_TEXTURE_TYPE_MAX, &dwByte, nullptr);
	}

	CloseHandle(hFile);

	if (TYPE_ANIM == m_eModelType)
	{
		char szName[MAX_PATH] = "";
		_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, szName, MAX_PATH, nullptr, 0);

		char ModelFilePath[MAX_PATH] = "../Bin/Data/Binary/";
		char AnimModelFileName[MAX_PATH] = "_Animation";
		char szExt[MAX_PATH] = ".dat";
		strcat_s(ModelFilePath, szName);
		strcat_s(ModelFilePath, AnimModelFileName);
		strcat_s(ModelFilePath, szExt);

		_tchar szModelFilePath[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, ModelFilePath, strlen(ModelFilePath), szModelFilePath, MAX_PATH);

		dwByte = 0;
		hFile = CreateFile(szModelFilePath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
		{
			_tchar szFailText[MAX_PATH] = TEXT("Failed to Save : ");
			wcscat_s(szFailText, szModelFilePath);
			ERR_MSG(szFailText);
			return E_FAIL;
		}

		vector<BINBONE> Bones;
		Save_Hierarchy(&Bones, m_pAIScene->mRootNode);

		for (_int i = 0; i < Bones.size(); ++i)
			WriteFile(hFile, &Bones[i], sizeof(BINBONE), &dwByte, nullptr);

		for (_uint i = 0; i < m_iNumMeshes; ++i)
		{
			for (_uint j = 0; j < m_pAIScene->mMeshes[i]->mNumBones; ++j)
			{
				WriteFile(hFile, m_pAIScene->mMeshes[i]->mBones[j]->mName.data, sizeof(char) * MAX_PATH, &dwByte, nullptr);
				WriteFile(hFile, &m_pAIScene->mMeshes[i]->mBones[j]->mOffsetMatrix, sizeof(_float4x4), &dwByte, nullptr);
			}
		}

		_uint iNumAnimations = m_pAIScene->mNumAnimations;
		WriteFile(hFile, &iNumAnimations, sizeof(_uint), &dwByte, nullptr);

		for (_uint i = 0; i < iNumAnimations; ++i)
		{
			BINANIM BinAnim; 

			memcpy(BinAnim.szName, m_pAIScene->mAnimations[i]->mName.data, sizeof(char) * MAX_PATH);
			BinAnim.dbDuration = m_pAIScene->mAnimations[i]->mDuration;
			BinAnim.dbTickPerSecond = m_pAIScene->mAnimations[i]->mTicksPerSecond;
			
			_uint iNumChannels = m_pAIScene->mAnimations[i]->mNumChannels;
			BinAnim.iNumChannels = iNumChannels;

			WriteFile(hFile, &BinAnim, sizeof(BINANIM), &dwByte, nullptr);

			for (_uint j = 0; j < iNumChannels; ++j)
			{
				BINCHANNEL BinChannel;
				memcpy(BinChannel.szName, m_pAIScene->mAnimations[i]->mChannels[j]->mNodeName.data, sizeof(char) * MAX_PATH);
				
				_uint iNumScalingKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
				_uint iNumRotationKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
				_uint iNumPositionKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;

				_uint iNumKeyFrames = max(iNumScalingKeys, iNumRotationKeys);
				iNumKeyFrames = max(iNumKeyFrames, iNumPositionKeys);

				BinChannel.iNumKeyFrames = iNumKeyFrames;

				WriteFile(hFile, &BinChannel, sizeof(BINCHANNEL), &dwByte, nullptr);

				for (_uint k = 0; k < iNumKeyFrames; ++k)
				{
					KEYFRAME KeyFrame;
					ZeroMemory(&KeyFrame, sizeof(KEYFRAME));

					if (k < iNumScalingKeys)
					{
						memcpy(&KeyFrame.vScale, &m_pAIScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue, sizeof(_float3));
						KeyFrame.fTime = m_pAIScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
					}
					if (k < iNumRotationKeys)
					{
						memcpy(&KeyFrame.vRotation.x, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x, sizeof(_float));
						memcpy(&KeyFrame.vRotation.y, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y, sizeof(_float));
						memcpy(&KeyFrame.vRotation.z, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z, sizeof(_float));
						memcpy(&KeyFrame.vRotation.w, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w, sizeof(_float));
						KeyFrame.fTime = m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
					}
					if (k < iNumPositionKeys)
					{
						memcpy(&KeyFrame.vPosition, &m_pAIScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue, sizeof(_float3));
						KeyFrame.fTime = m_pAIScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
					}

					WriteFile(hFile, &KeyFrame, sizeof(KEYFRAME), &dwByte, nullptr);
				}
			}
		}

		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	return S_OK;
}

void CModel::Save_Hierarchy(vector<BINBONE>* pBones, const aiNode* pNode)
{
	BINBONE BinBone;
	
	memcpy(&BinBone.szName, pNode->mName.data, sizeof(char) * MAX_PATH);
	memcpy(&BinBone.Transformation, &pNode->mTransformation, sizeof(_float4x4));

	_uint iNumChildren = pNode->mNumChildren;
	memcpy(&BinBone.iNumChildren, &iNumChildren, sizeof(_uint));

	pBones->push_back(BinBone);

	for (_uint i = 0; i < iNumChildren; ++i)
		Save_Hierarchy(pBones, pNode->mChildren[i]);
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eModelType, const char* pModelFilePath, const char* pMaterialPath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, pMaterialPath, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free(void)
{
	__super::Free();

	m_Importer.FreeScene();
}
