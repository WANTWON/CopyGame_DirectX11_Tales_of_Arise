#include "stdafx.h"
#include "..\Public\UI_AlphaScreen.h"

#include "GameInstance.h"

CUI_AlphaScreen::CUI_AlphaScreen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_AlphaScreen::CUI_AlphaScreen(const CUI_AlphaScreen & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_AlphaScreen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_AlphaScreen::Initialize(void * pArg)
{
	m_eShaderID = UI_BATTLESTART;

	m_fSize.x = g_iWinSizeX;
	m_fSize.y = g_iWinSizeY;
	m_fPosition.x = g_iWinSizeX >> 1;
	m_fPosition.y = g_iWinSizeY >> 1;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_AlphaScreen::Tick(_float fTimeDelta)
{
	/*if (m_fTimer > m_fDuration)
	{
		m_fTimer = 0.f;

		return OBJ_DEAD;
	}
	else
		m_fTimer += fTimeDelta;*/

	return OBJ_NOEVENT;
}

void CUI_AlphaScreen::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI_BACK, this);
}

HRESULT CUI_AlphaScreen::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);
	m_pVIBufferCom->Render();

	return S_OK;
}



HRESULT CUI_AlphaScreen::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_AlphaScreen"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_AlphaScreen::SetUp_ShaderResources()
{
	if (!m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	/* Sprite Variables */


	return S_OK;
}

CUI_AlphaScreen * CUI_AlphaScreen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_AlphaScreen* pInstance = new CUI_AlphaScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_AlphaScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CUI_AlphaScreen::Clone(void * pArg)
{
	CUI_AlphaScreen* pInstance = new CUI_AlphaScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_AlphaScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_AlphaScreen::Free()
{
	__super::Free();
}