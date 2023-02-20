#include "stdafx.h"
#include "UI_font_Damage_number.h"

#include "GameInstance.h"

CUI_font_Damage_number::CUI_font_Damage_number(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_font_Damage_number::CUI_font_Damage_number(const CUI_font_Damage_number & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_font_Damage_number::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_font_Damage_number::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;
	m_eShaderID = UI_BRIGHT;
	m_fSize.x = 28.f;
	m_fSize.y = 32.f;
	m_fPosition.x = 1030.f;
	m_fPosition.y = 165.f;
	

	m_fAlpha = 0;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_font_Damage_number::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return OBJ_NOEVENT;
	m_fDietimer += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));;
	if (m_fDietimer > 5.f)
		m_bfadeout = true;


	if (m_bfadein)
	{
		m_fOffsetX += 10.f;
		m_fAlpha += 0.05f;

		if (m_fOffsetX >= 160.f)
		{
			m_fAlpha = 1.f;
			m_fOffsetX = 160.f;
			m_bfadein = false;
		}

	}




	if (m_bfadeout)
	{
		m_fOffsetX -= 10.f;
		m_fAlpha -= 0.05f;
		if (m_fOffsetX <= 0.f)
		{
			m_fAlpha = 0.f;
			m_fOffsetX = 160.f;
			m_bfadeout = false;
			m_iCurrentDamage = 0;
		}
	}


	

	

	for (_uint i = 0; i < 6; ++i)
		m_fbrightpos_damagefont[i] += 0.015f;

	for (_uint i = 0; i < 6; ++i)
	{
		if (m_fbrightpos_damagefont[i] >= 3.f)
			m_fbrightpos_damagefont[i] = 0.f;
	}

	
	if (m_bsizedown)
		sizedown();

	
	


	if (m_fSize.x <= 28.f || m_fSize.y <= 32.f)
		m_bsizedown = false;
	/*m_fSize.x = 28.f;
	m_fSize.y = 32.f;*/
	m_fPosition.x = 1290.f - m_fOffsetX;
	m_fPosition.y = 165.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (m_iCurrentDamage == 0)
		m_itexnum = 0;


	return OBJ_NOEVENT;
}

void CUI_font_Damage_number::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_StopTick())
		return ;
	
	if (m_bRender == false)
		return;

	
	

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_font_Damage_number::Render()
{
	/*if (m_bRender == false)
		return S_OK;*/

	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	//m_fPosition.x += 20.f;
	if (m_iCurrentDamage >= 100000)
	{
		m_itexnum = (m_iCurrentDamage/ 100000);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x = 1210.f - m_fOffsetX;
	if (m_iCurrentDamage >= 10000)
	{
		m_itexnum = ((m_iCurrentDamage % 100000) / 10000);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x = 1230.f - m_fOffsetX;
	if(m_iCurrentDamage >= 1000)
	{
		m_itexnum = ((m_iCurrentDamage % 10000) / 1000);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x = 1250.f - m_fOffsetX;
	if (m_iCurrentDamage >= 100)
	{
		m_itexnum = ((m_iCurrentDamage % 1000) / 100);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[3], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x = 1270.f - m_fOffsetX;
	if (m_iCurrentDamage >= 10)
	{
		m_itexnum = ((m_iCurrentDamage % 100) / 10);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[4], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x = 1290.f - m_fOffsetX;
	if (m_iCurrentDamage > 0)
	{
		m_itexnum = (m_iCurrentDamage % 10);

		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_damagefont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_font_Damage_number::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_font_Damage_number::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;*/


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	//if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fBright, sizeof(_float))))
	//	return E_FAIL;


	return S_OK;
}

CUI_font_Damage_number * CUI_font_Damage_number::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_font_Damage_number*	pInstance = new CUI_font_Damage_number(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_font_Damage_number"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_font_Damage_number::Clone(void * pArg)
{
	CUI_font_Damage_number*	pInstance = new CUI_font_Damage_number(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_font_Damage_number"));
		Safe_Release(pInstance);
	}


	CUI_Manager::Get_Instance()->Set_Damagenumber(pInstance);

	return pInstance;
}

void CUI_font_Damage_number::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}

void CUI_font_Damage_number::updatedamage(_uint damage)
{

	m_iCurrentDamage += damage;
	m_fDietimer = 0.f;
	if (!m_bfadeout)
	{
		m_fSize.x = 36.f;
		m_fSize.y = 40.f;
		m_bsizedown = true;
	//	m_fDietimer = 0.f;
	}
	else
	{
	//	m_fDietimer = 0.f;
		m_bfadein = true;
		m_bfadeout = false;
		m_fOffsetX = 0.f;
	}



}
