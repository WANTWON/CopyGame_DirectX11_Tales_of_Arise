#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "UIBase.h"
#include "GameInstance.h"
#include "Player.h"


IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

void CUI_Manager::Initialize_PlayerState()
{
	
	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Inventory()
{

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Map()
{
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_PlayerState()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	/* HP Update */
	
	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Coin()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	/* Coin Update */

	RELEASE_INSTANCE(CGameInstance);
}


void CUI_Manager::Tick_UI(_float fTimeDelta)
{
	Tick_PlayerState();
	Tick_Coin();
}

void CUI_Manager::Set_UI_OpenType(UITYPE eUIType)
{
	if (m_eUIType == eUIType)
	{
		m_eUIType = UI_END;
	}
	else
	{
		m_eUIType = eUIType;
	}
	
}



void CUI_Manager::Free()
{
}
