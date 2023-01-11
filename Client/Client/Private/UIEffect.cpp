#include "stdafx.h"
#include "..\Public\UIEffect.h"
#include "GameInstance.h"


CUIEffect::CUIEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CObj_UI(pDevice, pContext)
{
}

CUIEffect::CUIEffect(const CUIEffect & rhs)
	: CObj_UI(rhs)
{
}

HRESULT CUIEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUIEffect::Initialize(void * pArg)
{

	if (pArg != nullptr)
		memcpy(&m_EffectDesc, pArg, sizeof(EFFECTDESC));

	
	switch (m_EffectDesc.eEffectID)
	{
	case COMPOSS_RING:
		m_fSize.x = m_EffectDesc.vInitScale.x;
		m_fSize.y = m_EffectDesc.vInitScale.y;
		m_eShaderID = SHADER_TWOCOLOR_DEFAULT;
		m_vColorFront = XMVectorSet(255, 255, 255, 255);
		m_vColorBack = XMVectorSet(200, 200, 255, 255);
		break;
	}

	m_fPosition.x = m_EffectDesc.vInitPositon.x;
	m_fPosition.y = m_EffectDesc.vInitPositon.y;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

int CUIEffect::Tick(_float fTimeDelta)
{
	if (__super::Tick(fTimeDelta))
		return OBJ_DEAD;


	switch (m_EffectDesc.eEffectID)
	{
	case COMPOSS_RING:
		Tick_CompossRing(fTimeDelta);
		break;
	}

	return OBJ_NOEVENT;
}

void CUIEffect::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CUIEffect::Render()
{

	__super::Render();

	return S_OK;
}


HRESULT CUIEffect::Ready_Components(void * pArg)
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Components(TEXT("Com_Shader"), LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	switch (m_EffectDesc.eEffectID)
	{
	case COMPOSS_RING:
		/* For.Com_Texture */
		if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Ripple"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	default:
		break;
	}

		

	return S_OK;
}

HRESULT CUIEffect::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorBack", &m_vColorBack, sizeof(_vector))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_ColorFront", &m_vColorFront, sizeof(_vector))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_EffectDesc.iTextureNum))))
		return E_FAIL;

	return S_OK;

}

HRESULT CUIEffect::SetUp_ShaderID()
{
	

	return S_OK;
}

void CUIEffect::Tick_CompossRing(_float fTimeDelta)
{
	m_fDeadtime += fTimeDelta;

	if (m_fDeadtime < m_EffectDesc.fDeadTime)
	{
		m_fSize.x += 3.f;
		m_fSize.y += 3.f;
		m_fAlpha += 0.05f;

		if (m_fAlpha >= 1.f)
			m_fAlpha = 1.f;
	}
	else
	{
		m_fSize.x += 0.5f;
		m_fSize.y += 0.5f;
		m_fAlpha -= 0.02f;

	}

	if (m_fSize.x <= 0 || m_fAlpha <= 0)
		m_bDead = true;
}


CUIEffect * CUIEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUIEffect*	pInstance = new CUIEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CInvenTile"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUIEffect::Clone(void * pArg)
{
	CUIEffect*	pInstance = new CUIEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUIEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUIEffect::Free()
{
	__super::Free();
}
