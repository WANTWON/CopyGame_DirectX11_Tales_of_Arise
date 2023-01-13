#include "stdafx.h"
#include "..\Public\UI_Screen.h"
#include "GameInstance.h"

CUI_Screen::CUI_Screen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Screen::CUI_Screen(const CUI_Screen & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Screen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Screen::Initialize(void * pArg)
{
	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;	

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_Screen::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return OBJ_NOEVENT;
}

void CUI_Screen::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CUI_Screen::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUI_Screen::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_BackGround"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Screen::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Screen * CUI_Screen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Screen*	pInstance = new CUI_Screen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Screen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Screen::Clone(void * pArg)
{
	CUI_Screen*	pInstance = new CUI_Screen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Screen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Screen::Free()
{
	__super::Free();
}
