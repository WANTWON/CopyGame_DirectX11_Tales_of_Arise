#include "stdafx.h"
#include "..\Public\UI_Damagefont_Break.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h"
#include "UI_Dialoguepopup.h"



CUI_Damagefont_Break::CUI_Damagefont_Break(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Damagefont_Break::CUI_Damagefont_Break(const CUI_Damagefont_Break & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Damagefont_Break::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Damagefont_Break::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_pPointer = (CBaseObj*)pArg;


	m_fSize.x = 200.f;
	m_fSize.y = 20.f;
	m_fPosition = m_pPointer->Get_ProjPosition();
	/*m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;*/
	//m_fFontPos.x = 500.f;
	m_fAlpha = 1;



	/*m_fStartYScale = -300.f;

	m_fLineXOffset = -600.f;*/

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/*m_iQuestindex = CUI_Manager::Get_Instance()->Get_QuestIndex();

	if (!m_bQuestStart)
	{
	CGameInstance::Get_Instance()->PlaySounds(TEXT("QuestStart.wav"), SOUND_EFFECT, 1.0f);
	m_bQuestStart = true;
	}*/

	return S_OK;
}

int CUI_Damagefont_Break::Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon() || CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
		return OBJ_NOEVENT;
	if (m_pPointer == nullptr)
		return OBJ_DEAD;
	m_fPosition = m_pPointer->Get_ProjPosition();

	//	if (m_btick)
	//	{
	if (m_fLevelupAlphaup)
	{
		m_fLevelupAlpha += 0.05f;
		if (m_fLevelupAlpha >= 1.f)
		{
			//m_fAlphaScreen = 1.f;
			m_fLevelupAlphaup = false;

		}
	}





	if (m_bLinemove)
	{
		m_fLineYScaler -= 0.4f;

		if (m_fLineYScaler <= 1.f)
		{
			m_fLineYScaler = 1.f;
			//	m_fLineXOffset = 0.f;
			//	m_fLightEffectXOffset = 0.f;   //linemove

			m_bLinemove = false;
		}
	}

	if (m_bLinexup)
	{
		m_fLineXScaler += 0.2f;
		if (m_fLineXScaler >= 2.f)
		{
			m_fLineXScaler = 2.f;
			m_bLinexup = false;
			m_bLinexdown = true;
		}

	}

	if (m_bLinexdown)
	{
		m_fLineXScaler -= 0.1f;
		m_fLineAlpha -= 0.06f;
		if (m_fLineAlpha <= 0.f)
		{
			m_bLinexdown = false;
			m_bDeadtimeron = true;
		}

	}




	if (m_bDeadtimeron)
	{
		m_fDietimer += fTimeDelta;
		if (m_fDietimer > 0.5f)
		{
			m_fLevelupAlpha -= 0.03f;
			if (m_fLevelupAlpha <= 0.f)
				return OBJ_DEAD;
		}

	}











	return OBJ_NOEVENT;
}

void CUI_Damagefont_Break::Late_Tick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_Mainmenuon() || CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
		return ;
	
	if (m_pPointer == nullptr)
		return;


	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_FRONT, this);


}

HRESULT CUI_Damagefont_Break::Render()
{
	//	if (!m_btick)
	//		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fLevelupAlpha, sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 200.f;
	m_fSize.y = 20.f;
	/*m_fPosition.x = 165.f;
	m_fPosition.y = 276.f + m_fIndex*50.f;;*/
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	//m_fAlpha = 1.f;


	/*m_fSize.x = 512.f;
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

	m_pVIBufferCom->Render();*/

	m_fSize.x = 200.f *m_fLineXScaler;
	m_fSize.y = 5.f * m_fLineYScaler;
	m_fPosition.x = m_fPosition.x - 25.f;
	m_fPosition.y = m_fPosition.y - 1.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//_float linealpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fLineAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);


	m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_Damagefont_Break::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_break"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dialoguebox"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;





	return S_OK;
}

HRESULT CUI_Damagefont_Break::SetUp_ShaderResources()
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




void CUI_Damagefont_Break::Open_sysmsg(_uint index)//, _uint index1)
{

	//	m_btick = true;

	//	msgtype = index;
	m_bfadein = true;
	//	m_bInterect = true;
	m_fDietimer = 0.f;

}





CUI_Damagefont_Break * CUI_Damagefont_Break::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Damagefont_Break*	pInstance = new CUI_Damagefont_Break(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Damagefont_Break"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_Damagefont_Break::Clone(void * pArg)
{
	CUI_Damagefont_Break*	pInstance = new CUI_Damagefont_Break(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Damagefont_Break"));
		Safe_Release(pInstance);
	}

	//CUI_Manager::Get_Instance()->Set_SystemMsg(pInstance);

	return pInstance;
}

void CUI_Damagefont_Break::Free()
{

	Safe_Release(m_pTextureCom1);

	__super::Free();
}


