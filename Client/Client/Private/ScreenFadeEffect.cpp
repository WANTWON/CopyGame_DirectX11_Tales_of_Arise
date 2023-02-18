#include "stdafx.h"

#include "ScreenFadeEffect.h"
#include "GameInstance.h"

CScreenFadeEffect::CScreenFadeEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CScreenFadeEffect::CScreenFadeEffect(const CScreenFadeEffect & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CScreenFadeEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScreenFadeEffect::Initialize(void * pArg)
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

int CScreenFadeEffect::Tick(_float fTimeDelta)
{
	if (m_fTimer > m_fDuration)
	{
		m_fTimer = 0.f;

		return OBJ_DEAD;
	}
	else
		m_fTimer += fTimeDelta;

	return OBJ_NOEVENT;
}

void CScreenFadeEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);	

	if (m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_UI_GLOW, this);
}

HRESULT CScreenFadeEffect::Render()
{
	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BATTLESTART);
	m_pVIBufferCom->Render();
	
	return S_OK;
}

HRESULT CScreenFadeEffect::Render_Glow()
{
	if (!m_pShaderCom || !m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GlowTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	/* Sprite Variables */
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSprite", &m_vSprite, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fSpriteTimer", &m_fTimer, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fSpriteDuration", &m_fDuration, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_SPRITE_GLOW);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CScreenFadeEffect::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Battlestart"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CScreenFadeEffect::SetUp_ShaderResources()
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
	if (FAILED(m_pShaderCom->Set_RawValue("g_vSprite", &m_vSprite, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fSpriteTimer", &m_fTimer, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fSpriteDuration", &m_fDuration, sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CScreenFadeEffect * CScreenFadeEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CScreenFadeEffect* pInstance = new CScreenFadeEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CScreenFadeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CScreenFadeEffect::Clone(void * pArg)
{
	CScreenFadeEffect* pInstance = new CScreenFadeEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CScreenFadeEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScreenFadeEffect::Free()
{
	__super::Free();
}