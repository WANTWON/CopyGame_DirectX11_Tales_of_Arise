#include "stdafx.h"
#include "..\Public\HPbar.h"

#include "GameInstance.h"

CHPbar::CHPbar(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CHPbar::CHPbar(const CHPbar & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CHPbar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHPbar::Initialize(void * pArg)
{

	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;
		//memcpy(&m_ScreenEffectdesc, pArg, sizeof(SCREENFADE_DESC));
	m_fSize.x = 200.0f;
	m_fSize.y = 22.0f;
	m_fPosition.x = 1130.f; 
	m_fPosition.y = 340 + (m_iIndex * 60);


	m_eShaderID = UI_HPBAR;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CHPbar::Tick(_float fTimeDelta)
{
	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
		--m_fcurrenthp;

	if (CGameInstance::Get_Instance()->Key_Up(DIK_J))
		++m_fcurrenthp;

	return OBJ_NOEVENT;
}

void CHPbar::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CHPbar::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderID()))
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();*/

	return S_OK;

	
}

HRESULT CHPbar::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;
	
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHPbar::SetUp_ShaderResources()
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

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrenthp, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fmaxhp, sizeof(_float))))
		return E_FAIL;



	return S_OK;
}

CHPbar * CHPbar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHPbar*	pInstance = new CHPbar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHPbar::Clone(void * pArg)
{
	CHPbar*	pInstance = new CHPbar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHPbar"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHPbar::Free()
{
	__super::Free();
}
