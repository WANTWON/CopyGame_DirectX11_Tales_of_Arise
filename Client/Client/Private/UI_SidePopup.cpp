#include "stdafx.h"
#include "..\Public\UI_SidePopup.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
#include "UI_RuneEffect.h"



CUI_SidePopup::CUI_SidePopup(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_SidePopup::CUI_SidePopup(const CUI_SidePopup & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_SidePopup::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_SidePopup::Initialize(void * pArg)
{
	//if (pArg != nullptr)
	//	m_fGainExp = *(_float*)pArg;


	m_fSize.x = 720.f;
	m_fSize.y = 150.f;
	m_fPosition.x = 200.f;
	m_fPosition.y = 600.f;
	m_fAlpha = 0;
	m_btick = true;



	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CUI_SidePopup::Tick(_float fTimeDelta)
{

	/*_uint index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;

	if (m_fGainExp > 0.f)
	{
		m_fGainExp -= 5.f;
		m_fGainExp1 += 5.f;
		for (_uint i = 0; i < index; ++i)
		{
			CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Plus_EXP(5);
		}


	}
	else
		m_bExpFinish = true;*/

	/*if (m_bExpFinish && m_bFirst)
	{
		for (_uint i = 0; i < index; ++i)
		{
			if (CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Levelup())
			{
				m_bLevelaction[i] = true;
				m_bRuneOn[i] = true;
				m_bLevelupOn[i] = true;
				CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Set_Levelup(false);
			}
		}

		m_bExpFinish = false;
		m_bFirst = false;

	}*/



	/*if (CGameInstance::Get_Instance()->Key_Down(DIK_LCONTROL))
	{
		


		
	}*/
	
	
		if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() != LEVEL_BATTLE && CGameInstance::Get_Instance()->Get_CurrentLevelIndex() != LEVEL_LAWBATTLE &&
			CGameInstance::Get_Instance()->Get_CurrentLevelIndex() != LEVEL_BOSS)
		{
			if (CGameInstance::Get_Instance()->Key_Down(DIK_LCONTROL))
			{
				CUI_RuneEffect::RUNEDESC desc;
				desc.position.x = 52.f;
				desc.position.y = 227.f + 20.f;
				desc.m_etype = 1;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_STATIC, TEXT("test"), &desc)))
					return E_FAIL;
				desc.position.y = 227.f + 70.f;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_STATIC, TEXT("test"), &desc)))
					return E_FAIL;
				if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
				{
					desc.position.y = 227.f + 120.f;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_STATIC, TEXT("test"), &desc)))
						return E_FAIL;
				}


				if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
				{
					desc.position.y = 227.f + 170.f;
					if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_STATIC, TEXT("test"), &desc)))
						return E_FAIL;
				}
			}
			if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL))
			{
				m_bfadein = true;
				m_bfadeinname = true;




			}
			else
				m_bfadeout = true;
		}
		
	

	

	/*for (_uint i = 0; i < index; ++i)
	{
		if (m_bLevelaction[i])
		{
			m_fLevelactiontimer[i] += fTimeDelta;
			if (m_bRuneOn[i])
			{
				CUI_RuneEffect::RUNEDESC desc;
				desc.position.x = 62.f;
				desc.position.y = 277.f + i*50.f;
				desc.m_etype = 1;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
					return E_FAIL;

				m_bRuneOn[i] = false;
			}
			else if (m_fLevelactiontimer[i] > 0.7f && m_bLevelupOn[i])
			{

				_float index = (_float)i;
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Levelup"), LEVEL_STATIC, TEXT("SS"), &index)))
					return OBJ_NOEVENT;
				m_bLevelupOn[i] = false;
				m_PlayersHPAlphadown[i] = true;
				m_bLevelaction[i] = false;
				m_fLevelactiontimer[i] = 0.f;

			}


		}
	}*/



	_uint index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;


	m_fbrightpos += 0.015f;
	m_fbrightpos_hpbar += 0.015f;

	for (_uint i = 0; i < 9; ++i)
		m_fbrightpos_hpfont[i] += 0.015f;

	if (m_fbrightpos >= 3.f)
		m_fbrightpos = 0.f;

	if (m_fbrightpos_hpbar >= 5.f)
		m_fbrightpos_hpbar = 0.f;



	for (_uint i = 0; i < 9; ++i)
	{
		if (m_fbrightpos_hpfont[i] >= 3.f)
			m_fbrightpos_hpfont[i] = 0.f;
	}
	index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;
	for (_uint i = 0; i < index; ++i)
	{
		m_PlayersCurrentHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp;
		m_PlayersMaxHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp;

		m_PlayersCurrentExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iCurrentExp;
		m_PlayersMaxExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iMaxExp;

		m_PlayersLevel[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iLevel;
	}



	timer += fTimeDelta;

	/*if (timer > 4.f)
	{
	m_bfadeout = true;


	}*/

	if (m_bfadeinname)
	{
		m_fnameAlpha += 0.1f;
		if (m_fnameAlpha >= 1.f)
		{
			m_bfadeinname = false;
			m_fnameAlpha = 1.f;
		}
			

	}
		


	if (m_bfadein)
	{
		
		m_fMainAlpha += 0.1f;
		m_fAlpha += 0.1f; //»ý±æ¶§
		m_fFadeX -= 30.f;
		m_PlayersHPAlpha[0] += 0.1f;
		m_PlayersHPAlpha[1] += 0.1f;
		m_PlayersHPAlpha[2] += 0.1f;
		m_PlayersHPAlpha[3] += 0.1f;
		if (m_fFadeX <= -20.f)
		{
			m_fMainAlpha = 1.f;
			m_bfadein = false;
			m_fAlpha = 1.f;
			m_fFadeX = -20.f;

			m_PlayersHPAlpha[0] = 1.f;
			m_PlayersHPAlpha[1] = 1.f;
			m_PlayersHPAlpha[2] = 1.f;
			m_PlayersHPAlpha[3] = 1.f;
		}

		//m_bfirstarrived = true;
	}


	else if (m_bfadeout)
	{
		m_fnameAlpha -= 0.1f;
		m_fMainAlpha -= 0.1f;
		m_fAlpha -= 0.1f;
		m_fAlpha1 -= 0.1f;
		m_PlayersHPAlpha[0] -= 0.1f;
		m_PlayersHPAlpha[1] -= 0.1f;
		m_PlayersHPAlpha[2] -= 0.1f;
		m_PlayersHPAlpha[3] -= 0.1f;

		//	m_fFade1X -= 2.f;
		m_fFadeX += 30.f;

		if (m_fAlpha <= 0.f)
		{
			m_bfadeout = false;
			m_fMainAlpha = 0.f;
			m_fAlpha = 0.f;
			m_fAlpha1 = 0.f;
			m_PlayersHPAlpha[0] = 0.f;
			m_PlayersHPAlpha[1] = 0.f;
			m_PlayersHPAlpha[2] = 0.f;
			m_PlayersHPAlpha[3] = 0.f;
			m_fFadeX = 200.f;
			m_fnameAlpha = -2.f;
			//m_btick = false;
			//return OBJ_DEAD;
		}

	}

	/*if (m_PlayersHPAlphadown[0])
	{
		m_PlayersHPAlpha[0] -= 0.1f;
		if (m_PlayersHPAlpha[0] <= -5.f)
		{
			m_PlayersHPAlpha[0] = 0.f;
			m_PlayersHPAlphadown[0] = false;
			m_PlayersHPAlphaup[0] = true;
		}

	}

	if (m_PlayersHPAlphadown[1])
	{
		m_PlayersHPAlpha[1] -= 0.1f;
		if (m_PlayersHPAlpha[1] <= -5.f)
		{
			m_PlayersHPAlpha[1] = 0.f;
			m_PlayersHPAlphadown[1] = false;
			m_PlayersHPAlphaup[1] = true;
		}

	}

	if (m_PlayersHPAlphadown[2])
	{
		m_PlayersHPAlpha[2] -= 0.1f;
		if (m_PlayersHPAlpha[2] <= 0.f)
		{
			m_PlayersHPAlpha[2] = 0.f;
			m_PlayersHPAlphadown[2] = false;
		}

	}

	if (m_PlayersHPAlphadown[3])
	{
		m_PlayersHPAlpha[3] -= 0.1f;
		if (m_PlayersHPAlpha[3] <= 0.f)
		{
			m_PlayersHPAlpha[3] = 0.f;
			m_PlayersHPAlphadown[4] = false;
		}

	}






	if (m_PlayersHPAlphaup[0])
	{
		m_PlayersHPAlpha[0] += 0.1f;
		if (m_PlayersHPAlpha[0] >= 1.f)
		{
			m_PlayersHPAlpha[0] = 1.f;
			m_PlayersHPAlphaup[0] = false;
		}
	}
	if (m_PlayersHPAlphaup[1])
	{
		m_PlayersHPAlpha[1] += 0.1f;
		if (m_PlayersHPAlpha[1] >= 1.f)
		{
			m_PlayersHPAlpha[1] = 1.f;
			m_PlayersHPAlphaup[1] = false;
		}
	}
	if (m_PlayersHPAlphaup[2])
	{
		m_PlayersHPAlpha[2] += 0.1f;
		if (m_PlayersHPAlpha[2] >= 1.f)
		{
			m_PlayersHPAlpha[2] = 1.f;
			m_PlayersHPAlphaup[2] = false;
		}
	}
	if (m_PlayersHPAlphaup[3])
	{
		m_PlayersHPAlpha[3] += 0.1f;
		if (m_PlayersHPAlpha[3] >= 1.f)
		{
			m_PlayersHPAlpha[3] = 1.f;
			m_PlayersHPAlphaup[3] = false;
		}
	}*/


	/*if (m_fAlpha >= 1.f && m_bfadein)
	{
	m_fAlpha = 1.f;
	m_bfadein = false;

	}*/



	/*if (m_fAlpha <= 0.f && m_bfadeout)
	{
	m_fAlpha = 0.f;
	m_bfadeout = false;
	m_fAlpha1 = 0;
	m_btick = false;

	return OBJ_DEAD;
	}*/
	return OBJ_NOEVENT;
}






void CUI_SidePopup::Late_Tick(_float fTimeDelta)
{






	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);




}

HRESULT CUI_SidePopup::Render()
{
	if (!m_btick)
		return S_OK;


	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	_float alpha = m_fAlpha * 0.7f;

	m_fSize.x = 430.f;
	m_fSize.y = 320.f;
	m_fPosition.x = 50.f - m_fFadeX;
	m_fPosition.y = 330.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	m_fSize.y = 80.f;
	m_fPosition.y = 520.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_DIALOGUEBOX);

	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 300.f;
	m_fSize.y = 4.f;
	m_fPosition.x = 100.f - m_fFadeX;
	m_fPosition.y = 460.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	m_fPosition.y = 195.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	m_fPosition.y = 488.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	m_fPosition.y = 552.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();


	m_fSize.x = 450.f;
	m_fSize.y = 6.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 140.f;// +m_fFadeY + m_fIndexOffsetY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHTDIALOGUELINE);

	m_pVIBufferCom->Render();

	if (CUI_Manager::Get_Instance()->Get_QuestIndex() > 0)
	{
		m_fSize.x = 300.f;
		m_fSize.y = 38.f;
		m_fPosition.x = 675.f;
		m_fPosition.y = 118.f;// +m_fFadeY + m_fIndexOffsetY;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom9->Get_SRV(CUI_Manager::Get_Instance()->Get_QuestIndex() - 1))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	

	//m_fSize.x = 200;
	//m_fSize.y = 24.f;
	//m_fPosition.x = 100.f - m_fFadeX;
	//m_fPosition.y = 140.f;// +m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(0))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();

	//Render_Exp();

	//m_fSize.x = 180;
	//m_fSize.y = 18.f;
	//m_fPosition.x = 90.f - m_fFadeX;
	//m_fPosition.y = 180.f;// +m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(1))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();


	////m_fPosition.x = 90.f - m_fFadeX;
	//m_fPosition.y = 210.f;// +m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(2))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();

	//m_fSize.x = 28.f;
	//m_fSize.y = 28.f;
	//m_fPosition.x = 600.f - m_fFadeX;
	//m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();

	//m_fSize.x = 240.f;
	//m_fSize.y = 26.f;
	//m_fPosition.x = 700.f - m_fFadeX;
	//m_fPosition.y = 120.f;// + m_fFadeY + m_fIndexOffsetY;
	//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(m_iNum))))
	//	return E_FAIL;

	//m_pShaderCom->Begin(UI_BRIGHT);

	//m_pVIBufferCom->Render();



	Render_Player1();
	Render_Player2();
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
		Render_Player3();
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
		Render_Player4();

	Render_Cp();








	return S_OK;
}

HRESULT CUI_SidePopup::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skillmsg"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LIGHTEFFECT"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom5)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture6"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom6)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture7"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Battleresult"), (CComponent**)&m_pTextureCom7)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture8"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CPguage"), (CComponent**)&m_pTextureCom8)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture9"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTNAME"), (CComponent**)&m_pTextureCom9)))
		return E_FAIL;

	
	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture26"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"), (CComponent**)&m_pTextureCom26)))
	//	return E_FAIL;

	///* For.Com_Texture */
	//if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"), (CComponent**)&m_pTextureCom2)))
	//	return E_FAIL;



	return S_OK;
}

HRESULT CUI_SidePopup::SetUp_ShaderResources()
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

HRESULT CUI_SidePopup::Render_Player1()
{
	m_fSize.x = 36.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 0.f - m_fFadeX;
	m_fPosition.y = 260.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[0] - (_float)m_PlayersCurrentExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_EXPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 16.f;
	m_fPosition.x = -6.5f - m_fFadeX;
	m_fPosition.y = 261.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[0] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersLevel[0] % 10);
	if (m_PlayersLevel[0] >= 10)
		m_fPosition.x += 12.f;
	else
		m_fPosition.x += 6.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	m_fSize.x = 38.f;
	m_fSize.y = 22.f;
	m_fPosition.x = 35.f - m_fFadeX;
	m_fPosition.y = 245.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fnameAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();




	m_fSize.x = 180.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 17.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_PlayersHPAlpha[0], sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 18.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 15.f;
	m_fPosition.y += 18.f;

	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[0] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersCurrentHP[0] % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 12.f;
	m_fSize.y = 16.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_fPosition.x += 9.f;
	if ((_uint)m_PlayersMaxHP[0] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[0] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[0] >= 10.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[0] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[7], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersMaxHP[0] % 10);

	m_fPosition.x += 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[8], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SidePopup::Render_Player2()
{
	m_fSize.x = 36.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 0.f - m_fFadeX;
	m_fPosition.y = 310.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[1] - (_float)m_PlayersCurrentExp[1];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[1];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_EXPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 16.f;
	m_fPosition.x = -6.5f - m_fFadeX;
	m_fPosition.y = 311.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[1] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[1] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersLevel[1] % 10);
	if (m_PlayersLevel[1] >= 10)
		m_fPosition.x += 12.f;
	else
		m_fPosition.x += 6.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	m_fSize.x = 38.f;
	m_fSize.y = 22.f;
	m_fPosition.x = 35.f - m_fFadeX;
	m_fPosition.y = 295.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fnameAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(1))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();




	m_fSize.x = 180.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 17.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[1], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[1], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_PlayersHPAlpha[1], sizeof(_float))))
		return E_FAIL;
	m_fSize.x = 18.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 15.f;
	m_fPosition.y += 18.f;

	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[1] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[1] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[1] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[1] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[1] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[1] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersCurrentHP[1] % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 12.f;
	m_fSize.y = 16.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_fPosition.x += 9.f;
	if ((_uint)m_PlayersMaxHP[1] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[1] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[1] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[1] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[1] >= 10.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[1] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[7], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersMaxHP[1] % 10);

	m_fPosition.x += 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[8], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SidePopup::Render_Player3()
{
	m_fSize.x = 36.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 0.f - m_fFadeX;
	m_fPosition.y = 360.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[2] - (_float)m_PlayersCurrentExp[2];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[2];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_EXPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 16.f;
	m_fPosition.x = -6.5f - m_fFadeX;
	m_fPosition.y = 361.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[0] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[2] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersLevel[2] % 10);
	if (m_PlayersLevel[2] >= 10)
		m_fPosition.x += 12.f;
	else
		m_fPosition.x += 6.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	m_fSize.x = 38.f;
	m_fSize.y = 22.f;
	m_fPosition.x = 35.f - m_fFadeX;
	m_fPosition.y = 345.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fnameAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();




	m_fSize.x = 180.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 17.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_PlayersHPAlpha[2], sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 15.f;
	m_fPosition.y += 18.f;

	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[2] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[2] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[2] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[2] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[2] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[2] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersCurrentHP[2] % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 12.f;
	m_fSize.y = 16.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_fPosition.x += 9.f;
	if ((_uint)m_PlayersMaxHP[2] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[2] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[2] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[2] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[2] >= 10.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[2] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[7], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersMaxHP[2] % 10);

	m_fPosition.x += 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[8], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SidePopup::Render_Player4()
{
	m_fSize.x = 36.f;
	m_fSize.y = 36.f;
	m_fPosition.x = 0.f - m_fFadeX;
	m_fPosition.y = 410.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[3] - (_float)m_PlayersCurrentExp[3];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[3];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_EXPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 16.f;
	m_fPosition.x = -6.5f - m_fFadeX;
	m_fPosition.y = 411.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[3] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[3] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersLevel[3] % 10);
	if (m_PlayersLevel[3] >= 10)
		m_fPosition.x += 12.f;
	else
		m_fPosition.x += 6.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();



	m_fSize.x = 38.f;
	m_fSize.y = 22.f;
	m_fPosition.x = 35.f - m_fFadeX;
	m_fPosition.y = 395.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fnameAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();




	m_fSize.x = 180.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 17.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[3], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[3], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_PlayersHPAlpha[3], sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 15.f;
	m_fPosition.y += 18.f;

	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[3] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[3] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[3] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[3] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_PlayersCurrentHP[3] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersCurrentHP[3] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersCurrentHP[3] % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 16.f;
	m_fSize.y = 20.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 12.f;
	m_fSize.y = 16.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_fPosition.x += 9.f;
	if ((_uint)m_PlayersMaxHP[3] >= 1000.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[3] % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[3] >= 100.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[3] % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 9.f;
	if (m_PlayersMaxHP[3] >= 10.f)
	{
		m_ihpnum = (((_uint)m_PlayersMaxHP[3] % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[7], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_PlayersMaxHP[3] % 10);

	m_fPosition.x += 9.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[8], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SidePopup::Render_Cp()
{
	m_fPosition.x = 160.f - m_fFadeX;
	m_fPosition.y = 520.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom8->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;
	m_fmaxcp = (_float)CUI_Manager::Get_Instance()->Get_MAXCP();
	m_fcurrentcp = (_float)CUI_Manager::Get_Instance()->Get_CP();

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fcurrentcp, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_fmaxcp, sizeof(_float))))
		return E_FAIL;

	m_fSize.x = 46.f;
	m_fSize.y = 46.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);

	m_pVIBufferCom->Render();


	m_fSize.x = 41.f;
	m_fSize.y = 41.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	/*if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(1))))
	return E_FAIL;*/
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE);
	//UI_CP_GUAGE_BLACK
	m_pVIBufferCom->Render();


	m_fSize.x = 26.f;
	m_fSize.y = 30.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 130.f;
	m_fPosition.y += 10.f;

	m_fPosition.x += 16.f;
	if ((_uint)m_fcurrentcp >= 100)
	{
		m_icpnum = (((_uint)m_fcurrentcp % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 16.f;
	if ((_uint)m_fcurrentcp >= 10)
	{
		m_icpnum = (((_uint)m_fcurrentcp % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_icpnum = ((_uint)m_fcurrentcp % 10);

	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 24.f;
	m_fPosition.x += 14.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(10))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//m_fmaxcp
	m_fPosition.x += 13.f;
	if ((_uint)m_fmaxcp >= 100)
	{
		m_icpnum = (((_uint)m_fmaxcp % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 13.f;
	if ((_uint)m_fmaxcp >= 10)
	{
		m_icpnum = (((_uint)m_fmaxcp % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_fmaxcp % 10);

	m_fPosition.x += 13.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_icpnum))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_SidePopup::Render_Exp()
{
	m_fSize.x = 18.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x += 45.f;
	m_fPosition.y += 40.f;

	m_fPosition.x += 12.f;
	if (m_fGainExp1 >= 1000.f)
	{
		m_ihpnum = (((_uint)m_fGainExp1 % 10000) / 1000);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_fGainExp1 >= 100.f)
	{
		m_ihpnum = (((_uint)m_fGainExp1 % 1000) / 100);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}
	m_fPosition.x += 12.f;
	if (m_fGainExp1 >= 10)
	{
		m_ihpnum = (((_uint)m_fGainExp1 % 100) / 10);


		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();
	}


	m_ihpnum = ((_uint)m_fGainExp1 % 10);

	m_fPosition.x += 12.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}




void CUI_SidePopup::Skillmsg_on(_uint index)//, _uint index1)
{

	m_btick = true;

	m_bfadein = true;
	m_fAlpha = 0.f;
	m_fFadeX = 200.f;
	m_iNum = index;
	timer = 0.f;
}




CUI_SidePopup * CUI_SidePopup::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_SidePopup*	pInstance = new CUI_SidePopup(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_SidePopup"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



CGameObject * CUI_SidePopup::Clone(void * pArg)
{
	CUI_SidePopup*	pInstance = new CUI_SidePopup(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_SidePopup"));
		Safe_Release(pInstance);
	}

	/*CUI_Manager::Get_Instance()->Get_Itempopup_list()->push_back(pInstance);*/

	return pInstance;
}

void CUI_SidePopup::Free()
{

	//CUI_Manager::Get_Instance()->Erase_Itempopup_list(this);

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTextureCom5);
	Safe_Release(m_pTextureCom6);
	Safe_Release(m_pTextureCom7);
	Safe_Release(m_pTextureCom8);
	Safe_Release(m_pTextureCom9);

	//Safe_Release(m_pTextureCom2);
	__super::Free();
}


