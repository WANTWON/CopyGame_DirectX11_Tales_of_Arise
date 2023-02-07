#include "..\Public\MeshContainer_Instance.h"
#include "HierarchyNode.h"
#include "Frustum.h"

CMeshContainer_Instance::CMeshContainer_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CMeshContainer_Instance::CMeshContainer_Instance(const CMeshContainer_Instance & rhs)
	: CVIBuffer_Instance(rhs)
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_iNumBones(rhs.m_iNumBones)
	, m_pInstance(rhs.m_pInstance)
	, m_iOriginalInstance(rhs.m_iOriginalInstance)
{
	strcpy_s(m_szName, rhs.m_szName);
}

void CMeshContainer_Instance::Get_BoneMatrices(_float4x4 * pBoneMatrices, _fmatrix PivotMatrix)
{
	if (0 == m_iNumBones)
	{
		XMStoreFloat4x4(&pBoneMatrices[0], XMMatrixIdentity());
		return;
	}

	_uint iNumBones = 0;

	for (auto& pBoneNode : m_Bones)
	{
		/* Offset * Combined * Pivot */
		XMStoreFloat4x4(&pBoneMatrices[iNumBones++], pBoneNode->Get_OffsetMatrix() * pBoneNode->Get_CombinedTransformationMatrix() * PivotMatrix);
	}
}

HRESULT CMeshContainer_Instance::Initialize_Prototype(CModel::TYPE eModelType, HANDLE hFile, _ulong * pdwByte, const char* pDataFilePath, CModel * pModel, _fmatrix PivotMatrix)
{
	_tchar szModelFilePath[MAX_PATH] = TEXT("");
	MultiByteToWideChar(CP_ACP, 0, pDataFilePath, (_int)strlen(pDataFilePath), szModelFilePath, MAX_PATH);

	_ulong dwDataByte = 0;
	HANDLE hDataFile = CreateFile(szModelFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
	{
		_tchar szErrorMsg[MAX_PATH] = TEXT("Failed to Open File : ");
		lstrcat(szErrorMsg, szModelFilePath);
		ERR_MSG(szErrorMsg);
		return E_FAIL;
	}

	_int iNumInstance = 0;
	ReadFile(hDataFile, &iNumInstance, sizeof(_int), &dwDataByte, nullptr);

	if (FAILED(__super::Initialize_Prototype(iNumInstance)))
		return E_FAIL;

	m_iOriginalInstance = m_iNumInstance;

	/* 메시의 이름을 보관한다. */
	/* 정해진 뼈가 없는 경우에는 최초에는 항등으로 셋팅했었고 이후 메시가 배치되어야할 위치에 셋팅하기위해.
	이 메시의 이름과 같은 노드를 찾아서 저장해놓는다. + 뼈의 갯수로 강제로 1로 늘려놨었다. */
	ReadFile(hFile, m_szName, sizeof(char) * MAX_PATH, pdwByte, nullptr);

	/* 이 메시가 그려질때 맵핑되어야할 재질정보(텍스쳐)를 찾아내기위해. */
	/* 모델이 보관하고 있는 머테리얼에게서 찾아낸다. */
	ReadFile(hFile, &m_iMaterialIndex, sizeof(_uint), pdwByte, nullptr);

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXMODEL);
	ReadFile(hFile, &m_iNumVertices, sizeof(_uint), pdwByte, nullptr);
	m_iNumVertexBuffers = 2;
	m_eFormat = DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXMODEL* pVertices = new VTXMODEL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		ReadFile(hFile, &pVertices[i].vPosition, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vNormal, sizeof(_float3), pdwByte, nullptr);
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));

		ReadFile(hFile, &pVertices[i].vTexUV, sizeof(_float2), pdwByte, nullptr);
		ReadFile(hFile, &pVertices[i].vTangent, sizeof(_float3), pdwByte, nullptr);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion
#pragma region INDICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES32);
	ReadFile(hFile, &m_iNumPrimitive, sizeof(_uint), pdwByte, nullptr);
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	/* 동적 버퍼를 생성한다. */
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ulong);

	FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitive];
	FACEINDICES32* pInstanceIndices = new FACEINDICES32[m_iNumPrimitive * m_iNumInstance];

	for (_uint i = 0; i < m_iNumPrimitive; ++i)
	{
		ReadFile(hFile, &pIndices[i]._0, sizeof(_uint), pdwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._1, sizeof(_uint), pdwByte, nullptr);
		ReadFile(hFile, &pIndices[i]._2, sizeof(_uint), pdwByte, nullptr);
	}

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		for (_uint j = 0; j < m_iNumPrimitive; ++j)
			pInstanceIndices[i * m_iNumPrimitive + j] = pIndices[j];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	Safe_Delete_Array(pInstanceIndices);
#pragma endregion
#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = m_iNumPrimitive * m_iNumIndicesPerPrimitive;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	/* 동적 버퍼를 생성한다. */
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXMATRIX* pInstanceVtx = new VTXMATRIX[m_iNumVertices];
	m_pInstance = new VTXMATRIX[m_iNumVertices];


	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		NONANIMDESC  ModelDesc;
		_float3 vPos, vScale, vRotation;

		ReadFile(hDataFile, &(ModelDesc), sizeof(NONANIMDESC), &dwDataByte, nullptr);
		vPos = ModelDesc.vPosition;
		vScale = ModelDesc.vScale;
		vRotation = ModelDesc.vRotation;

		pInstanceVtx[i].vRight = *(_float4*)&ModelDesc.WorldMatrix.m[0];
		pInstanceVtx[i].vUp = *(_float4*)&ModelDesc.WorldMatrix.m[1];
		pInstanceVtx[i].vLook = *(_float4*)&ModelDesc.WorldMatrix.m[2];
		pInstanceVtx[i].vPosition = *(_float4*)&ModelDesc.WorldMatrix.m[3];

		m_pInstance[i] = pInstanceVtx[i];
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstanceVtx;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVtx);
#pragma endregion

	CloseHandle(hDataFile);

	return S_OK;
}

HRESULT CMeshContainer_Instance::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMeshContainer_Instance::SetUp_Bones(HANDLE hFile, _ulong * pdwByte, CModel * pModel)
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

void CMeshContainer_Instance::Tick(_float fRaius)
{
	CFrustum* pFrustum = GET_INSTANCE(CFrustum);

	vector<VTXMATRIX> vInstance;

	for (_uint i = 0; i < m_iOriginalInstance; ++i)
	{
		if (true == pFrustum->isIn_WorldFrustum(XMLoadFloat4(&m_pInstance[i].vPosition), fRaius))
			vInstance.push_back(m_pInstance[i]);
	}

	m_iNumInstance = (_uint)vInstance.size();

	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
		((VTXMATRIX*)(MappedSubResource.pData))[i] = vInstance[i];

	m_pContext->Unmap(m_pInstanceBuffer, 0);

	RELEASE_INSTANCE(CFrustum);
}

CMeshContainer_Instance * CMeshContainer_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, HANDLE hFile, _ulong * pdwByte, const char* pDataFilePath, CModel * pModel, _fmatrix PivotMatrix)
{
	CMeshContainer_Instance* pInstance = new CMeshContainer_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, hFile, pdwByte, pDataFilePath, pModel, PivotMatrix)))
	{
		ERR_MSG(TEXT("Failed to Created : CMeshContainer_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CMeshContainer_Instance::Clone(void * pArg)
{
	CMeshContainer_Instance* pInstance = new CMeshContainer_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CMeshContainer_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshContainer_Instance::Free(void)
{
	__super::Free();

	if (false == m_isCloned && nullptr != m_pInstance)
		Safe_Delete_Array(m_pInstance);

	for (auto& pBoneNode : m_Bones)
		Safe_Release(pBoneNode);

	m_Bones.clear();
}