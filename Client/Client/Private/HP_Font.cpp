#include "stdafx.h"
#include "..\Public\HP_Font.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "Player.h"
#include "PlayerManager.h"
#include "UI_Get_item_Popup.h"
#include "BattleManager.h"
CHP_Font::CHP_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CHP_Font::CHP_Font(const CHP_Font & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CHP_Font::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CHP_Font::Initialize(void * pArg)
{
	if (pArg != nullptr)
		m_iIndex = *(_uint*)pArg;

	if (m_iIndex == CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1)
		m_bforMainPlayer = true;

	m_eShaderID = UI_BRIGHT;
	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fAlpha = 0.f;


	CUI_RuneEffect::RUNEDESC desc;
	desc.position.x = 1130.f;
	m_fPosition.x = 1180.f;
	if (m_iIndex == 0)
	{
//		m_fPosition.x = 1180.f + (m_iIndex * 14);
		
		
		m_fnumberY = 	desc.position.y  = m_fPosition.y = 325.f;
		
		
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;

		desc.position.x = 1060.f;
		desc.position.y = m_fnumberY - 34.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;

	}
	else if (m_iIndex == 1)
	{
		
		m_fnumberY = desc.position.y  = m_fPosition.y = 385.f;
		
		
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;
		desc.position.x = 1060.f;
		desc.position.y = m_fnumberY - 34.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;
	}
	else if (m_iIndex == 2)
	{

		m_fnumberY = desc.position.y = m_fPosition.y = 445.f;

		
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;
		desc.position.x = 1060.f;
		desc.position.y = m_fnumberY - 34.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
			return E_FAIL;

	}
  else if (m_iIndex == 3)
{

	m_fnumberY = desc.position.y = m_fPosition.y = 505.f;


	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
		return E_FAIL;
	desc.position.x = 1060.f;
	desc.position.y = m_fnumberY - 34.f;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
		return E_FAIL;

}

	
	



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

//	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	return S_OK;
}

int CHP_Font::Tick(_float fTimeDelta)
{
	//CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return OBJ_NOEVENT;
	for (_uint i = 0; i < 4; ++i)
		m_fbrightpos_hp[i] += 0.015f;

	for (_uint i = 0; i < 4; ++i)
	{
		if (m_fbrightpos_hp[i] >= 3.f)
			m_fbrightpos_hp[i] = 0.f;
	}
	
	switch (m_iIndex)
	{
	case 0:
		//if(m_bforMainPlayer)
		m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentHp;
		m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();//alpen0  , sion 1
			break;

	case 1:
		if (m_bforMainPlayer)
		{
			m_iCurrenthp =(_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();//alpen0  , sion 1
		}
			
		else
		{
			m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[0]->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[0]->Get_PlayerID();
		}
		
		
			
			break;

	case 2:
		if (m_bforMainPlayer)
		{
			m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();//alpen0  , sion 1
		}

		else
		{
			m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[1]->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[1]->Get_PlayerID();
		}
		/*else
		{
			m_bRenderoff = true;
			return OBJ_NOEVENT;
		}*/
		break;

	case 3:
		if (m_bforMainPlayer)
		{
			m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_PlayerID();//alpen0  , sion 1
		}

		else
		{
			m_iCurrenthp = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[2]->Get_Info().fCurrentHp;
			m_iCharactername = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers()[2]->Get_PlayerID();
		}
	/*	else
		{
			m_bRenderoff = true;
			return OBJ_NOEVENT;
		}*/
		break;
	}


	
	m_fStart_timer += fTimeDelta;



	if (m_fStart_timer > 0.5f || m_iIndex == 3)
	{
		if (m_bfadein)
			m_fAlpha += 0.01f;


		//if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
		//{
		//	CUI_RuneEffect::RUNEDESC desc;
		//	desc.position.x = 1130.f;
		//	desc.position.y = 375.f;
		////	desc.m_etype = 1;
		//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
		//		return E_FAIL;
		//	desc.position.y = 435.f;
		//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
		//		return E_FAIL;
		//	desc.position.y = 495.f;
		//	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
		//		return E_FAIL;
		//}

		/*if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
		{
			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 1130.f;
			desc.position.y = 375.f;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return E_FAIL;
			desc.position.y = 435.f;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return E_FAIL;
			desc.position.y = 495.f;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return E_FAIL;
		}*/

		
		if (CGameInstance::Get_Instance()->Key_Up(DIK_L))
		{
			
			
			/*testdesc.iIndex = 0;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"),&testdesc)))
				return E_FAIL;
			testdesc.iIndex = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
				return E_FAIL;
			testdesc.iIndex = 2;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
				return E_FAIL;
			testdesc.iIndex = 3;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, TEXT("TETE"), &testdesc)))
				return E_FAIL;*/

		}
			

		if (CGameInstance::Get_Instance()->Key_Pressing(DIK_K))
			--m_iCurrenthp;

		if (CGameInstance::Get_Instance()->Key_Pressing(DIK_J))
			++m_iCurrenthp;

		if (!m_bforMainPlayer)
		{
			m_fSize.x = 16.f;
			m_fSize.y = 20.f;
			m_fPosition.x = 1180.f;
			m_fPosition.y = m_fnumberY;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

			


		}
		else
		{
			/*CGameObject* pGameObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
			CTransform*	pPlayerTransform = (CTransform*)CGameInstance::Get_Instance()->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform"));*/
			Compute_CamDistance(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));//Get_State(CTransform::STATE_TRANSLATION));
			m_fPosition.x = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().x + 44.f;
			m_fPosition.y = CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_ProjPosition().y + 31.f - (m_fCamDistance / 5.f);

			if (m_fCamDistance > 20.f)
			{
				m_fNext = 14.f / m_fCamDistance * 20;
				m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f / m_fCamDistance * 18.f);
				m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f / m_fCamDistance * 18.f);
				m_fPosition.x += m_fCamDistance *0.1f - 10.f;//(m_fNext*3);
			}
			else
			{
				m_fNext = 14.f;
				m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, 16.f);
				m_pTransformCom->Set_Scale(CTransform::STATE_UP, 16.f);
			}

		}
		
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	}
		
		return OBJ_NOEVENT;
}
	

void CHP_Font::Late_Tick(_float fTimeDelta)
{
	if (m_bRenderoff)
		return;
	if (CUI_Manager::Get_Instance()->Get_StopTick() || CBattleManager::Get_Instance()->Get_IsStrike())
		return;

	if (m_fStart_timer > 0.8f)
	{
		if (m_fAlpha >= 1.f)
		{
			m_fAlpha = 1.f;
			m_bfadein = false;
		}

		if (nullptr != m_pRendererCom)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);
	}

	

}

HRESULT CHP_Font::Render()
{
	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_itexnum = m_iCurrenthp / 1000;

	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	if (m_iCurrenthp >= 1000)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	if (m_iCurrenthp >= 100)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[1], sizeof(_float))))
			return E_FAIL;
		m_itexnum = ((m_iCurrenthp % 1000) / 100);
		if (m_bforMainPlayer)
			m_fPosition.x += m_fNext;
		else
		m_fPosition.x = 1194.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}

	if (m_iCurrenthp >= 10)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[2], sizeof(_float))))
			return E_FAIL;
		m_itexnum = ((m_iCurrenthp % 100) / 10);

	if (m_bforMainPlayer)
			m_fPosition.x += m_fNext;
		else
			m_fPosition.x = 1208.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}


	if (m_iCurrenthp >= 1)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hp[3], sizeof(_float))))
			return E_FAIL;
		m_itexnum = m_iCurrenthp % 10;

		if (m_bforMainPlayer)
			m_fPosition.x += m_fNext;
		else
			m_fPosition.x = 1222.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();


	}

	if (!m_bforMainPlayer)
	{
		m_fPosition.x = 1060.f;
		m_fPosition.y = m_fnumberY - 34.f;
		m_fSize.x = 38.f;
		m_fSize.y = 26.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iCharactername))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
	

	return S_OK;
}

HRESULT CHP_Font::Ready_Components(void * pArg)
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

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHP_Font::SetUp_ShaderResources()
{
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ViewMatrix", &m_ViewMatrix, sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_ProjMatrix", &m_ProjMatrix, sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(m_itexnum))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CHP_Font * CHP_Font::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CHP_Font*	pInstance = new CHP_Font(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CHP_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CHP_Font::Clone(void * pArg)
{
	CHP_Font*	pInstance = new CHP_Font(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CHP_Font"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_Font::Free()
{

	Safe_Release(m_pTextureCom1);
	__super::Free();
}
