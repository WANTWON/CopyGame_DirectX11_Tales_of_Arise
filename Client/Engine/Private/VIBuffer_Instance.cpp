#include "..\Public\VIBuffer_Instance.h"

CVIBuffer_Instance::CVIBuffer_Instance(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{

}

CVIBuffer_Instance::CVIBuffer_Instance(const CVIBuffer_Instance & rhs)
	: CVIBuffer(rhs)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_pInstanceBuffer(rhs.m_pInstanceBuffer)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iNumIndicesPerInstance(rhs.m_iNumIndicesPerInstance)
{
	Safe_AddRef(m_pInstanceBuffer);
}

HRESULT CVIBuffer_Instance::Initialize_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	m_iNumInstance = iNumInstance;

	return S_OK;	
}

HRESULT CVIBuffer_Instance::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Instance::Render()
{
	ID3D11Buffer*		pBuffer[] = {
		m_pVB,
		m_pInstanceBuffer
	};

	_uint				iStrides[] = {
		m_iStride,
		m_iInstanceStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffer, iStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);
	// m_pContext->IASetInputLayout();

	m_pContext->DrawIndexedInstanced(m_iNumIndicesPerInstance, m_iNumInstance, 0, 0, 0);


	return S_OK;
}

void CVIBuffer_Instance::Free()
{
	__super::Free();

	Safe_Release(m_pInstanceBuffer);

}
