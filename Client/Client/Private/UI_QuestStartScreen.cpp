#include "stdafx.h"
#include "..\Public\UI_QuestStartScreen.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h"



CUI_QuestStartScreen::CUI_QuestStartScreen(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_QuestStartScreen::CUI_QuestStartScreen(const CUI_QuestStartScreen & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_QuestStartScreen::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_QuestStartScreen::Initialize(void * pArg)
{



	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fFontPos.x = 500.f;
	m_fAlpha = 1;



	m_fStartYScale = -300.f;

	m_fLineXOffset = -600.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_QuestStartScreen::Tick(_float fTimeDelta)
{
//	if (m_btick)
//	{
	if (m_bAlphascreenon)
	{
		m_fAlphaScreen += 0.02f;
		if (m_fAlphaScreen >= 0.4f)
		{
			m_fAlphaScreen = 0.4f;
			m_bAlphascreenon = false;

		}
	}
	


	if (m_bLinemove)
	{
		m_fLineXOffset += 30.f;

		if (m_fLineXOffset >= 0.f)
		{
			m_fLineXOffset = 0.f;
			//	m_fLineXOffset = 0.f;
			//	m_fLightEffectXOffset = 0.f;   //linemove

			m_bLinemove = false;
			m_bStastYScalemove = true;
			m_bNameYScalemove = true;
		}
	}


	if (m_bStastYScalemove)
	{
		m_fStartYScale += 15.f;
		m_fAlphaMiddle += 0.05f;
		if (m_fStartYScale >= 0.f)
		{
			m_fAlphaMiddle = 1.f;
			m_fStartYScale = 0.f;
			m_bStastYScalemove = false;
			m_bAlpharuneup = true;
			m_bLightEffectmove = true;
		}

	}

	if (m_bAlpharuneup)
	{
		m_fAlpharune += 0.015f;
		if (m_fAlpharune >= 0.3f)
		{
			m_fAlpharune = 0.3f;
			m_bAlpharuneup = false;
		}
			


	}


	if (m_bLightEffectmove)
	{
		m_fLightEffectXOffset += 10.f;
		if (m_bLightreverse)
			m_fAlphaLight -= 0.005f;
		else
		m_fAlphaLight += 0.02f;
		if (m_fAlphaLight >= 0.8f)
		{
			//m_fAlphaLight = 0.8f;
			m_bLightreverse = true;
			
		}
		if (m_fLightEffectXOffset >= 0.f)
		{
			//m_fLightEffectXOffset = 0.f;
			m_bDeadtimeron = true;
			//m_bLightEffectmove = false;
		}

	}
	
	

	if (m_bDeadtimeron)
	{
		m_fDietimer += fTimeDelta;
		m_fAlpha1 += 0.02f;
 }



	if (m_fDietimer > 1.5f)
	{
		m_fAlphaMiddle -= 0.05f;

		m_fAlphaLight -= 0.04f;

		m_fAlphaScreen -= 0.02f;

		m_fAlpharune -= 0.015f;

		m_fAlpha -= 0.05f;
	}

	if (m_fDietimer > 2.f)
	{
		return OBJ_DEAD;
	}
	
	//		
	//	
	//}

	/*if (m_bNameYScalemove)
	{
		m_fNameYScale -= 5.f;
		if (m_fStartYScale <= 0.f)
		{
			m_fNameYScale = 0.f;
			m_bNameYScalemove = false;
		}

	}*/






	return OBJ_NOEVENT;
}

void CUI_QuestStartScreen::Late_Tick(_float fTimeDelta)
{

//	if (m_btick)
//	{
	/*	if (m_fAlpha >= 1.f && m_bfadein)
		{
			m_fAlpha = 1.f;
			m_bfadein = false;
			m_bfirstarrived = true;
		}

		if (m_fAlpha <= 0.f && m_bfadeout)
		{
			m_fAlpha = 0.f;
			m_bfadeout = false;
			m_fAlpha1 = 0;
			m_fFadeY = -20.f;
			m_fFade1Y = -20.f;
			m_btick = false;
		}*/

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
//	}



}

HRESULT CUI_QuestStartScreen::Render()
{
//	if (!m_btick)
//		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (m_bDeadtimeron)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fAlpha1, sizeof(_float))))
			return E_FAIL;
	}
//	m_fAlpha = 1.f;
	//_float alpha = 0.4f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaScreen, sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 1280.f;
	m_fSize.y = 720.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	
	m_pShaderCom->Begin(UI_SCREEN);

	m_pVIBufferCom->Render();

	//m_fAlpha = 1.f;
	

	m_fSize.x = 512.f;
	m_fSize.y = 512.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpharune, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(5))))
		return E_FAIL;
	if (m_bDeadtimeron)
	{
		m_pShaderCom->Begin(UI_BRIGHTBLACK);
	}
	else
	m_pShaderCom->Begin(UI_ALPHATESTSET);

	m_pVIBufferCom->Render();

	m_fSize.x = 1320.f;
	m_fSize.y = 5.f;
	m_fPosition.x = 650.f + m_fLineXOffset;
	m_fPosition.y = 377.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//_float linealpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;
	if (m_bDeadtimeron)
	{
		m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);
	}
	else
	m_pShaderCom->Begin(UI_DIALOGUELINE);

	m_pVIBufferCom->Render();

	
	m_fSize.x = 480.f;
	m_fSize.y = 64.f;
	m_fPosition.x = 730.f - m_fStartYScale;
	m_fPosition.y = 345.f;// +m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaMiddle, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (m_bDeadtimeron)
	{
		m_pShaderCom->Begin(UI_BRIGHT);
	}
	else
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();

	m_fSize.x = 300.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 675.f + m_fStartYScale;;
	m_fPosition.y = 405.f;// +m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaMiddle, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(0))))
		return E_FAIL;
	if (m_bDeadtimeron)
	{
		m_pShaderCom->Begin(UI_BRIGHT);
	}
	else
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();


	m_fSize.x = 750.f;
	m_fSize.y = 170.f;
	m_fPosition.x = 670.f + m_fLightEffectXOffset;
	m_fPosition.y = 330.f;// +m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaLight, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(12))))
		return E_FAIL;
	
	m_pShaderCom->Begin(UI_ALPHATESTSET);

	m_pVIBufferCom->Render();

	//m_fSize.x = 750.f;
	//m_fSize.y = 170.f;
	//m_fPosition.x = 670.f + m_fLightEffectXOffset;
	//m_fPosition.y = 330.f;// +m_fFadeY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	////m_fAlphaLight = 0.8f;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlphaLight, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(12))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_ALPHATESTSET);

	//m_pVIBufferCom->Render();
	/////////////////////////////////////////////////////////////////screen
	//_float alpha = m_fAlpha * 0.1f;
	//m_fAlpharune = 0.3f;

	
	
	
	/*m_fSize.x = 28.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 800.f + m_fFadeX;
	m_fPosition.y = 500.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(5))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();


	m_fSize.x = 256.f;
	m_fSize.y = 26.f;
	m_fPosition.x = 950.f + m_fFadeX;
	m_fPosition.y = 500.f + m_fFadeY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(msgtype))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();
*/






	return S_OK;
}

HRESULT CUI_QuestStartScreen::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUBUTTON"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_SYSTEM_MSG"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUEST"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fade"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTNAME"), (CComponent**)&m_pTextureCom5)))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_QuestStartScreen::SetUp_ShaderResources()
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

	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom1->Get_SRV(0))))
	return E_FAIL;*/

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fFlowCurrent, sizeof(_float))))
	return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fFlowMAX, sizeof(_float))))
	return E_FAIL;*/


	return S_OK;
}




void CUI_QuestStartScreen::Open_sysmsg(_uint index)//, _uint index1)
{

//	m_btick = true;

	msgtype = index;
	m_bfadein = true;
//	m_bInterect = true;
	m_fDietimer = 0.f;

}





CUI_QuestStartScreen * CUI_QuestStartScreen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_QuestStartScreen*	pInstance = new CUI_QuestStartScreen(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_QuestStartScreen"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_QuestStartScreen::Clone(void * pArg)
{
	CUI_QuestStartScreen*	pInstance = new CUI_QuestStartScreen(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_QuestStartScreen"));
		Safe_Release(pInstance);
	}

	//CUI_Manager::Get_Instance()->Set_SystemMsg(pInstance);

	return pInstance;
}

void CUI_QuestStartScreen::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTextureCom5);
	__super::Free();
}


