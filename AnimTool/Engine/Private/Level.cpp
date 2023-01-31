#include "..\Public\Level.h"

CLevel::CLevel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize(void)
{
	return S_OK;
}

void CLevel::Tick(_float fTimeDelta)
{
}

void CLevel::Late_Tick(_float fTimeDelta)
{
}

void CLevel::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
