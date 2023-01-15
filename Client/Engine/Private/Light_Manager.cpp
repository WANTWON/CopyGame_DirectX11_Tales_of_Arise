#include "..\Public\Light_Manager.h"
#include "Light.h"

IMPLEMENT_SINGLETON(CLight_Manager)

CLight_Manager::CLight_Manager()
{
}

const LIGHTDESC * CLight_Manager::Get_LightDesc(_uint iIndex)
{
	if (iIndex >= m_Lights.size())
		return nullptr;

	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightDesc();	
}

void CLight_Manager::Set_LightDesc(_uint iIndex, LIGHTDESC * pLightDesc)
{
	if (iIndex >= m_Lights.size())
		return;

	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	(*iter)->Set_LightDesc(pLightDesc);
}

_float4x4 CLight_Manager::Get_ShadowLightView()
{
	_float4x4 mMatrix;

	XMStoreFloat4x4(&mMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_matShadowLightView)));

	return mMatrix;
}

HRESULT CLight_Manager::Set_ShadowLightView(_float4 vEye, _float4 vAt)
{
	_vector vLightEye = XMLoadFloat4(&vEye);
	_vector vLightAt = XMLoadFloat4(&vAt);
	_vector	vLightUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMStoreFloat4x4(&m_matShadowLightView, XMMatrixLookAtLH(vLightEye, vLightAt, vLightUp));

	return S_OK;
}


HRESULT CLight_Manager::Add_Light(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const LIGHTDESC & LightDesc)
{
	CLight*		pLight = CLight::Create(pDevice, pContext, LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}

HRESULT CLight_Manager::Render_Lights(CShader * pShader, CVIBuffer_Rect * pVIBuffer)
{
	for (auto& pLight : m_Lights)
	{
		if (nullptr != pLight)
			pLight->Render(pShader, pVIBuffer);
	}

	return S_OK;
}


void CLight_Manager::Clear_AllLight()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}

void CLight_Manager::Clear_Light(_uint iIndex)
{
	if (iIndex >= m_Lights.size())
		return;

	auto	iter = m_Lights.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	Safe_Release(*iter);

	iter = m_Lights.erase(iter);
}



void CLight_Manager::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}
