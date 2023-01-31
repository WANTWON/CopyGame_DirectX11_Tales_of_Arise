#include "..\Public\MeshContainer.h"
#include "HierarchyNode.h"
#include "Picking.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: CVIBuffer(rhs)
	, m_pAIMesh(rhs.m_pAIMesh)
	, m_pNonAnimVertices(rhs.m_pNonAnimVertices)
	, m_pAnimVertices(rhs.m_pAnimVertices)
	, m_pIndices(rhs.m_pIndices)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CMeshContainer::Get_BoneMatricese(_float4x4 * pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	_uint iNumBones = 0;

	for (auto& pBoneNode : m_Bones)
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], pBoneNode->Get_OffsetMatrix() * pBoneNode->Get_CombinedTransformationMatrix() * PivotMatrix);
}

HRESULT CMeshContainer::Initialize_Prototype_Binary(CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, CModel* pModel, _fmatrix PivotMatrix)
{
	ReadFile(hFile, m_szName, sizeof(char) * MAX_PATH, pdwByte, nullptr);
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), pdwByte, nullptr);

#pragma region VERTICES
	HRESULT hr = 0;

	if (CModel::TYPE_NONANIM == eModelType)
		hr = Create_VertexBuffer_NonAnim(hFile, pdwByte, PivotMatrix);
	else
		hr = Create_VertexBuffer_Anim(hFile, pdwByte, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion
#pragma region INDICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), pdwByte, nullptr);
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ulong);

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
		ReadFile(hFile, &pIndices[i], sizeof(FACEINDICES32), pdwByte, nullptr);

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}

HRESULT CMeshContainer::Initialize_Prototype(CModel::TYPE eModelType, const aiMesh * pAIMesh, class CModel* pModel, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_pAIMesh = pAIMesh;

#pragma region VERTICES
	HRESULT hr = 0;

	if (CModel::TYPE_NONANIM == eModelType)
		hr = Create_VertexBuffer_NonAnimModel(pAIMesh, PivotMatrix);
	else
		hr = Create_VertexBuffer_AnimModel(pAIMesh, pModel);

	if (FAILED(hr))
		return E_FAIL;
#pragma endregion
#pragma region INDICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	m_iNumPrimitive = m_pAIMesh->mNumFaces;
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ulong);

	m_pIndices = new FACEINDICES32[m_iNumPrimitive];

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		pIndices[i]._0 = m_pIndices[i]._0 = m_pAIMesh->mFaces[i].mIndices[0];
		pIndices[i]._1 = m_pIndices[i]._1 = m_pAIMesh->mFaces[i].mIndices[1];
		pIndices[i]._2 = m_pIndices[i]._2 = m_pAIMesh->mFaces[i].mIndices[2];
	}

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;
	
	Safe_Delete_Array(pIndices);
#pragma endregion
	return S_OK;
}

HRESULT CMeshContainer::Initialize(void * pArg)
{
	m_isCloned = true;

	return S_OK;
}

_bool CMeshContainer::Picking(_matrix WorldMatrix, vector<_float3>* vPickPoint)
{
	CPicking* pPicking = GET_INSTANCE(CPicking);

	_matrix InvWorldMatrix = XMMatrixInverse(nullptr, WorldMatrix);

	pPicking->Transform_ToLocalSpace(InvWorldMatrix);

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		_float3 pPickPoint;

		if (true == pPicking->Intersect_InLocalSpace(m_pVerticesPos[m_pIndices[i]._0], m_pVerticesPos[m_pIndices[i]._1], m_pVerticesPos[m_pIndices[i]._2], &pPickPoint))
		{
			XMStoreFloat3(&pPickPoint, XMVector3TransformCoord(XMLoadFloat3(&pPickPoint), WorldMatrix));

			vPickPoint->push_back(pPickPoint);
		}
	}

	RELEASE_INSTANCE(CPicking);

	if (!vPickPoint->empty())
		return true;
	else
		return false;
}

HRESULT CMeshContainer::SetUp_Bones(CModel * pModel)
{
	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = m_pAIMesh->mBones[i];

		CHierarchyNode* pBoneNode = pModel->Get_BonePtr(pAIBone->mName.data);
		if (nullptr == pBoneNode)
			return E_FAIL;

		m_Bones.push_back(pBoneNode);

		Safe_AddRef(pBoneNode);

		_float4x4 OffsetMatrix;

		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		pBoneNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_BonePtr(m_szName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

HRESULT CMeshContainer::SetUp_Bones_Binary(HANDLE hFile, _ulong * pdwByte, CModel * pModel)
{
	if (nullptr == hFile)
		return E_FAIL;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		char BoneName[MAX_PATH] = "";
		ReadFile(hFile, BoneName, sizeof(char) * MAX_PATH, pdwByte, nullptr);

		CHierarchyNode* pBoneNode = pModel->Get_BonePtr(BoneName);
		if (nullptr == pBoneNode)
			return E_FAIL;

		m_Bones.push_back(pBoneNode);

		Safe_AddRef(pBoneNode);

		_float4x4 OffsetMatrix;
		ReadFile(hFile, &OffsetMatrix, sizeof(_float4x4), pdwByte, nullptr);		

		pBoneNode->Set_OffsetMatrix(XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
	}

	if (0 == m_iNumBones)
	{
		CHierarchyNode* pNode = pModel->Get_BonePtr(m_szName);
		if (nullptr == pNode)
			return S_OK;

		m_iNumBones = 1;

		m_Bones.push_back(pNode);
		Safe_AddRef(pNode);
	}

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_NonAnimModel(const aiMesh * pAIMesh, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	m_pNonAnimVertices = new VTXMODEL[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		
		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));

		m_pNonAnimVertices[i] = pVertices[i];
		m_pVerticesPos[i] = pVertices[i].vPosition;
	}

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_NonAnim(HANDLE hFile, _ulong* pdwByte, _fmatrix PivotMatrix)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), pdwByte, nullptr);
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	m_pNonAnimVertices = new VTXMODEL[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];
	
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		ReadFile(hFile, &pVertices[i].vPosition, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vNormal, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vTexUV, sizeof(_float2), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vTangent, sizeof(_float3), pdwByte, nullptr);

		m_pNonAnimVertices[i] = pVertices[i];
		m_pVerticesPos[i] = pVertices[i].vPosition;
	}

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_Anim(HANDLE hFile, _ulong* pdwByte, class CModel* pModel)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXANIMMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), pdwByte, nullptr);
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		ReadFile(hFile, &pVertices[i], sizeof(VTXANIMMODEL), pdwByte, nullptr);

		m_pAnimVertices[i] = pVertices[i];
		m_pVerticesPos[i] = pVertices[i].vPosition;
	}

	ReadFile(hFile, &m_iNumBones, sizeof(_uint), pdwByte, nullptr);

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMeshContainer::Create_VertexBuffer_AnimModel(const aiMesh * pAIMesh, class CModel* pModel)
{
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXANIMMODEL);
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iNumVertexBuffers = 1;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	m_pAnimVertices = new VTXANIMMODEL[m_iNumVertices];
	m_pVerticesPos = new _float3[m_iNumVertices];

	VTXANIMMODEL* pVertices = new VTXANIMMODEL[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		memcpy(&pVertices[i].vPosition, &m_pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &m_pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexUV, &m_pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &m_pAIMesh->mTangents[i], sizeof(_float3));

		m_pAnimVertices[i] = pVertices[i];
		m_pVerticesPos[i] = pVertices[i].vPosition;
	}

	m_iNumBones = pAIMesh->mNumBones;

	for (_uint i = 0; i < m_iNumBones; ++i)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		for (_uint j = 0; j < pAIBone->mNumWeights; ++j)
		{
			_uint iVertexIndex = pAIBone->mWeights[j].mVertexId;

			if (0 == pVertices[iVertexIndex].vBlendWeight.x)
			{
				pVertices[iVertexIndex].vBlendWeight.x = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.x = i;				  
			}															  
			else if (0 == pVertices[iVertexIndex].vBlendWeight.y)		  
			{															  
				pVertices[iVertexIndex].vBlendWeight.y = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.y = i;				   
			}															   
			else if (0 == pVertices[iVertexIndex].vBlendWeight.z)		   
			{															   
				pVertices[iVertexIndex].vBlendWeight.z = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.z = i;
			}
			else if (0 == pVertices[iVertexIndex].vBlendWeight.w)
			{
				pVertices[iVertexIndex].vBlendWeight.w = pAIBone->mWeights[j].mWeight;
				pVertices[iVertexIndex].vBlendIndex.w = i;
			}

			m_pAnimVertices[i].vBlendWeight = pVertices[iVertexIndex].vBlendWeight;
			m_pAnimVertices[i].vBlendIndex = pVertices[iVertexIndex].vBlendIndex;
		}
	}

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, const aiMesh * pAIMesh, class CModel* pModel, _fmatrix PivotMatrix)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshContainer"));
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong* pdwByte, CModel* pModel, _fmatrix PivotMatrix)
{
	CMeshContainer* pInstance = new CMeshContainer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype_Binary(eModelType, hFile, pdwByte, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshContainer::Clone(void * pArg)
{
	CMeshContainer* pInstance = new CMeshContainer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshContainer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshContainer::Free(void)
{
	__super::Free();

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pNonAnimVertices);
		Safe_Delete_Array(m_pAnimVertices);
		Safe_Delete_Array(m_pIndices);
	}

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);
	
	m_Bones.clear();
}
