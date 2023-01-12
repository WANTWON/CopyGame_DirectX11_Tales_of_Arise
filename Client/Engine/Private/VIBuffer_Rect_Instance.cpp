#include "..\Public\VIBuffer_Rect_Instance.h"


CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer_Instance(pDevice, pContext)
{
}

CVIBuffer_Rect_Instance::CVIBuffer_Rect_Instance(const CVIBuffer_Rect_Instance & rhs)
	: CVIBuffer_Instance(rhs)
	, m_pSpeedPerSec(rhs.m_pSpeedPerSec)
{
}

HRESULT CVIBuffer_Rect_Instance::Initialize_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype(iNumInstance)))
		return E_FAIL;

#pragma region VERTICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iStride = sizeof(VTXTEX);
	m_iNumVertices = 4;
	m_iNumVertexBuffers = 2;
	m_eFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXTEX*			pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion


#pragma region Indices
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2 * m_iNumInstance;
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	FACEINDICES16*			pIndices = new FACEINDICES16[m_iNumPrimitive];

	_uint		iNumFaces = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 1;
		pIndices[iNumFaces]._2 = 2;
		++iNumFaces;

		pIndices[iNumFaces]._0 = 0;
		pIndices[iNumFaces]._1 = 2;
		pIndices[iNumFaces]._2 = 3;
		++iNumFaces;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion


#pragma region INSTANCE_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iInstanceStride = sizeof(VTXMATRIX);
	m_iNumVertices = m_iNumInstance;
	m_iNumIndicesPerInstance = 6;

	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼를 생성한다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;

	VTXMATRIX*			pInstanceVtx = new VTXMATRIX[m_iNumVertices];
	m_pSpeedPerSec = new _float[m_iNumInstance];
	ZeroMemory(m_pSpeedPerSec, sizeof(_float) * m_iNumInstance);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pInstanceVtx[i].vRight = _float4(0.5f, 0.f, 0.f, 0.f);
		pInstanceVtx[i].vUp = _float4(0.f, 0.5f, 0.f, 0.f);
		pInstanceVtx[i].vLook = _float4(0.f, 0.f, 0.5f, 0.f);
		pInstanceVtx[i].vPosition = _float4(rand() % 7 - 3, 0.0f, rand() % 7 - 3, 1.f);

		m_pSpeedPerSec[i] = rand() % 5 + 1;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pInstanceVtx;

	/* 정점을 담기 위한 공간을 할당하고, 내가 전달해준 배열의 값들을 멤카피한다. */
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pInstanceBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pInstanceVtx);
#pragma endregion

	return S_OK;

}

HRESULT CVIBuffer_Rect_Instance::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Rect_Instance::Update(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;
	ZeroMemory(&MappedSubResource, sizeof MappedSubResource);

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		((VTXMATRIX*)MappedSubResource.pData)[i].vPosition.y += m_pSpeedPerSec[i] * fTimeDelta;

		if (5.0f < ((VTXMATRIX*)MappedSubResource.pData)[i].vPosition.y)
			((VTXMATRIX*)MappedSubResource.pData)[i].vPosition.y = 0.0f;
	}

	

	m_pContext->Unmap(m_pInstanceBuffer, 0);
}



CVIBuffer_Rect_Instance * CVIBuffer_Rect_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	CVIBuffer_Rect_Instance*	pInstance = new CVIBuffer_Rect_Instance(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
	{
		ERR_MSG(TEXT("Failed to Created : CVIBuffer_Rect_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CVIBuffer_Rect_Instance::Clone(void * pArg)
{
	CVIBuffer_Rect_Instance*	pInstance = new CVIBuffer_Rect_Instance(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CVIBuffer_Rect_Instance"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Rect_Instance::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pSpeedPerSec);


}
