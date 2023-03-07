#include "stdafx.h"
#include "..\Public\UI_Minigame1.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "Player.h"
#include "PlayerManager.h"
#include "UI_Get_item_Popup.h"
#include "BattleManager.h"
#include "Level_Restaurant.h"
CUI_Minigame1::CUI_Minigame1(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_Minigame1::CUI_Minigame1(const CUI_Minigame1 & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_Minigame1::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CUI_Minigame1::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;


	m_eShaderID = UI_ALPHASET;
	//m_fSize.x = 16.f;
	//m_fSize.y = 20.f;
	m_fAlpha = 1.f;

	m_fSize.x = 50.f;
	m_fSize.y = 75.f;
	






	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

int CUI_Minigame1::Tick(_float fTimeDelta)
{
	if (!dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Get_MiniGameStart())
		return OBJ_DEAD;


	//CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;

	if (CGameInstance::Get_Instance()->Key_Up(DIK_RETURN))
	{
		if(!m_bGameStart)
		m_bGameStartUI = true;

		CGameInstance::Get_Instance()->StopAll();
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_MiniGame1_Start.wav"), g_fSoundVolume);
	}


	m_iScore = dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Get_Score();//999;
	m_fGametime = dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Get_LimitTime();//99.f;

		//m_bGameStart = true;
	if (m_bGameStartUI)
	{
		
		m_fScaler += 0.02f;
		if (m_fScaler >= 1.f)
		{
			m_fScaler = 1.f;
			m_fStopTimer += fTimeDelta;

		}

		if (m_fStopTimer > 2.5f)
		{
			//CUI_Manager::Get_Instance()->Plus_Minigamecaptionindex(1);
			m_fStopTimer = 0.f;
			m_fScaler = 0.1f;
			m_bGameStartUI = false;
			m_bGameStart = true;
			dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Set_MiniGameUI(false);

			if (m_fGametime <= 0)
			{
				dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Set_MiniGameStart(false);
				return OBJ_DEAD;
			}
			

		}

		if (m_fGametime <= 0)
		{
			if (!m_bMiniGameSound_End)
			{
				CGameInstance::Get_Instance()->StopAll();
				CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_MiniGame1_End.wav"), g_fSoundVolume);
				m_bMiniGameSound_End = true;
			}
		}
	}


	if (m_fGametime <= 0)
		m_bGameStartUI = true;

	

	
	if (m_bGameStart)
	{
		m_fSize.x = 50.f;
		m_fSize.y = 75.f;
		m_fNext = 50.f;
		m_fPosition.x = 940.f;
		m_fPosition.y = 60.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	}
		
	



	return OBJ_NOEVENT;
}


void CUI_Minigame1::Late_Tick(_float fTimeDelta)
{
	if (!dynamic_cast<CLevel_Restaurant*>(CGameInstance::Get_Instance()->Get_CurrentLevel())->Get_MiniGameStart())
		return;

	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return;



	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_Minigame1::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	if (m_bGameStartUI)
	{
		
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
			return E_FAIL;
		m_fSize.x = 450.f * m_fScaler;
		m_fSize.y = 100.f * m_fScaler;
		m_fPosition.x = 640.f;
		m_fPosition.y = 80.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(m_iStartEndtexnum))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		
	}
	else
	{
		if (m_bGameStart)
		{
			m_itexnum = m_iScore / 1000;

			if (FAILED(SetUp_ShaderResources()))
				return E_FAIL;
			if (m_iScore >= 1000)
			{

				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();
			}

			if (m_iScore >= 100)
			{

				m_itexnum = ((m_iScore % 1000) / 100);

				m_fPosition.x += m_fNext;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();
			}

			if (m_iScore >= 10)
			{

				m_itexnum = ((m_iScore % 100) / 10);


				m_fPosition.x += m_fNext;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();
			}


			if (m_iScore >= 1)
			{

				m_itexnum = m_iScore % 10;

				m_fPosition.x += m_fNext;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
					return E_FAIL;
				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();


			}



			m_fSize.x = 125.f;
			m_fSize.y = 125.f;
			m_fNext = 80.f;
			m_fPosition.x = 500.f;
			m_fPosition.y = 60.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);


			m_fPosition.x += m_fNext;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(10))))
				return E_FAIL;
			m_pShaderCom->Begin(m_eShaderID);

			m_pVIBufferCom->Render();





			if (m_fGametime >= 10)
			{

				m_itexnum = (((_uint)m_fGametime % 100) / 10);


				m_fPosition.x += m_fNext;
				m_fSize.x = 80.f;
				m_fSize.y = 120.f;
				m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
				m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
					return E_FAIL;
				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();
			}


			if (m_fGametime >= 1)
			{

				m_itexnum = (_uint)m_fGametime % 10;

				m_fPosition.x += m_fNext;
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
					return E_FAIL;
				m_pShaderCom->Begin(m_eShaderID);

				m_pVIBufferCom->Render();


			}
		}

		else
		{
			_float alpha = m_fAlpha * 0.6f;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
				return E_FAIL;
			m_fSize.x = 600.f;
			m_fSize.y = 400.f;
			m_fPosition.x = 640.f;
			m_fPosition.y = 360.f;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_OUTLINE);

			m_pVIBufferCom->Render();

			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
				return E_FAIL;





			m_fPosition.x = 640.f;
			m_fPosition.y = 360.f;
			m_fSize.x = 500.f;
			m_fSize.y = 300.f;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));


			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(2))))
				return E_FAIL;


			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
				return E_FAIL;

			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

		}
	}

	

	

	


	return S_OK;
}

HRESULT CUI_Minigame1::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame_timenum"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame_pointnum"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITemMsgobox"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;



	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Partyjoin"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Minigame"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame1::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_itexnum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CUI_Minigame1 * CUI_Minigame1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_Minigame1*	pInstance = new CUI_Minigame1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_Minigame1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_Minigame1::Clone(void * pArg)
{
	CUI_Minigame1*	pInstance = new CUI_Minigame1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_Minigame1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame1::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	__super::Free();
}
