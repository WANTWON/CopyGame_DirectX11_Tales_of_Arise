#include "..\Public\Model.h"
#include "MeshContainer.h"
#include "Texture.h"
#include "Shader.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Channel.h"
#include "MeshContainer_Instance.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)//, m_bIsProto(true) // �߰�
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	//, m_pAIScene(rhs.m_pAIScene)
	, m_Materials(rhs.m_Materials)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_iNumBones(rhs.m_iNumBones)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_eModelType(rhs.m_eModelType)
	//, m_pBin_AIScene(rhs.m_pBin_AIScene)	// �߰�
	, m_iCurrentAnimIndex(rhs.m_iCurrentAnimIndex)	// �߰�
	, m_iNumAnimations(rhs.m_iNumAnimations)	// �߰�
	//, m_DataMaterials(rhs.m_DataMaterials)	// �߰�
	//, m_bIsBin(rhs.m_bIsBin)	// �߰�
	, m_iPreAnimIndex(rhs.m_iPreAnimIndex)
{
	for (auto& pMeshContainer : rhs.m_Meshes)
		m_Meshes.push_back((CMeshContainer*)pMeshContainer->Clone());

	for (auto& pMeshContainer_Instance : rhs.m_InstanceMeshes)
		m_InstanceMeshes.push_back((CMeshContainer_Instance*)pMeshContainer_Instance->Clone());

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pMaterials[i]);
	}

	memcpy(m_szFilePath, rhs.m_szFilePath, sizeof(_tchar) * MAX_PATH);
}

CHierarchyNode * CModel::Get_BonePtr(const char * pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CHierarchyNode* pNode)
	{
		return !strcmp(pNode->Get_Name(), pBoneName);
	});

	if (iter == m_Bones.end())
		return nullptr;

	return *iter;
}

vector<ANIMEVENT> CModel::Get_Events(void)
{
	return m_Animations[m_iCurrentAnimIndex]->Get_Events();
}

void CModel::Get_MoveTransformationMatrix(const char * pBoneName, _vector * pTranslation, _float * pRotation)
{
	for (auto& pBone : m_Bones)
	{
		// 이름 비교
		if (!strcmp(pBoneName, pBone->Get_Name()))
		{
			_vector vecMoveTranslation = pBone->Get_Translation();
			if (m_bInterupted)
				vecMoveTranslation = XMVectorSet(0.f, 0.f, 0.f, 1.f);

			_float Rotation = pBone->Get_RotationRadian();

			// 이동 값, 회전 값 복사
			memcpy(pTranslation, &vecMoveTranslation, sizeof(_vector));
			memcpy(pRotation, &Rotation, sizeof(_vector));
		}
	}
}

void CModel::Set_CurrentAnimIndex(_uint iAnimIndex)
{
	if (!m_bInterupted)
	{
		m_iPreAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = iAnimIndex;
		m_bInterupted = true;
	}
	else
	{
		m_Animations[m_iPreAnimIndex]->Reset();
		m_iPreAnimIndex = m_iCurrentAnimIndex;
		m_iCurrentAnimIndex = iAnimIndex;
	}
}

_bool CModel::Is_Keyframe(char * pChannelName, _uint iKeyframe)
{
	return m_Animations[m_iCurrentAnimIndex]->Is_Keyframe(pChannelName, iKeyframe);
}

_bool CModel::Under_Keyframe(char * pChannelName, _uint iKeyframe)
{
	return m_Animations[m_iCurrentAnimIndex]->Under_Keyframe(pChannelName, iKeyframe);
}

_bool CModel::Over_Keyframe(char * pChannelName, _uint iKeyframe)
{
	return m_Animations[m_iCurrentAnimIndex]->Over_Keyframe(pChannelName, iKeyframe);
}

_bool CModel::Between_Keyframe(char * pChannelName, _uint iKeyframeLower, _uint iKeyframeUpper)
{
	return m_Animations[m_iCurrentAnimIndex]->Between_Keyframe(pChannelName, iKeyframeLower, iKeyframeUpper);
}

void CModel::Set_TimeReset()
{
	m_Animations[m_iCurrentAnimIndex]->Set_TimeReset();
}

void CModel::Reset(void)
{
	m_Animations[m_iCurrentAnimIndex]->Reset();
}

HRESULT CModel::Initialize_Prototype(TYPE eModelType, const char * pModelFilePath, _fmatrix PivotMatrix)
{
	m_eModelType = eModelType;

	_tchar szModelFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pModelFilePath, (_int)strlen(pModelFilePath), szModelFilePath, MAX_PATH);

	memcpy(m_szFilePath, szModelFilePath, sizeof(_tchar) * MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szModelFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		_tchar szErrorMsg[MAX_PATH] = TEXT("Failed to Open File : ");
		lstrcat(szErrorMsg, szModelFilePath);
		ERR_MSG(szErrorMsg);
		return E_FAIL;
	}

	ReadFile(hFile, &m_PivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

	if (FAILED(Create_MeshContainer(hFile, &dwByte)))
		return E_FAIL;

	if (FAILED(Create_Materials(hFile, &dwByte)))
		return E_FAIL;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Instance_Initialize_Prototype(TYPE eModelType, const char * pModelFilePath, const char * pDataFilePath, _fmatrix PivotMatrix)
{
	m_eModelType = eModelType;

	_tchar szModelFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pModelFilePath, (_int)strlen(pModelFilePath), szModelFilePath, MAX_PATH);

	memcpy(m_szFilePath, szModelFilePath, sizeof(_tchar) * MAX_PATH);

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szModelFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		_tchar szErrorMsg[MAX_PATH] = TEXT("Failed to Open File : ");
		lstrcat(szErrorMsg, szModelFilePath);
		ERR_MSG(szErrorMsg);
		return E_FAIL;
	}

	ReadFile(hFile, &m_PivotMatrix, sizeof(_float4x4), &dwByte, nullptr);

	ReadFile(hFile, &m_iNumMeshes, sizeof(_uint), &dwByte, nullptr);

	m_InstanceMeshes.reserve(m_iNumMeshes);

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMeshContainer_Instance* pMeshContainer = CMeshContainer_Instance::Create(m_pDevice, m_pContext, m_eModelType, hFile, &dwByte, pDataFilePath, this, XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_InstanceMeshes.push_back(pMeshContainer);
	}

	if (FAILED(Create_Materials(hFile, &dwByte)))
		return E_FAIL;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{
	if (TYPE_ANIM == m_eModelType)
	{
		char szFilePath[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, m_szFilePath, -1, szFilePath, MAX_PATH, nullptr, nullptr);

		char szDir[MAX_PATH] = "";
		char szName[MAX_PATH] = "";
		char szAnimName[MAX_PATH] = "_Animation";
		char szExt[MAX_PATH] = "";
		char szAnimationFileName[MAX_PATH] = "";
		
		char szAddData[MAX_PATH] = "_Add";
		char szAnimationAddFileName[MAX_PATH] = "";

		_splitpath_s(szFilePath, nullptr, 0, szDir, MAX_PATH, szName, MAX_PATH, szExt, MAX_PATH);

		strcpy_s(szAnimationFileName, szDir);
		strcat_s(szAnimationFileName, szName);
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
			_tchar szErrorMsg[MAX_PATH] = TEXT("Failed to Open File : ");
			lstrcat(szErrorMsg, szTFullPath);
			ERR_MSG(szErrorMsg);
			return E_FAIL;
		}

		if (FAILED(Create_HierarchyNodes(hFile, &dwByte)))
			return E_FAIL;

		for (auto& pMeshContainer : m_Meshes)
			pMeshContainer->SetUp_Bones(hFile, &dwByte, this);

		if (FAILED(Create_Animations(hFile, &dwByte, szTAddFullPath)))
			return E_FAIL;

		CloseHandle(hFile);
	}

	return S_OK;
}

HRESULT CModel::SetUp_Material(CShader * pShader, const char * pConstantName, _uint iMeshIndex, aiTextureType eType, _uint TextureNum)
{
	if (iMeshIndex >= m_iNumMeshes)
		return E_FAIL;

	if (TYPE_NONANIM_INSTANCE != m_eModelType)
	{
		if (nullptr == m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType])
			return S_OK;

		return pShader->Set_ShaderResourceView(pConstantName, m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType]->Get_SRV());
	}
	else
	{
		if (nullptr == m_Materials[m_InstanceMeshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType])
			return S_OK;

		return pShader->Set_ShaderResourceView(pConstantName, m_Materials[m_InstanceMeshes[iMeshIndex]->Get_MaterialIndex()].pMaterials[eType]->Get_SRV());
	}
}

_bool CModel::Play_Animation(_float fTimeDelta, _bool isLoop, const char* pBoneName)
{
	_bool isAnimLoop = false;

	if (m_bInterupted)
	{
		_bool isLinearEnd = false;

		m_fLinearCurrentTime += fTimeDelta;

		vector<CChannel*> PreChannels = m_Animations[m_iPreAnimIndex]->Get_Channels();
		vector<CChannel*> CurChannels = m_Animations[m_iCurrentAnimIndex]->Get_Channels();

		for (_int i = 0; i < PreChannels.size(); ++i)
		{
			_float fPreLinearTime = PreChannels[i]->Get_LinearKeyFrame().fTime;
			_float fCurLinearTime = CurChannels[i]->Get_LinearKeyFrame().fTime;

			PreChannels[i]->Invalidate_TransformationMatrix(fPreLinearTime + fTimeDelta, pBoneName);
			CurChannels[i]->Invalidate_TransformationMatrix(fCurLinearTime + fTimeDelta, pBoneName);

			_vector vScale, vRotation, vPosition;

			if (m_fLinearCurrentTime >= 0.1f)
			{
				KEYFRAME CurLinearKeyFrame = CurChannels[i]->Get_LinearKeyFrame();

				vScale = XMLoadFloat3(&CurLinearKeyFrame.vScale);
				vRotation = XMLoadFloat4(&CurLinearKeyFrame.vRotation);
				vPosition = XMLoadFloat3(&CurLinearKeyFrame.vPosition);
				vPosition = XMVectorSetW(vPosition, 1.f);

				isLinearEnd = true;
			}
			else
			{
				_vector vSourScale, vSourRotation, vSourPosition;
				_vector vDestScale, vDestRotation, vDestPosition;

				KEYFRAME PreLinearKeyFrame = PreChannels[i]->Get_LinearKeyFrame();
				KEYFRAME CurLinearKeyFrame = CurChannels[i]->Get_LinearKeyFrame();

				vSourScale = XMLoadFloat3(&PreLinearKeyFrame.vScale);
				vSourRotation = XMLoadFloat4(&PreLinearKeyFrame.vRotation);
				vSourPosition = XMLoadFloat3(&PreLinearKeyFrame.vPosition);

				vDestScale = XMLoadFloat3(&CurLinearKeyFrame.vScale);
				vDestRotation = XMLoadFloat4(&CurLinearKeyFrame.vRotation);
				vDestPosition = XMLoadFloat3(&CurLinearKeyFrame.vPosition);

				_float fRatio = m_fLinearCurrentTime / 0.1f;

				vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
				vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
				vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);
				vPosition = XMVectorSetW(vPosition, 1.f);
			}

			_matrix	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

			PreChannels[i]->Get_BoneNode()->Set_TransformationMatrix(TransformationMatrix);
		}

		if (isLinearEnd)
		{
			m_Animations[m_iPreAnimIndex]->Reset();
			
			m_Animations[m_iCurrentAnimIndex]->Set_CurrentTime(m_fLinearCurrentTime);

			m_fLinearCurrentTime = 0.f;

			m_bInterupted = false;
		}
	}
	else
		isAnimLoop = m_Animations[m_iCurrentAnimIndex]->Invalidate_TransformationMatrix(fTimeDelta, isLoop, pBoneName);

	for (auto& pBoneNode : m_Bones)
		pBoneNode->Invalidate_CombinedTransformationmatrix(pBoneName);

	return isAnimLoop;
}

HRESULT CModel::Render(CShader * pShader, _uint iMeshIndex, _uint iPassIndex)
{
	/* �޽ú��� �׸���. */
	/* �޽� �� ���⤷�� �ִ� ������ ����� �����´�. */
	if (TYPE_ANIM == m_eModelType)
	{
		//_float4x4		BoneMatrix[400];

		/* �޽ÿ��� �����ؼ� �ϰ� ����ִ� ������ �迭�� ��ƿ�.
		�� = ���� ������ * ���� �Ĺ��ε��Ʈ���� * ���ʻ���������� */
		//m_Meshes[iMeshIndex]->Get_BoneMatrices(BoneMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		//pShader->Set_MatrixArray("g_BoneMatrices", BoneMatrix, 400);

		_uint iBoneSize = m_Meshes[iMeshIndex]->Get_NumBones();
		_float4x4* BoneMatrix = new _float4x4[iBoneSize];

		m_Meshes[iMeshIndex]->Get_BoneMatrices_Texture(BoneMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		ID3D11Texture2D* pTexture2D = nullptr;

		D3D11_TEXTURE2D_DESC TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = 128;
		TextureDesc.Height = 128;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
		TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		TextureDesc.MiscFlags = 0;

		_float4* pPixel = new _float4[TextureDesc.Width * TextureDesc.Height];

		_uint iMatrix = 0;

		for (_uint i = 0; i < TextureDesc.Height; ++i)
		{
			for (_uint j = 0; j < TextureDesc.Width; j = j + 4)
			{
				_uint iIndex = i * TextureDesc.Width + j;

				if (iBoneSize > iMatrix)
				{
					pPixel[iIndex + 0] = _float4(BoneMatrix[iMatrix].m[0][0], BoneMatrix[iMatrix].m[0][1], BoneMatrix[iMatrix].m[0][2], BoneMatrix[iMatrix].m[0][3]);
					pPixel[iIndex + 1] = _float4(BoneMatrix[iMatrix].m[1][0], BoneMatrix[iMatrix].m[1][1], BoneMatrix[iMatrix].m[1][2], BoneMatrix[iMatrix].m[1][3]);
					pPixel[iIndex + 2] = _float4(BoneMatrix[iMatrix].m[2][0], BoneMatrix[iMatrix].m[2][1], BoneMatrix[iMatrix].m[2][2], BoneMatrix[iMatrix].m[2][3]);
					pPixel[iIndex + 3] = _float4(BoneMatrix[iMatrix].m[3][0], BoneMatrix[iMatrix].m[3][1], BoneMatrix[iMatrix].m[3][2], BoneMatrix[iMatrix].m[3][3]);
				}
				else
				{
					pPixel[iIndex + 0] = _float4(1.f, 0.f, 0.f, 0.f);
					pPixel[iIndex + 1] = _float4(0.f, 1.f, 0.f, 0.f);
					pPixel[iIndex + 2] = _float4(0.f, 0.f, 1.f, 0.f);
					pPixel[iIndex + 3] = _float4(0.f, 0.f, 0.f, 1.f);
				}

				++iMatrix;
			}
		}
		
		D3D11_SUBRESOURCE_DATA SubResourceData;
		ZeroMemory(&SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));

		SubResourceData.pSysMem = pPixel;
		SubResourceData.SysMemPitch = 128 * 4;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &SubResourceData, &pTexture2D)))
			return E_FAIL;

		D3D11_MAPPED_SUBRESOURCE SubResource;
		ZeroMemory(&SubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		if (FAILED(m_pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
			return E_FAIL;
		
		memcpy(SubResource.pData, pPixel, sizeof(_float4) * TextureDesc.Width * TextureDesc.Height);

		m_pContext->Unmap(pTexture2D, 0);
		
		ID3D11ShaderResourceView* pSRV = nullptr;

		if (FAILED(DirectX::SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../../../Bin/Bin_Data/Test/Bone.dds"))))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D, nullptr, &pSRV)))
			return E_FAIL;

		pShader->Set_ShaderResourceView("g_BoneTexture", pSRV);

		Safe_Delete_Array(BoneMatrix);
		Safe_Delete_Array(pPixel);
		Safe_Release(pTexture2D);
	}

	pShader->Begin(iPassIndex);

	if (TYPE_NONANIM_INSTANCE != m_eModelType)
		m_Meshes[iMeshIndex]->Render();
	else
		m_InstanceMeshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::RenderShadow(CShader * pShader, _uint iMeshIndex, _uint iLevelIndex, _uint iPassIndex)
{
	
	/* �޽ú��� �׸���. */
	/* �޽� �� ���⤷�� �ִ� ������ ����� �����´�. */
	if (TYPE_ANIM == m_eModelType)
	{
		_float4x4		BoneMatrix[400];

		/* �޽ÿ��� �����ؼ� �ϰ� ����ִ� ������ �迭�� ��ƿ�.
		�� = ���� ������ * ���� �Ĺ��ε��Ʈ���� * ���ʻ���������� */
		m_Meshes[iMeshIndex]->Get_BoneMatrices(BoneMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		pShader->Set_MatrixArray("g_BoneMatrices", BoneMatrix, 400);

	}

	pShader->Begin(iPassIndex);

	if (TYPE_NONANIM_INSTANCE != m_eModelType)
		m_Meshes[iMeshIndex]->Render();
	else
		m_InstanceMeshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Set_AnimationReset()
{
	m_Animations[m_iCurrentAnimIndex]->Set_TimeReset();

	return S_OK;
}

_bool CModel::Picking(CTransform * pTransform, _float3 * pOut)
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		if (m_Meshes[i]->Picking(pTransform, pOut))
			return true;
	}

	return false;
}

void CModel::Update(_float fRadius)
{
	if (TYPE_NONANIM_INSTANCE == m_eModelType)
	{
		for (auto& pInstanceMesh : m_InstanceMeshes)
			pInstanceMesh->Tick(fRadius);
	}
}

//HRESULT CModel::Create_MeshContainer()
//{
//	if (nullptr == m_pAIScene)
//		return E_FAIL;
//
//	m_iNumMeshes = m_pAIScene->mNumMeshes;
//
//	m_Meshes.reserve(m_iNumMeshes);
//
//	for (_uint i = 0; i < m_iNumMeshes; ++i)
//	{
//		aiMesh*		pAIMesh = m_pAIScene->mMeshes[i];
//
//		CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pContext, m_eModelType, pAIMesh, this, XMLoadFloat4x4(&m_PivotMatrix));
//		if (nullptr == pMeshContainer)
//			return E_FAIL;
//
//		m_Meshes.push_back(pMeshContainer);
//	}
//
//	return S_OK;
//}

//HRESULT CModel::Create_Materials(const char* pModelFilePath)
//{
//	if (nullptr == m_pAIScene)
//		return E_FAIL;
//
//	m_iNumMaterials = m_pAIScene->mNumMaterials;
//
//	for (_uint i = 0; i < m_iNumMaterials; ++i)
//	{
//		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];
//
//		MODELMATERIAL		ModelMaterial;
//		ZeroMemory(&ModelMaterial, sizeof(MODELMATERIAL));
//
//
//		DATA_BINMATERIAL	DataMaterialDesc;	// �߰�
//		ZeroMemory(&DataMaterialDesc, sizeof(DATA_BINMATERIAL)); // �߰�
//
//		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
//		{
//			aiString		strPath;
//
//			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
//				continue;
//
//			char			szName[MAX_PATH] = "";
//			char			szExt[MAX_PATH] = "";
//			char			szTextureFileName[MAX_PATH] = "";
//
//			_splitpath_s(strPath.data, nullptr, 0, nullptr, 0, szName, MAX_PATH, szExt, MAX_PATH);
//
//			strcpy_s(szTextureFileName, szName);
//			strcat_s(szTextureFileName, szExt);
//
//			memcpy(&DataMaterialDesc.cNames[j], &szTextureFileName, sizeof(char) * MAX_PATH); // �߰�
//
//
//			char			szDirectory[MAX_PATH] = "";
//			char			szFullPath[MAX_PATH] = "";
//			_splitpath_s(pModelFilePath, nullptr, 0, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
//
//			strcpy_s(szFullPath, szDirectory);
//			strcat_s(szFullPath, szTextureFileName);
//
//			_tchar			szRealPath[MAX_PATH] = TEXT("");
//
//			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szRealPath, MAX_PATH);
//
//			ModelMaterial.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szRealPath);
//			if (nullptr == ModelMaterial.pMaterials[j])
//				return E_FAIL;
//		}
//
//		m_Materials.push_back(ModelMaterial);
//		m_DataMaterials.push_back(DataMaterialDesc); // �߰�
//	}
//
//	return S_OK;
//}
//
//
//HRESULT CModel::Create_HierarchyNodes(const aiNode* pNode, CHierarchyNode* pParent)
//{
//	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode, pParent);
//	if (nullptr == pHierarchyNode)
//		return E_FAIL;
//
//	m_Bones.push_back(pHierarchyNode);
//
//	for (_uint i = 0; i < pNode->mNumChildren; ++i)
//		Create_HierarchyNodes(pNode->mChildren[i], pHierarchyNode);
//
//	return S_OK;
//}
//
//HRESULT CModel::Create_Animations()
//{
//	if (nullptr == m_pAIScene)
//		return E_FAIL;
//
//	m_iNumAnimations = m_pAIScene->mNumAnimations;
//
//	for (_uint i = 0; i < m_iNumAnimations; ++i)
//	{
//		aiAnimation*	pAIAnimation = m_pAIScene->mAnimations[i];
//
//		CAnimation*		pAnimation = CAnimation::Create(this, pAIAnimation);
//		if (nullptr == pAIAnimation)
//			return E_FAIL;
//
//		m_Animations.push_back(pAnimation);
//	}
//
//	return S_OK;
//}

HRESULT CModel::Create_MeshContainer(HANDLE hFile, _ulong * pdwByte)
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

HRESULT CModel::Create_Materials(HANDLE hFile, _ulong * pdwByte)
{
	if (nullptr == hFile)
		return E_FAIL;

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

			ModelMaterial.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szTFullPath);
			if (nullptr == ModelMaterial.pMaterials[j])
				return E_FAIL;
		}

		m_Materials.push_back(ModelMaterial);
	}

	return S_OK;
}

HRESULT CModel::Create_HierarchyNodes(HANDLE hFile, _ulong * pdwByte, CHierarchyNode * pParent)
{
	BINBONE BinBone;

	ReadFile(hFile, &BinBone, sizeof(BINBONE), pdwByte, nullptr);

	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(BinBone, pParent);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_Bones.push_back(pHierarchyNode);

	for (_uint i = 0; i < BinBone.iNumChildren; ++i)
		Create_HierarchyNodes(hFile, pdwByte, pHierarchyNode);

	return S_OK;
}

HRESULT CModel::Create_Animations(HANDLE hFile, _ulong * pdwByte, const _tchar* pAddDataFilePath)
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
	CModel*	pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PivotMatrix)))
	{
		//ERR_MSG(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eModelType, const char * pModelFilePath, const char * pDataFilePath, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Instance_Initialize_Prototype(eModelType, pModelFilePath, pDataFilePath, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

//CModel * CModel::Bin_Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, DATA_BINSCENE * pScene, TYPE eType, const char * pModelFilePath, _fmatrix PivotMatrix)
//{
//	CModel*			pInstance = new CModel(pDevice, pContext);
//
//	if (FAILED(pInstance->Bin_Initialize_Prototype(pScene, eType, pModelFilePath, PivotMatrix)))
//	{
//		ERR_MSG(TEXT("Failed To Created : CModel_Bin"));
//		Safe_Release(pInstance);
//	}
//
//	return pInstance;
//}

CComponent * CModel::Clone(void * pArg)
{
	CModel*	pInstance = new CModel(*this);

	//if (!m_pBin_AIScene)	// �߰�
	//{
	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CModel"));
		Safe_Release(pInstance);
	}
	/*}
	else
	{
		if (FAILED(pInstance->Bin_Initialize(pArg)))
		{
			ERR_MSG(TEXT("Failed To Cloned : CModel_Bin"));
			Safe_Release(pInstance);
		}
	}*/

	return pInstance;
}

//HRESULT CModel::Get_HierarchyNodeData(DATA_BINSCENE * pBinScene)
//{
//	if (0 == m_Bones.size())
//	{
//		pBinScene->pBinNodes = nullptr;
//		return S_OK;
//	}
//
//	pBinScene->pBinNodes = new DATA_BINNODE[m_Bones.size()];
//	pBinScene->iNodeCount = (int)m_Bones.size();
//
//	for (_int i = 0; i < m_Bones.size(); ++i)
//	{
//		DATA_BINNODE Bin_Node;
//		ZeroMemory(&Bin_Node, sizeof(DATA_BINNODE));
//
//		const char* pMyName = m_Bones[i]->Get_Name();
//		const char* pParentName = m_Bones[i]->Get_ParentName();
//		memcpy(&Bin_Node.cName, pMyName, sizeof(char) * MAX_PATH);
//		memcpy(&Bin_Node.cParent, pParentName, sizeof(char) * MAX_PATH);
//
//		Bin_Node.mTransform = m_Bones[i]->Get_OriTransformationMatrix();
//
//		pBinScene->pBinNodes[i] = Bin_Node;
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Get_MaterialData(DATA_BINSCENE * pBinScene)
//{
//	pBinScene->iMaterialCount = m_iNumMaterials;
//	pBinScene->pBinMaterial = new DATA_BINMATERIAL[m_iNumMaterials];
//
//	for (_int i = 0; i < m_DataMaterials.size(); ++i)
//	{
//		memcpy(&pBinScene->pBinMaterial[i], &m_DataMaterials[i], sizeof(DATA_BINMATERIAL));
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Get_MeshData(DATA_BINSCENE * pBinScene)
//{
//	pBinScene->iMeshCount = m_iNumMeshes;
//	pBinScene->pBinMesh = new DATA_BINMESH[m_iNumMeshes];
//	for (_uint i = 0; i < m_iNumMeshes; ++i)
//	{
//		DATA_BINMESH BinMesh;
//		m_Meshes[i]->Get_MeshData(&BinMesh);
//		memcpy(&pBinScene->pBinMesh[i], &BinMesh, sizeof(DATA_BINMESH));
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Get_AnimData(DATA_BINSCENE * pBinScene)
//{
//	if (0 == m_iNumAnimations)
//	{
//		pBinScene->pBinAnim = nullptr;
//		pBinScene->iNumAnimations = 0;
//		return S_OK;
//	}
//
//	pBinScene->iNumAnimations = m_iNumAnimations;
//	pBinScene->pBinAnim = new DATA_BINANIM[m_iNumAnimations];
//
//	for (_uint i = 0; i < m_iNumAnimations; ++i)
//	{
//		m_Animations[i]->Get_AnimData(&pBinScene->pBinAnim[i]);
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Bin_Ready_MeshContainers(_fmatrix PivotMatrix)
//{
//	m_iNumMeshes = m_pBin_AIScene->iMeshCount;
//
//	for (_uint i = 0; i < m_iNumMeshes; ++i)
//	{
//		CMeshContainer*		pMeshContainer = CMeshContainer::Bin_Create(m_pDevice, m_pContext, m_eModelType, &m_pBin_AIScene->pBinMesh[i], this, PivotMatrix);
//		if (nullptr == pMeshContainer)
//			return E_FAIL;
//
//		m_Meshes.push_back(pMeshContainer);
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Bin_Ready_Materials(const char * pModelFilePath)
//{
//	if (nullptr == m_pBin_AIScene)
//		return E_FAIL;
//
//	m_iNumMaterials = m_pBin_AIScene->iMaterialCount;
//
//	for (_uint i = 0; i < m_iNumMaterials; ++i)
//	{
//		MODELMATERIAL		MaterialDesc;
//		ZeroMemory(&MaterialDesc, sizeof(MODELMATERIAL));
//
//		DATA_BINMATERIAL		pAIMaterial = m_pBin_AIScene->pBinMaterial[i];
//
//		DATA_BINMATERIAL		DataMaterialDesc;
//		ZeroMemory(&DataMaterialDesc, sizeof(DATA_BINMATERIAL));
//
//		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
//		{
//			if (!strcmp(pAIMaterial.cNames[j], ""))
//				continue;
//
//
//			char			szFullPath[MAX_PATH] = "";
//			char			szExt[MAX_PATH] = "";
//
//			_splitpath_s(pAIMaterial.cNames[j], nullptr, 0, nullptr, 0, szFullPath, MAX_PATH, szExt, MAX_PATH);
//
//			strcpy_s(szFullPath, pModelFilePath);
//			strcat_s(szFullPath, pAIMaterial.cNames[j]);
//
//			_tchar			szWideFullPath[MAX_PATH] = TEXT("");
//
//			MultiByteToWideChar(CP_ACP, 0, szFullPath, (int)strlen(szFullPath), szWideFullPath, MAX_PATH);
//
//			memcpy(&DataMaterialDesc.cNames[j], &pAIMaterial.cNames[j], sizeof(char) * MAX_PATH);
//
//			MaterialDesc.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szWideFullPath);
//			if (nullptr == MaterialDesc.pMaterials[j])
//				continue; //return E_FAIL;
//		}
//
//		m_Materials.push_back(MaterialDesc);
//		m_DataMaterials.push_back(DataMaterialDesc);
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Bin_Ready_HierarchyNodes()
//{
//	for (int i = 0; i < m_pBin_AIScene->iNodeCount; ++i)
//	{
//		DATA_BINNODE Node = m_pBin_AIScene->pBinNodes[i];
//
//		CHierarchyNode*		pHierarchyNode = CHierarchyNode::Bin_Create(&Node);
//		if (nullptr == pHierarchyNode)
//			return E_FAIL;
//
//		m_Bones.push_back(pHierarchyNode);
//	}
//
//	return S_OK;
//}
//
//HRESULT CModel::Bin_Ready_Animations(CModel* pModel)
//{
//	m_iNumAnimations = m_pBin_AIScene->iNumAnimations;
//
//	for (_int i = 0; i < m_pBin_AIScene->iNumAnimations; ++i)
//	{
//		DATA_BINANIM*		pAIAnimation = &m_pBin_AIScene->pBinAnim[i];
//
//		CAnimation*			pAnimation = CAnimation::Bin_Create(pAIAnimation, this);
//		if (nullptr == pAnimation)
//			return E_FAIL;
//
//		m_Animations.push_back(pAnimation);
//	}
//	return S_OK;
//}
//
//HRESULT CModel::Safe_Release_Scene()
//{
//	Safe_Delete_Array(m_pBin_AIScene->pBinNodes);
//	Safe_Delete_Array(m_pBin_AIScene->pBinMaterial);
//
//	for (_int i = 0; i < m_pBin_AIScene->iMeshCount; ++i)
//	{
//		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pAnimVertices);
//		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pNonAnimVertices);
//		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pIndices);
//		Safe_Delete_Array(m_pBin_AIScene->pBinMesh[i].pBones);
//	}
//	Safe_Delete_Array(m_pBin_AIScene->pBinMesh);
//
//
//	for (_int i = 0; i < m_pBin_AIScene->iNumAnimations; ++i)
//	{
//		for (_int j = 0; j < m_pBin_AIScene->pBinAnim[i].iNumChannels; ++j)
//		{
//			Safe_Delete_Array(m_pBin_AIScene->pBinAnim[i].pBinChannel[j].pKeyFrames);
//		}
//		Safe_Delete_Array(m_pBin_AIScene->pBinAnim[i].pBinChannel);
//	}
//	Safe_Delete_Array(m_pBin_AIScene->pBinAnim);
//
//	Safe_Delete(m_pBin_AIScene);
//
//	return S_OK;
//}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);
	m_Bones.clear();

	for (auto& pInstanceMeshContainer : m_InstanceMeshes)
		Safe_Release(pInstanceMeshContainer);
	m_InstanceMeshes.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pMaterials[i]);
	}
	m_Materials.clear();

	//m_Importer.FreeScene();

	//if (m_pBin_AIScene && m_bIsProto)	// �߰�
	//{
	//	Safe_Release_Scene();
	//}
}
