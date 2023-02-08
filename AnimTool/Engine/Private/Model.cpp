#include "..\Public\Model.h"

#include "MeshContainer.h"
#include "HierarchyNode.h"
#include "Channel.h"
#include "GameInstance.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
	ZeroMemory(&m_tAnimDesc, sizeof(ANIMDESC));
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_Materials(rhs.m_Materials)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumBones(rhs.m_iNumBones)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eModelType(rhs.m_eModelType)
	, m_tAnimDesc(rhs.m_tAnimDesc)
{
	for (auto& pMeshContainer : rhs.m_Meshes)
		m_Meshes.push_back((CMeshContainer*)pMeshContainer->Clone());

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pMaterials[i]);
	}

	memcpy(m_FilePath, rhs.m_FilePath, sizeof(_tchar) * MAX_PATH);
}

CHierarchyNode * CModel::Get_BonePtr(const char * pBoneName) const
{
	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CHierarchyNode* pNode)
	{
		return !strcmp(pNode->Get_Name(), pBoneName);
	});

	if (iter == m_Bones.end())
		return nullptr;

	return *iter;
}

void CModel::Set_CurrentAnimIndex(_uint iAnimIndex)
{
	if (m_iCurrentAnimIndex != iAnimIndex)
	{
		m_iPreAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = iAnimIndex;
	}
}

void CModel::Set_AnimIndex(_uint iAnimIndex)
{
	m_Animations[m_iCurrentAnimIndex]->Reset();

	m_iCurrentAnimIndex = iAnimIndex;
}

_bool CModel::Picking(_matrix WorldMatrix, _vector vCamWorldPos, _float3 * pPickPoint)
{
	vector<_float3> vPickPoint;

	for (auto& Mesh : m_Meshes)
	{
		if (Mesh->Picking(WorldMatrix, &vPickPoint))
		{
			for (auto iter = vPickPoint.begin(); iter != vPickPoint.end(); ++iter)
			{
				if (iter == vPickPoint.begin())
					*pPickPoint = (*iter);

				_float fPickLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(pPickPoint) - vCamWorldPos));
				_float fPointLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&(*iter)) - vCamWorldPos));

				if (fPickLength > fPointLength)
					*pPickPoint = (*iter);
			}
		}
	}

	if (vPickPoint.empty())
		return false;
	else
		return true;
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const char * pModelFilePath, _fmatrix PivotMatrix)
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

	if (FAILED(Create_MeshContainer()))
		return E_FAIL;

	if (FAILED(Create_Materials(pModelFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	if (TYPE_ANIM == m_eModelType)
	{
		/*if (FAILED(Create_HierarchyNodes(m_pAIScene->mRootNode)))
			return E_FAIL;*/

		char szFilePath[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, m_FilePath, -1, szFilePath, MAX_PATH, nullptr, nullptr);

		char szDir[MAX_PATH] = "";
		char szModelName[MAX_PATH] = "";
		char szAnimName[MAX_PATH] = "_Animation";
		char szExt[MAX_PATH] = "";
		char szAnimationFileName[MAX_PATH] = "";

		char szAddData[MAX_PATH] = "_Add";
		char szAnimationAddFileName[MAX_PATH] = "";

		_splitpath_s(szFilePath, nullptr, 0, szDir, MAX_PATH, szModelName, MAX_PATH, szExt, MAX_PATH);

		strcpy_s(szAnimationFileName, szDir);
		strcat_s(szAnimationFileName, szModelName);
		strcat_s(szAnimationFileName, szAnimName);

		strcpy_s(szAnimationAddFileName, szAnimationFileName);
		
		strcat_s(szAnimationFileName, szExt);

		strcat_s(szAnimationAddFileName, szAddData);
		strcat_s(szAnimationAddFileName, szExt);

		_tchar szTFullPath[MAX_PATH] = TEXT("");
		_tchar szTAddFullPath[MAX_PATH] = TEXT("");

		MultiByteToWideChar(CP_ACP, 0, szAnimationFileName, (int)strlen(szAnimationFileName), szTFullPath, MAX_PATH);
		MultiByteToWideChar(CP_ACP, 0, szAnimationAddFileName, (int)strlen(szAnimationAddFileName), szTAddFullPath, MAX_PATH);

		_ulong dwByte = 0;
		HANDLE hFile = CreateFile(szTFullPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (0 == hFile)
		{
			ERR_MSG(TEXT("Failed to Open File : %s", szTFullPath));
			return E_FAIL;
		}

		if (FAILED(Create_HierarchyNodes_Binary(hFile, &dwByte)))
			return E_FAIL;

		if (nullptr != pArg)
			memcpy(&m_tAnimDesc, pArg, sizeof(ANIMDESC));

		for (auto& pMeshContainer : m_Meshes)
			pMeshContainer->SetUp_Bones_Binary(hFile, &dwByte, this);

		if (FAILED(Create_Animations_Binary(hFile, &dwByte, szTAddFullPath)))
			return E_FAIL;

		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CModel::Initialize_Prototype_Binary(TYPE eModelType, const char * pModelFilePath)
{
	m_eModelType = eModelType;

	_tchar szModelFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pModelFilePath, strlen(pModelFilePath), szModelFilePath, MAX_PATH);

	memcpy(m_FilePath, szModelFilePath, sizeof(_tchar) * MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szModelFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		ERR_MSG(TEXT("Failed to Open File : %s", szModelFilePath));
		return E_FAIL;
	}

	ReadFile(hFile, &m_PivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

	/*_uint iFlag = 0;

	if (TYPE_NONANIM == m_eModelType)
	iFlag = aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;
	else
	iFlag = aiProcess_ConvertToLeftHanded | aiProcess_GenNormals | aiProcess_CalcTangentSpace;*/

	if (FAILED(Create_MeshContainer_Binary(hFile, &dwByte)))
		return E_FAIL;

	if (FAILED(Create_Materials_Binary(hFile, &dwByte)))
		return E_FAIL;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::SetUp_Material(CShader * pShader, const char * pConstantName, _uint iMeshIndex, aiTextureType eType)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	if (nullptr == m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType])
		return S_OK;

	return pShader->Set_ShaderResourceView(pConstantName, m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType]->Get_SRV());
}

HRESULT CModel::Play_Animation(_float fTimeDelta, vector<CAnimation::EVENTTYPE>* pEventTypeVector)
{
	if (m_iPreAnimIndex != 0)
	{
		m_fCurrentTime += fTimeDelta;

		for (auto& pPreChannel : m_Animations[m_iPreAnimIndex]->Get_Channels())
		{
			for (auto& pCurChannel : m_Animations[m_iCurrentAnimIndex]->Get_Channels())
			{
				if (strcmp(pPreChannel->Get_Name(), pCurChannel->Get_Name()))
					continue;

				else
				{
					_vector vScale, vRotation, vPosition;

					if (m_fCurrentTime >= 0.2f)
					{
						vScale = XMLoadFloat3(&(pCurChannel->Get_KeyFrames().front().vScale));
						vRotation = XMLoadFloat4(&(pCurChannel->Get_KeyFrames().front().vRotation));
						vPosition = XMLoadFloat3(&(pCurChannel->Get_KeyFrames().front().vPosition));
						vPosition = XMVectorSetW(vPosition, 1.f);

						m_fCurrentTime = 0.f;
						m_iPreAnimIndex = 0;
					}
					else
					{
						_vector vSourScale, vSourRotation, vSourPosition;
						_vector vDestScale, vDestRotation, vDestPosition;

						KEYFRAME PreKeyFrame = pPreChannel->Get_ChangeKeyFrame();

						vSourScale = XMLoadFloat3(&PreKeyFrame.vScale);
						vSourRotation = XMLoadFloat4(&PreKeyFrame.vRotation);
						vSourPosition = XMLoadFloat3(&PreKeyFrame.vPosition);

						vDestScale = XMLoadFloat3(&(pCurChannel->Get_KeyFrames().front().vScale));
						vDestRotation = XMLoadFloat4(&(pCurChannel->Get_KeyFrames().front().vRotation));
						vDestPosition = XMLoadFloat3(&(pCurChannel->Get_KeyFrames().front().vPosition));

						_float fRatio = m_fCurrentTime / 0.2f;

						vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
						vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
						vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
						vPosition = XMVectorSetW(vPosition, 1.f);
					}

					_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

					pCurChannel->Get_BoneNode()->Set_TransformationMatrix(TransformationMatrix);
				}
			}
		}
	}

	else
		m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(fTimeDelta, pEventTypeVector);

	for (auto& pBoneNode : m_Bones)
		pBoneNode->Invalidate_CombinedTransformationMatrix();

	return S_OK;
}

HRESULT CModel::Render(CShader * pShader, _uint iMeshIndex, _uint iPassIndex)
{
	if (TYPE_ANIM == m_eModelType)
	{
		_float4x4 BoneMatrix[400];

		m_Meshes[iMeshIndex]->Get_BoneMatricese(BoneMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		pShader->Set_MatrixArray("g_BoneMatrices", BoneMatrix, 400);
	}

	pShader->Begin(iPassIndex);

	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Create_MeshContainer(void)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMeshes = m_pAIScene->mNumMeshes;

	m_Meshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_MeshContainer_Binary(HANDLE hFile, _ulong* pdwByte)
{
	if (nullptr == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumMeshes, sizeof(_uint), pdwByte, nullptr);

	m_Meshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer* pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, hFile, pdwByte, this, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials(const char * pModelFilePath)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		MODELMATERIAL ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			char szName[MAX_PATH] = "";
			char szExt[MAX_PATH] = ".dds";
			char szTextureFileName[MAX_PATH] = "";

			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szName, MAX_PATH, nullptr, 0);

			strcpy_s(szTextureFileName, szName);
			strcat_s(szTextureFileName, szExt);

			char szDirectory[MAX_PATH] = "../Bin/Resources/Textures/WhiteDay/";
			char szFullPath[MAX_PATH] = "";

			//_splitpath_s(pModelFilePath, nullptr, 0, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			strcpy_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szTextureFileName);

			_tchar szTFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szTFullPath, MAX_PATH);

			ModelMaterial.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szTFullPath);
			if (nullptr == ModelMaterial.pMaterials[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	return S_OK;
}

HRESULT CModel::Create_Materials_Binary(HANDLE hFile, _ulong * pdwByte)
{
	if (nullptr == hFile)
		return E_FAIL;

	CGameInstance* pInstance = GET_INSTANCE(CGameInstance);

	ReadFile(hFile, &m_iNumMaterials, sizeof(_uint), pdwByte, nullptr);

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MODELMATERIAL ModelMaterial;
		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIAL));

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			char szFullPath[MAX_PATH] = "";
			ReadFile(hFile, szFullPath, sizeof(char) * MAX_PATH, pdwByte, nullptr);

			if (!strcmp(szFullPath, ""))
				continue;

			_tchar szTFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szTFullPath, MAX_PATH);

			CTexture* pFindTexture = pInstance->Find_Texture(szTFullPath);
			if (nullptr == pFindTexture)
			{
				CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, szTFullPath);
				ModelMaterial.pMaterials[j] = pTexture;
				//Safe_AddRef(pTexture);
				pInstance->Add_Texture(szTFullPath, pTexture);
			}
			else
			{
				ModelMaterial.pMaterials[j] = pFindTexture;
				Safe_AddRef(pFindTexture);
			}


			if (nullptr == ModelMaterial.pMaterials[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(const aiNode * pNode, CHierarchyNode* pParent)
{
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_Bones.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode);

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes_Binary(HANDLE hFile, _ulong* pdwByte, CHierarchyNode * pParent)
{
	BINBONE BinBone;

	ReadFile(hFile, &BinBone, sizeof(BINBONE), pdwByte, nullptr);

	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(BinBone, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_Bones.push_back(pHierarchyNode);

	for (_uint i = 0; i < BinBone.iNumChildren; ++i)
		Create_HierarchyNodes_Binary(hFile, pdwByte, pHierarchyNode);

	return S_OK;
}

HRESULT CModel::Create_Animations(void)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumAnimations = m_pAIScene->mNumAnimations;

	if (nullptr == m_tAnimDesc.pNonLoopArray)
	{
		for (_uint i = 0; i < m_iNumAnimations; ++i)
		{
			aiAnimation* pAIAnimation = m_pAIScene->mAnimations[i];

			CAnimation* pAnimation = CAnimation::Create(this, pAIAnimation);
			if (nullptr == pAnimation)
				return E_FAIL;

			m_Animations.push_back(pAnimation);
		}
	}
	else
	{
		for (_uint i = 0; i < m_tAnimDesc.iArraySize; ++i)
		{
			for (_uint j = 0; j < m_iNumAnimations; ++j)
			{
				aiAnimation* pAIAnimation = m_pAIScene->mAnimations[j];

				CAnimation* pAnimation = nullptr;

				if (m_tAnimDesc.pNonLoopArray[i] == j)
				{
					pAnimation = CAnimation::Create(this, pAIAnimation, false);
					if (nullptr == pAnimation)
						return E_FAIL;
				}
				else
				{
					pAnimation = CAnimation::Create(this, pAIAnimation);
					if (nullptr == pAnimation)
						return E_FAIL;
				}

				m_Animations.push_back(pAnimation);
			}
		}
	}

	return S_OK;
}

HRESULT CModel::Create_Animations_Binary(HANDLE hFile, _ulong * pdwByte, const _tchar* pAddDataFilePath)
{
	if (nullptr == hFile)
		return E_FAIL;

	ReadFile(hFile, &m_iNumAnimations, sizeof(_uint), pdwByte, nullptr);

	_ulong dwAddByte = 0;
	HANDLE hAddFile = CreateFile(pAddDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(hFile, pdwByte, this, hAddFile, &dwAddByte);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}

	CloseHandle(hAddFile);

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eModelType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	/*if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}*/

	if (FAILED(pInstance->Initialize_Prototype_Binary(eModelType, pModelFilePath)))
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

	if (nullptr != m_tAnimDesc.pNonLoopArray)
		Safe_Delete_Array(m_tAnimDesc.pNonLoopArray);

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);
	m_Bones.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pMaterials[i]);
	}
	m_Materials.clear();

	m_Importer.FreeScene();
}
