#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "UIBase.h"
#include "GameInstance.h"
#include "Player.h"
#include <fstream>
#include <iostream>
#include <string>

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
	:iPortrait_Arrived_Count(0), m_iGald(1000) , m_iCP(255) , m_iMAXCP(300) , m_bStopTick(false)
{									
	m_bMainMenuon = false;
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

void CUI_Manager::Erase_Itempopup_list(CUI_Base * point)
{

	for (auto iter = m_pItempopup.begin(); iter != m_pItempopup.end();)
	{
		if ((*iter) == point)
			iter = m_pItempopup.erase(iter);
		else
			++iter;
	}

}



//void CUI_Manager::Read_TextFiles_for_dialogue()
//{
//	std::ifstream file("../../../Bin/test.txt");
//	if (file.is_open())
//	{
//		while (file.getline(fuck, 256))
//		{
//			_tchar* pszDialog = new _tchar[MAX_PATH];
//			testvec.push_back(pszDialog);
//			ConverCtoWC(fuck);
//			memcpy(pszDialog, m_szFPS, sizeof(_tchar)*MAX_PATH);
//		}
//		file.close();
//	}
//	else
//	{
//		std::cout << "Unable to open file\n";
//	}
//
//
//}

//wchar_t * CUI_Manager::ConverCtoWC(char * str)
//{
//
//	//wchar_t형 변수 선언
//	wchar_t* pStr;
//	//멀티 바이트 크기 계산 길이 반환
//	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);
//	//wchar_t 메모리 할당
//	pStr = new WCHAR[MAX_PATH];
//	//형 변환
//	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, m_szFPS, MAX_PATH);
//	return pStr;
//
//}




void CUI_Manager::Free()
{

	/*for (auto& iter : m_vInventory)
		Safe_Release(iter);*/
	for (int i = 0; i<m_vInventory.size(); i++)
	{

		ITEMINFO* temp = m_vInventory[i];
      	delete temp;
	}

	/*if (m_vInventory_useable.size() > 0)
	{
		for (int i = 0; i<m_vInventory_useable.size(); i++)
		{

			ITEMINFO* temp = m_vInventory_useable[i];
			delete temp;
		}

	}

	if (m_vInventory_material.size() > 0)
	{
		for (int i = 0; i<m_vInventory_material.size(); i++)
		{

			ITEMINFO* temp = m_vInventory_material[i];
			delete temp;
		}
	}
	
	if (m_vInventory_weapon.size() > 0)
	{
		for (int i = 0; i < m_vInventory_weapon.size(); i++)
		{

			ITEMINFO* temp = m_vInventory_weapon[i];
			delete temp;
		}
	}

	
	if (m_vInventory_armor.size() > 0)
	{
		for (int i = 0; i < m_vInventory_armor.size(); i++)
		{

			ITEMINFO* temp = m_vInventory_armor[i];
			delete temp;
		}
	}*/
}