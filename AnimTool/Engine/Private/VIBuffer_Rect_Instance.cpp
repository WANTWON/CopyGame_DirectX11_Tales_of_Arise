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
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXTEX* pVertices = new VTXTEX[m_iNumVertices];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexture = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexture = _float2(1.f, 0.f);
	
	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexture = _float2(1.f, 1.f);
	
	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexture = _float2(0.f, 1.f);

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;
	
	Safe_Delete_Array(pVertices);
#pragma endregion
#pragma region INDICES
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_iIndicesByte = sizeof(FACEINDICES16);
	m_iNumPrimitive = 2 * m_iNumInstance;
	m_iNumIndicesPerPrimitive = 3;

	m_BufferDesc.ByteWidth = m_iIndicesByte * m_iNumPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = sizeof(_ushort);

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitive];

	_uint iNumFaces = 0;

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

	ZeroMemory(&m_SubResourcesData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourcesData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;
	
	Safe_Delete_Array(pIndices);
#pragma endregion
#pragma region INSTANCE
	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	//m_iInstanceStride = sizeof(VTXMATRIX);
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect_Instance::Initialize(void * pArg)
{
	return E_NOTIMPL;
}

void CVIBuffer_Rect_Instance::Tick(_float fTimeDelta)
{
}

CVIBuffer_Rect_Instance * CVIBuffer_Rect_Instance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint iNumInstance)
{
	return nullptr;
}

CComponent * CVIBuffer_Rect_Instance::Clone(void * pArg)
{
	return nullptr;
}

void CVIBuffer_Rect_Instance::Free(void)
{
}
