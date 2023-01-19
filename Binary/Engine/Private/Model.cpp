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

	//WriteFile(hFile, &m_eModelType, sizeof(m_eModelType), &dwByte, nullptr);
	WriteFile(hFile, &m_PivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

	_uint m_iNumMeshes = m_pAIScene->mNumMeshes;

	WriteFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		WriteFile(hFile, &m_pAIScene->mMeshes[i]->mName.data, sizeof(char) * MAX_PATH, &dwByte, nullptr);
		WriteFile(hFile, &m_pAIScene->mMeshes[i]->mMaterialIndex, sizeof(_uint), &dwByte, nullptr);

		_uint iNumVertices = m_pAIScene->mMeshes[i]->mNumVertices;
		WriteFile(hFile, &iNumVertices, sizeof(_uint), &dwByte, nullptr);

		for (_uint j = 0; j < iNumVertices; ++j)
		{
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3), &dwByte, nullptr);
		}

		if (TYPE_ANIM == m_eModelType)
		{
			_uint iNumBones = m_pAIScene->mMeshes[i]->mNumBones;
			WriteFile(hFile, &iNumBones, sizeof(_uint), &dwByte, nullptr);

			for (_uint j = 0; j < iNumBones; ++j)
			{
				_uint iNumWeights = m_pAIScene->mMeshes[i]->mBones[j]->mNumWeights;
				WriteFile(hFile, &iNumWeights, sizeof(_uint), &dwByte, nullptr);

				for (_uint k = 0; k < iNumWeights; k++)
				{
					WriteFile(hFile, &m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mVertexId, sizeof(_uint), &dwByte, nullptr);
					WriteFile(hFile, &m_pAIScene->mMeshes[i]->mBones[j]->mWeights[k].mWeight, sizeof(_float), &dwByte, nullptr);
				}
			}
		}

		_uint iNumPrimitive = m_pAIScene->mMeshes[i]->mNumFaces;
		WriteFile(hFile, &iNumPrimitive, sizeof(_uint), &dwByte, nullptr);

		for (_uint j = 0; j < iNumPrimitive; ++j)
		{
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mFaces[j].mIndices[0], sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mFaces[j].mIndices[1], sizeof(_uint), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mMeshes[i]->mFaces[j].mIndices[2], sizeof(_uint), &dwByte, nullptr);
		}
	}

	m_iNumMaterials = m_pAIScene->mNumMaterials;
	WriteFile(hFile, &m_iNumMaterials, sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
			{
				WriteFile(hFile, "", sizeof(char) * MAX_PATH, &dwByte, nullptr);
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

			WriteFile(hFile, szFullPath, sizeof(char) * MAX_PATH, &dwByte, nullptr);
		}
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

		Save_Hierarchy(hFile, &dwByte, m_pAIScene->mRootNode);

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
			WriteFile(hFile, m_pAIScene->mAnimations[i]->mName.data, sizeof(char) * MAX_PATH, &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mAnimations[i]->mDuration, sizeof(double), &dwByte, nullptr);
			WriteFile(hFile, &m_pAIScene->mAnimations[i]->mTicksPerSecond, sizeof(double), &dwByte, nullptr);
			
			_uint iNumChannels = m_pAIScene->mAnimations[i]->mNumChannels;
			WriteFile(hFile, &iNumChannels, sizeof(_uint), &dwByte, nullptr);

			for (_uint j = 0; j < iNumChannels; ++j)
			{
				WriteFile(hFile, m_pAIScene->mAnimations[i]->mChannels[j]->mNodeName.data, sizeof(char) * MAX_PATH, &dwByte, nullptr);
				
				_uint iNumScalingKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumScalingKeys;
				_uint iNumRotationKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumRotationKeys;
				_uint iNumPositionKeys = m_pAIScene->mAnimations[i]->mChannels[j]->mNumPositionKeys;

				_uint iNumKeyFrames = max(iNumScalingKeys, iNumRotationKeys);
				iNumKeyFrames = max(iNumKeyFrames, iNumPositionKeys);

				WriteFile(hFile, &iNumScalingKeys, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &iNumRotationKeys, sizeof(_uint), &dwByte, nullptr);
				WriteFile(hFile, &iNumPositionKeys, sizeof(_uint), &dwByte, nullptr);

				WriteFile(hFile, &iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

				for (_uint k = 0; k < iNumKeyFrames; ++k)
				{
					if (k < iNumScalingKeys)
					{
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue, sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime, sizeof(double), &dwByte, nullptr);
					}
					if (k < iNumRotationKeys)
					{
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x, sizeof(_float), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y, sizeof(_float), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z, sizeof(_float), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w, sizeof(_float), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime, sizeof(double), &dwByte, nullptr);
					}
					if (k < iNumPositionKeys)
					{
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue, sizeof(_float3), &dwByte, nullptr);
						WriteFile(hFile, &m_pAIScene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime, sizeof(double), &dwByte, nullptr);
					}
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

void CModel::Save_Hierarchy(HANDLE hFile, _ulong * pdwByte, const aiNode* pNode)
{
	if (nullptr == hFile)
	{
		ERR_MSG(TEXT("Failed to Save Hierarchy"));
		return;
	}

	WriteFile(hFile, pNode->mName.data, sizeof(char) * MAX_PATH, pdwByte, nullptr);
	WriteFile(hFile, &pNode->mTransformation, sizeof(_float4x4), pdwByte, nullptr);

	_uint iNumChildren = pNode->mNumChildren;
	WriteFile(hFile, &iNumChildren, sizeof(_uint), pdwByte, nullptr);

	for (_uint i = 0; i < iNumChildren; ++i)
		Save_Hierarchy(hFile, pdwByte, pNode->mChildren[i]);
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
