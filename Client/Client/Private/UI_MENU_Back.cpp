#include "stdafx.h"
#include "..\Public\UI_MENU_Back.h"

#include "GameInstance.h"
#include "UI_RuneEffect.h"
#include "PlayerManager.h"
#include "Player.h"
#include "UI_Skillmessage.h"
#include "BattleManager.h"
#include "AIState.h"
//#include "..\Public\UI_MENU_BACK_part1.h"

CUI_MENU_Back::CUI_MENU_Back(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CUI_Base(pDevice, pContext)
{
}

CUI_MENU_Back::CUI_MENU_Back(const CUI_MENU_Back & rhs)
	: CUI_Base(rhs)
{
}

HRESULT CUI_MENU_Back::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_MENU_Back::Initialize(void * pArg)
{
	m_eShaderID = 0;
	
	m_fSize.x = 1280;
	m_fSize.y = 720.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;

	m_fAlpha = 1;

	m_bfadein = true;

	m_fMainAlpha = 0.f;
	m_fMain_Bottom_buttonY = 400.f;
	m_ficonposition4 = m_ficonposition3 = m_ficonposition2 = m_ficonposition1 = 600.f;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}                                
                          
int CUI_MENU_Back::Tick(_float fTimeDelta)
{
	
	if (!CUI_Manager::Get_Instance()->Get_Mainmenuon() && CGameInstance::Get_Instance()->Key_Up(DIK_ESCAPE))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Open.wav"), SOUND_UI, 1.f);
		CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.1f);
		fadeinMain();
	}

	if (!CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return OBJ_NOEVENT;
	
	/*_uint index = CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;
	for (_uint i = 0; i < index; ++i)
	{
		m_PlayersCurrentHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp;
		m_PlayersMaxHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp;
	}*/
	
	if (!m_brecover_finsh)
	{
		Fill_HP(m_iCursor_itemuse);   //차는기능끝
	}
	if (m_brecover_effect[m_iCursor_itemuse])
	{
		m_fRecoverFinish_HpbarEffectX += 1.f;

		if (m_fRecoverFinish_HpbarEffectX > 10.f)
		{
			m_brecover_effect[m_iCursor_itemuse] = false;
			m_fRecoverFinish_HpbarEffectX = 0.f;
		}
			

	}


	/*m_PlayersCurrentHP[0] 
		m_PlayersMaxHP[4] = */


	m_fbrightpos += 0.015f;
	m_fbrightpos_hpbar += 0.015f;
	m_fbrightpos_icon += 0.015f;

	for (_uint i = 0; i < 9; ++i)
		m_fbrightpos_hpfont[i] += 0.015f;

	if (m_fbrightpos >= 3.f)
		m_fbrightpos = 0.f;

	if (m_fbrightpos_hpbar >= 5.f)
		m_fbrightpos_hpbar = 0.f;

	if (m_fbrightpos_icon >= 5.f)
		m_fbrightpos_icon = 0.f;

	for (_uint i = 0; i < 9; ++i)
	{
		if (m_fbrightpos_hpfont[i] >= 3.f)
			m_fbrightpos_hpfont[i] = 0.f;
	}



	/*if (m_fbrightpos_hpbar >= 1.f)
		m_breverselerp = true;

	if (m_fbrightpos_hpbar <= 0.f && m_breverselerp)
		m_breverselerp = false;
	

	if (m_breverselerp)
	{
		m_fbrightlerp -= 0.1f;
	}
	else
		m_fbrightlerp += 0.1f;*/

	
	if (m_fbackfadeout)
	{
		m_fMainAlpha -= 0.05f;
		m_fbackalpha -= 0.05f;
		

		if (m_fbackalpha <= 0.f)
		{
			m_fbackalpha = 0.f;
			m_fMainAlpha = 0.f;
			m_fbackfadeout = false;
			_uint index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;
		for (_uint i = 0; i < index; ++i)
		{
			CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Set_HP(m_PlayersCurrentHP[i]);
			//CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp = m_PlayersMaxHP[i];
		}
			CUI_Manager::Get_Instance()->Set_Mainmenuon(false);
		}

	}

	/*if (CUI_Manager::Get_Instance()->Get_UI_OpenType() == CUI_Manager::UI_INVEN)
	{

	}*/

	pingpong();

	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_3))
	{
		m_fPosition.x = g_iWinSizeX - m_fSize.x * 0.5f - 45 + 160;
		m_bmoveleft = true;
		m_bfadein = true;
		m_fAlpha = 1;
	}*/

	if (m_bfadein)
		m_fAlpha -= 0.04f; //생길때
	else if (m_bfadeout)
		m_fAlpha += 0.0483f;


	//if (CGameInstance::Get_Instance()->Key_Up(DIK_4)) // 사라질때
	//{
	//	m_bfadeout = true;
	//}




	m_fSize.x = 1280;
	m_fSize.y = 720.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	return OBJ_NOEVENT;
}

void CUI_MENU_Back::Late_Tick(_float fTimeDelta)
{
	if (!CUI_Manager::Get_Instance()->Get_Mainmenuon())
		return;
	
	if (m_fbackfadein)
	{
		m_fbackalpha += 0.05f;
		if (m_fbackalpha >= 1.f)
		{
			m_fbackalpha = 1.f;
			m_fbackfadein = false;
			CUI_Manager::Get_Instance()->Set_StopTick(true);
		}
	}

	if(m_bMainAlphaup) //main and icon screenswitch moving
	{
		m_fMainAlpha += 0.05f;
		m_fMain_Bottom_buttonY -= 20.f;
		//m_fplayernameAlpha += 0.05f;

		if (m_fMainAlpha >= 1.f)
			m_bMainAlphaup = false;

	}

	if (m_biconfirstmove) //icon moving
	{
		m_ficonposition1 -= 50.f;
		m_ficonposition2 -= 25.f;
		m_ficonposition3 += 25.f;
		m_ficonposition4 += 50.f;

		if (m_ficonposition1 <= 300.f)
			m_biconfirstmove = false;
	}

	if (m_biconlastmove) //icon moving
	{
		m_ficonposition1 += 50.f;
		m_ficonposition2 += 25.f;
		m_ficonposition3 -= 25.f;
		m_ficonposition4 -= 50.f;

		if (m_ficonposition1 >= 600.f)
			m_biconlastmove = false;
	}

	m_fAngle += 0.02f;

	m_fAngle1 += 0.02f;

	if (m_fAlpha <= 0)
	{
		m_fAlpha = 0;
		m_bfadein = false;
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_F))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);

		if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3) //풀방
		{
			if (m_iLeaderindex == 3)
				m_iLeaderindex = 0;
			else
				++m_iLeaderindex;
		}
		else if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2) // 3명
		{
			if (m_iLeaderindex == 2)
				m_iLeaderindex = 0;
			else
				++m_iLeaderindex;
		}
		else
		{
			if (m_iLeaderindex == 1)
				m_iLeaderindex = 0;
			else
				++m_iLeaderindex;

		}

		switch (m_iLeaderindex)//if(m_iLeaderindex == 0)
		{
		case 0:
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayerManager::Get_Instance()->Get_EnumPlayer(0));
			break;

		case 1:
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayerManager::Get_Instance()->Get_EnumPlayer(1));
			break;

		case 2:
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayerManager::Get_Instance()->Get_EnumPlayer(2));
			break;

		case 3:
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayerManager::Get_Instance()->Get_EnumPlayer(3));
			break;
		}
		
		
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_G))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);

		if (m_etype == MENU_QUEST)
		{
			if (m_bQuestInformation)
				m_bQuestInformation = false;
			else
				m_bQuestInformation = true;
		}
	}

	if (m_busingiteminmenu && CGameInstance::Get_Instance()->Key_Up(DIK_RIGHT))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
		if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3) //풀방
		{
			if (m_iCursor_itemuse == 3)
				m_iCursor_itemuse = 0;
			else
				++m_iCursor_itemuse;
		}
		else if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2) // 3명
		{
			if (m_iCursor_itemuse == 2)
				m_iCursor_itemuse = 0;
			else
				++m_iCursor_itemuse;
		}
		else
		{
			if (m_iCursor_itemuse == 1)
				m_iCursor_itemuse = 0;
			else
				++m_iCursor_itemuse;
			
		}


		
	}

	if (m_busingiteminmenu && CGameInstance::Get_Instance()->Key_Up(DIK_LEFT))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
		if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3) //풀방
		{
			if (m_iCursor_itemuse == 0)
				m_iCursor_itemuse = 3;
			else
				--m_iCursor_itemuse;
		}
		else if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2) // 3명
		{
			if (m_iCursor_itemuse == 0)
				m_iCursor_itemuse = 2;
			else
				--m_iCursor_itemuse;
		}
		else
		{
			if (m_iCursor_itemuse == 0)
				m_iCursor_itemuse = 1;

			else
				--m_iCursor_itemuse;

		}
	

	}



	if (CGameInstance::Get_Instance()->Key_Up(DIK_RIGHT))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
		if (m_etype == MENU_MAIN && m_iCursor == 3)
			m_iCursor = 0;
		else
		++m_iCursor;

	}
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_LEFT))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
		if (m_etype == MENU_MAIN && m_iCursor == 0)
			m_iCursor = 3;
		else
		--m_iCursor;
	}



	if (CGameInstance::Get_Instance()->Key_Up(DIK_SPACE))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);
		if (m_etype == MENU_MAIN)
		{
			switch (m_iCursor)
			{
			case 0:
				m_etype = MENU_INVENTORY1;
				Enter_Main_to_Another_Menu();
				break;

			case 1:
				//m_etype = MENU_END;
				break;

			case 2:
				m_etype = MENU_QUEST;
				Enter_Main_to_Another_Menu();
				break;

			case 3:
				m_etype = MENU_BATTLEORDER;
				Enter_Main_to_Another_Menu();
				break;
			}
			
		}
		

		
	
	}

	if (m_etype == MENU_INVENTORY1 && CGameInstance::Get_Instance()->Key_Up(DIK_RETURN))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);
		if (m_busingiteminmenu == true)
		{
			use_HPrecover_item();
		}

		else
		{
			if (m_eCurrentItemtype == ITEMTYPE_JELLY || m_eCurrentItemtype == ITEMTYPE_LEAVES || m_eCurrentItemtype == ITEMTYPE_POTION)
				m_busingiteminmenu = true;
		}
		
	}
	/*if (CGameInstance::Get_Instance()->Key_Up(DIK_O))
	{
		CUI_RuneEffect::RUNEDESC desc;
		desc.position.x = 640.f;
		desc.position.y = 100.f;
		desc.m_etype = 1;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_GAMEPLAY, TEXT("test"), &desc)))
			return;
	}*/
	if (CGameInstance::Get_Instance()->Key_Up(DIK_Q))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);
		m_iCursor_inventory1 = 0;
		if (m_eInventory_type == INVEN_MAIN)
			m_eInventory_type = INVEN_ARMOR;
		else
		--m_eInventory_type;

		//m_iInventory_category_cursor = m_

		switch (m_eInventory_type)
		{
		case INVEN_MAIN:
			break;

		case INVEN_USABLE:
			update_useable_inventory();
			break;

		case INVEN_MATERIAL:
			update_material_inventory();
			break;

			
		case INVEN_WEAPON:
			update_weapon_inventory();
			break;

		case INVEN_ARMOR:
			update_armor_inventory();
			break;

		}

		
	}
	else if (CGameInstance::Get_Instance()->Key_Up(DIK_E))
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);
		m_iCursor_inventory1 = 0;

		if (m_eInventory_type == INVEN_ARMOR)
			m_eInventory_type = INVEN_MAIN;
		else
			++m_eInventory_type;

		switch (m_eInventory_type)
		{
		case INVEN_MAIN:
			break;

		case INVEN_USABLE:
			update_useable_inventory();
			break;

		case INVEN_MATERIAL:
			update_material_inventory();
			break;


		case INVEN_WEAPON:
			update_weapon_inventory();
			break;

		case INVEN_ARMOR:
			update_armor_inventory();
			break;
		}
	}


	if (CGameInstance::Get_Instance()->Key_Up(DIK_9))
	{
		//CUI_MENU_Back*	pInstance = new CUI_MENU_Back(pDevice, pContext);
		ITEMINFO*  itempointer = new  ITEMINFO;
		itempointer->eitemname = (ITEM_NAME)(rand() % 10);
		itempointer->eitemtype = ITEMTYPE_JELLY;//(ITEM_TYPE)(rand() % 20);
		itempointer->icount = 1;

		vector<ITEMINFO*>* inv = CUI_Manager::Get_Instance()->Get_Inventory();


		_bool bshouldpush = true;
		for (auto& iter = inv->begin(); iter != inv->end(); ++iter)
		{

			if ((*iter)->eitemname == itempointer->eitemname)
			{
				(*iter)->icount += 1;
				bshouldpush = false;
				break;
			}


		}

		//vector<People> *Ptr = new vector<People>(num_of_people);

		if (bshouldpush)
			inv->push_back(itempointer);
		else
			delete(itempointer);


	}


	if (m_etype == MENU_INVENTORY1)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_UP))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);

			switch (m_eInventory_type)
			{
			case INVEN_MAIN:
				if (m_iCursor_inventory1 == 0)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory()->size() - 2;
				else if (m_iCursor_inventory1 == 1)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory()->size() - 1;
				else
					m_iCursor_inventory1 -= 2;
				break;

			case INVEN_USABLE:
				if (m_iCursor_inventory1 == 0)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() - 2;
				else if (m_iCursor_inventory1 == 1)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() - 1;
				else
					m_iCursor_inventory1 -= 2;
				
				break;

			case INVEN_MATERIAL:
				if (m_iCursor_inventory1 == 0)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_material()->size() - 2;
				else if (m_iCursor_inventory1 == 1)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_material()->size() - 1;
				else
					m_iCursor_inventory1 -= 2;

				break;

			case INVEN_WEAPON:
				if (m_iCursor_inventory1 == 0)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() - 2;
				else if (m_iCursor_inventory1 == 1)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() - 1;
				else
					m_iCursor_inventory1 -= 2;
				
				break;

			case INVEN_ARMOR:
				if (m_iCursor_inventory1 == 0)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() - 2;
				else if (m_iCursor_inventory1 == 1)
					m_iCursor_inventory1 = (_uint)CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() - 1;
				else
					m_iCursor_inventory1 -= 2;
				break;
			}

		
			
		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_RIGHT) && !m_busingiteminmenu)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if (m_iCursor_inventory1 % 2 == 0)
				++m_iCursor_inventory1;
			else
				--m_iCursor_inventory1;
		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_LEFT) && !m_busingiteminmenu)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if (m_iCursor_inventory1 % 2 == 0)
				++m_iCursor_inventory1;
			else
				--m_iCursor_inventory1;
		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_DOWN) && !m_busingiteminmenu)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			switch (m_eInventory_type)
			{
			case INVEN_MAIN:
				if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory()->size() - 2)
					m_iCursor_inventory1 = 0;
				else if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory()->size() - 1)
					m_iCursor_inventory1 = 1;
				else
					m_iCursor_inventory1 += 2;
				break;                                                                       

			case INVEN_USABLE:
				if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() - 2)
					m_iCursor_inventory1 = 0;
				else if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() - 1)
					m_iCursor_inventory1 = 1;
				else
					m_iCursor_inventory1 += 2;
				break;

			case INVEN_MATERIAL:
				if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_material()->size() - 2)
					m_iCursor_inventory1 = 0;
				else if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_material()->size() - 1)
					m_iCursor_inventory1 = 1;
				else
					m_iCursor_inventory1 += 2;
				break;

			case INVEN_WEAPON:
				if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() - 2)
					m_iCursor_inventory1 = 0;
				else if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() - 1)
					m_iCursor_inventory1 = 1;
				else
					m_iCursor_inventory1 += 2;
				break;

			case INVEN_ARMOR:
				if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() - 2)
					m_iCursor_inventory1 = 0;
				else if (m_iCursor_inventory1 == CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() - 1)
					m_iCursor_inventory1 = 1;
				else
					m_iCursor_inventory1 += 2;
				break;
			}
			

		}
		
		
	}

	else if (m_etype == MENU_BATTLEORDER)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_UP))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if(m_iOrdercursor == 0)
				m_iOrdercursor = 2;
				else
				--m_iOrdercursor;
			
		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_DOWN))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if (m_iOrdercursor == 2)
				m_iOrdercursor = 0;
			else
				++m_iOrdercursor;
		}

		else if (CGameInstance::Get_Instance()->Key_Up(DIK_RETURN))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Set.wav"), SOUND_UI, 1.f);
			if (m_bAIcanuseItem)
			{
				m_bAIcanuseItem = false;
				CBattleManager::Get_Instance()->Set_AIuseItem(false);
			}
				
			else
			{
				m_bAIcanuseItem = true;
				CBattleManager::Get_Instance()->Set_AIuseItem(true);
			}

		}

	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_ESCAPE))
	{
		if (m_busingiteminmenu)
		{
			m_busingiteminmenu = false;

		}
		else if (m_etype == MENU_INVENTORY1)
		{

			m_iCursor_inventory1 = 0;
			m_bfadeout_inventory = true;
		}
		else if (m_etype == MENU_QUEST)
		{
			m_iCursor_Quest = 0;
			m_bfadeout_inventory = true;
			m_bfadeout_quest = true;

		}

		else if (m_etype == MENU_BATTLEORDER)
		{
			//m_iCursor_Quest = 0;
			m_bfadeout_inventory = true;
			//m_bfadeout_quest = true;

		}


		CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Close.wav"), SOUND_UI, 1.f);
	}

	if (m_etype == MENU_QUEST)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_UP))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if (m_iCursor_Quest == 0)
				m_iCursor_Quest = 10;
			else
				--m_iCursor_Quest;

		}
		else if (CGameInstance::Get_Instance()->Key_Up(DIK_DOWN))
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Change.wav"), SOUND_UI, 1.f);
			if (m_iCursor_Quest == 10)
				m_iCursor_Quest = 0;
			else
				++m_iCursor_Quest;

		}
			 
	}


	if (m_etype == MENU_MAIN)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_ESCAPE))
		{
			CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, 0.3f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Inventory_Close.wav"), SOUND_UI, 1.f);
			m_fbackfadeout = true;
			CUI_Manager::Get_Instance()->Set_StopTick(false);
			//m_fMain_Bottom_buttonY = 400.f;
			m_biconlastmove = true;
			
		}
			
	}
	
	if (m_fbackfadeout)
	{
		m_fMainAlpha -= 0.05f;
		m_fbackalpha -= 0.05f;
		if (m_fbackalpha <= 0.f)
		{
			m_fbackalpha = 0.f;
			m_fMainAlpha = 0.f;
			m_fbackfadeout = false;
			CUI_Manager::Get_Instance()->Set_Mainmenuon(false);
		}

	}
	if (m_bfadeout_quest)
	{
		m_fQuestAlpha -= 0.2f;
		if (m_fQuestAlpha <= 0.f)
			m_bfadeout_quest = false;
		//Return_to_MainMenu();
	}

	if (m_bfadeout_inventory)
	{
		
		/*if (m_fgoback_menutimer > 1.2f)
		{
			m_etype = MENU_MAIN;
		}*/
		m_fFadeout_itemcount -= 1.f;
		m_fFadeout_itemname -= 1.2f;
		m_fFadeout_itemtype -= 1.5f;
		m_fFadeout_iteminfo -= 3.3f;
		m_fFadeout_cursor -= 3.2f;
		m_fFadeout_inventoryback -= 15.f;

		m_fFadeout_itemnamex -= 12.8f;
		m_fFadeout_iteminfox -= 17.5f;
		m_fFadeout_inventorybackx -= 45.7f;

		m_fFadeout_line -= 52.5f;
		m_fFadeout_liney -= 0.2f;

		m_fFadeout_effect -= 30.5f;
		m_fFadeout_effecty -= 2.6f;

		m_fFadeout_categoryx -= 1.6f;
		m_fFadeout_categoryeffectx -= 4.8f;


		m_fTopAlpha += 0.03f;

		m_fScale_Mainback += 5.f;

		m_fInven_Bottom_buttonY += 8.f;

		++m_fgoback_menutimer;

		m_fInventorymiddleAlpha -= 0.05f;

		if (m_fgoback_menutimer == 20)
		{
			m_fgoback_menutimer = 0.f;
			Return_to_MainMenu();
			
		//	m_fTopAlpha = 0.4f;
		}
	
	}

	if (m_bfadein_Quest)
	{
		m_fQuestAlpha += 0.2f;
		//m_fTopAlpha -= 0.2f;

		//m_fScale_Mainback -= 20.f;
		//m_fInven_Bottom_buttonY -= 32.f;
		if (m_fQuestAlpha >= 1.f)
		{
			m_fQuestAlpha = 1.f;
			m_bfadein_Quest = false;
		//	m_fTopAlpha = 0.4f;
		}
			
	}

	if (m_bfadein_inventory)
	{
		m_fFadeout_itemcount += 1.f;
		m_fFadeout_itemname += 1.2f;
		m_fFadeout_itemtype += 1.5f;
		m_fFadeout_iteminfo += 3.3f;
		m_fFadeout_cursor += 3.2f;
		m_fFadeout_inventoryback += 15.f;
		m_fFadeout_itemnamex += 12.8f;
		m_fFadeout_iteminfox += 17.5f;
		m_fFadeout_inventorybackx += 45.7f;

		m_fFadeout_line += 52.5f;
		m_fFadeout_liney += 0.2f;

		m_fFadeout_effect += 30.5f;
		m_fFadeout_effecty += 2.6f;
		m_fFadeout_categoryx += 1.6f;
		m_fFadeout_categoryeffectx += 4.8f;


		m_fTopAlpha -= 0.03f;

		m_fScale_Mainback -= 5.f;
		m_fInven_Bottom_buttonY -= 8.f;
		m_fInventorymiddleAlpha += 0.05f;

		++m_fgoback_menutimer;

		if (m_fgoback_menutimer == 20)
		{
			m_fgoback_menutimer = 0.f;
			m_bfadein_inventory = false;
			//m_fTopAlpha = 0.f;
		}
	}

	if (nullptr != m_pRendererCom)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_BACK, this);

}

HRESULT CUI_MENU_Back::Render()
{



	if (nullptr == m_pShaderCom ||
		nullptr == m_pVIBufferCom)
		return E_FAIL;



	if (FAILED(SetUp_ShaderResources()))
		return E_FAIL;
	m_fSize.x = 1280.f + m_fScale_Mainback;
	m_fSize.y = 720.f + m_fScale_Mainback;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fbackalpha, sizeof(_float))))
		return E_FAIL;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	//back

	switch (m_iMenubackSection)
	{
	case 0:

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(0))))
			return E_FAIL;
		break;

	case 1:
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(12))))
			return E_FAIL;
		break;

	case 2:
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(13))))
			return E_FAIL;


	case 3:
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(11))))
			return E_FAIL;
		break;




	}

	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	
	if (m_etype != MENU_MAIN )//&& m_etype != MENU_QUEST)
	{
		if (m_etype != MENU_QUEST && m_etype != MENU_BATTLEORDER)
		{
			m_fSize.x = 1050.f + m_fFadeout_line;
			m_fSize.y = 4.f + m_fFadeout_liney;
			m_fPosition.x = 705.f;
			m_fPosition.y = 570.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom6->Get_SRV(1))))
				return E_FAIL;

			m_pShaderCom->Begin(UI_MENULINE);

			m_pVIBufferCom->Render();
		}
		

		m_fSize.x = 1280.f;
		m_fSize.y = 130.f;
		m_fPosition.x = 640.f;
		m_fPosition.y = 30.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha1", &m_fTopAlpha, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_INVENTOPBOTTOMALPHA);

		m_pVIBufferCom->Render();

		

		m_fSize.x = 40.f;
		m_fSize.y = 40.f;
		m_fPosition.y = 30.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (m_etype == MENU_INVENTORY1)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
				return E_FAIL;
		}
		else if (m_etype == MENU_QUEST)
		{
			m_fSize.x = 96.f;
			m_fSize.y = 96.f;
			m_fPosition.y = 40.f;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(2))))
				return E_FAIL;
		}
		else if (m_etype == MENU_BATTLEORDER)
		{

			m_fSize.x = 48.f;
			m_fSize.y = 48.f;
			m_fPosition.y = 35.f;
		   // m_fPosition.x = 630.f;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(3))))
				return E_FAIL;
		}
		

		m_pShaderCom->Begin(UI_INVENICON);

		m_pVIBufferCom->Render();
		if (m_etype == MENU_INVENTORY1)
		{
			m_fPosition.x += 85.f;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(6))))
				return E_FAIL;
		}
		else if (m_etype == MENU_QUEST)
		{
			m_fPosition.x += 85.f;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(8))))
				return E_FAIL;
		}
		else if (m_etype == MENU_BATTLEORDER)
		{
			m_fPosition.x += 100.f;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(9))))
				return E_FAIL;
		}
		
		
		m_fSize.x = 240.f;
		m_fSize.y = 24.f;
		
		m_fPosition.y = 75.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (m_etype == MENU_INVENTORY1)
		{
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
				return E_FAIL;
		}
		else if (m_etype == MENU_QUEST)
		{
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fQuestAlpha, sizeof(_float))))
				return E_FAIL;
		}
		
		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
	}

	

	//if(m_bInventory1)

	
	switch (m_etype)
	{
	case MENU_MAIN:
		Render_MAINBACK();
		if (m_fbackfadeout)
			return S_OK;
		Render_bottmline();
		Render_Mainbottombutton();
		Render_GALD();
		Render_Player1_Status_MainMenu();
		Render_Player2_Status_MainMenu();
		if(CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
		Render_Player3_Status_MainMenu();
		if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
		Render_Player4_Status_MainMenu();
		Render_Flags();
		Render_CP();
		
	
		break;

	case MENU_INVENTORY1:
		
		if (m_busingiteminmenu)
			Render_inven_bottom_item_forUseitem();
		else
		{
			Render_Inventory1();
			Render_Inven_category();
		}
			

		Render_bottmline();
		
		if (m_eCurrentItemtype == ITEMTYPE_JELLY || m_eCurrentItemtype == ITEMTYPE_LEAVES || m_eCurrentItemtype == ITEMTYPE_POTION)
			Render_Itembottombutton_useable();
		else
			Render_Itembottombutton_default();

		break;


	case MENU_QUEST:

		Render_bottmline_Quest();
		Render_Questmenu();
		break;

	case MENU_BATTLEORDER:
		Render_bottmline_Battleorder();
		Render_BattleOrder();


	}


	

	


	return S_OK;
}

HRESULT CUI_MENU_Back::Ready_Components(void * pArg)
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
	if (FAILED(__super::Add_Components(TEXT("Com_Texture"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_BACK"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Components(TEXT("Com_VIBuffer"), LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture1"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_ICON"), (CComponent**)&m_pTextureCom1)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture2"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENU_SELECT"), (CComponent**)&m_pTextureCom2)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture3"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_back"), (CComponent**)&m_pTextureCom3)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture4"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_name"), (CComponent**)&m_pTextureCom4)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture5"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom5)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture6"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENULINE"), (CComponent**)&m_pTextureCom6)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture7"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMICON"), (CComponent**)&m_pTextureCom7)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture8"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMINFO"), (CComponent**)&m_pTextureCom8)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture9"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUGALD"), (CComponent**)&m_pTextureCom9)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture10"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_HPbar"), (CComponent**)&m_pTextureCom10)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture11"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_hpgrad"), (CComponent**)&m_pTextureCom11)))
		return E_FAIL;


	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture12"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_charactername"), (CComponent**)&m_pTextureCom12)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture13"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_font"), (CComponent**)&m_pTextureCom13)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture14"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CPguage"), (CComponent**)&m_pTextureCom14)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture15"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUBUTTON"), (CComponent**)&m_pTextureCom15)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture16"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_MENUFONT"), (CComponent**)&m_pTextureCom16)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture17"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_CATEGORYNAMEFONT"), (CComponent**)&m_pTextureCom17)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture18"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_FLAG"), (CComponent**)&m_pTextureCom18)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture19"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_ITEMUSEPORTRAIT"), (CComponent**)&m_pTextureCom19)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture20"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_LIGHTEFFECT"), (CComponent**)&m_pTextureCom20)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture21"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTNAME"), (CComponent**)&m_pTextureCom21)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture22"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTMENU"), (CComponent**)&m_pTextureCom22)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture23"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTTODO"), (CComponent**)&m_pTextureCom23)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture24"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTINFO"), (CComponent**)&m_pTextureCom24)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture25"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_QUESTDIRECTION"), (CComponent**)&m_pTextureCom25)))
		return E_FAIL;
	
	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture26"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_Exp"), (CComponent**)&m_pTextureCom26)))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Components(TEXT("Com_Texture27"), LEVEL_STATIC, TEXT("Prototype_Component_Texture_battleorder"), (CComponent**)&m_pTextureCom27)))
		return E_FAIL;
	
	
	
	return S_OK;
}

HRESULT CUI_MENU_Back::SetUp_ShaderResources()
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


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle", &m_fAngle, sizeof(_float))))
		return E_FAIL;


	return S_OK;
}

CUI_MENU_Back * CUI_MENU_Back::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CUI_MENU_Back*	pInstance = new CUI_MENU_Back(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CUI_MENU_Back"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CGameObject * CUI_MENU_Back::Clone(void * pArg)
{
	CUI_MENU_Back*	pInstance = new CUI_MENU_Back(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CUI_MENU_Back"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_MENU_Back::Free()
{

	Safe_Release(m_pTextureCom1);
	Safe_Release(m_pTextureCom2);
	Safe_Release(m_pTextureCom3);
	Safe_Release(m_pTextureCom4);
	Safe_Release(m_pTextureCom5);
	Safe_Release(m_pTextureCom6);
	Safe_Release(m_pTextureCom7);
	Safe_Release(m_pTextureCom8);
	Safe_Release(m_pTextureCom9);
	Safe_Release(m_pTextureCom10);
	Safe_Release(m_pTextureCom11);
	Safe_Release(m_pTextureCom12);
	Safe_Release(m_pTextureCom13);
	Safe_Release(m_pTextureCom14);
	Safe_Release(m_pTextureCom15);
	Safe_Release(m_pTextureCom16);
	Safe_Release(m_pTextureCom17);
	Safe_Release(m_pTextureCom18);
	Safe_Release(m_pTextureCom19);
	Safe_Release(m_pTextureCom20);
	Safe_Release(m_pTextureCom21);
	Safe_Release(m_pTextureCom22);
	Safe_Release(m_pTextureCom23);
	Safe_Release(m_pTextureCom24);
	Safe_Release(m_pTextureCom25);
	Safe_Release(m_pTextureCom26);
	Safe_Release(m_pTextureCom27);

	__super::Free();
}

HRESULT CUI_MENU_Back::Render_MAINBACK()
{

	switch (m_iMenubackSection)
	{
	case 0:
		m_fSize.x = 720.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 750.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(2))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();


		m_fSize.x = 710.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 450.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
		break;

	case 1:
		m_fSize.x = 720.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 620.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(3))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		m_fSize.x = 420.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 450.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(5))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();


		m_fSize.x = 760.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 950.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(4))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();
		break;

	case 2:
		m_fSize.x = 720.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 670.f;
		m_fPosition.y = 310.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(8))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		if (m_iMenubackSection == 3)
		{
			m_fSize.x = 360.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 410.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(14))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fSize.x = 360.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 410.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(15))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();
		}

		m_fSize.x = 360.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 410.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(9))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		m_fSize.x = 720.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 960.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(10))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();


		m_fSize.x = 720.f;
		m_fSize.y = 720.f;
		m_fPosition.x = 670.f;
		m_fPosition.y = 360.f;

		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(7))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
			return E_FAIL;


		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		break;

		case 3:
			m_fSize.x = 360.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 480.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(14))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fSize.x = 360.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 820.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(15))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fSize.x = 720.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 670.f;
			m_fPosition.y = 310.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(8))))
				return E_FAIL;

			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			
				
			

			m_fSize.x = 360.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 410.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(9))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fSize.x = 720.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 960.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(10))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();


			m_fSize.x = 720.f;
			m_fSize.y = 720.f;
			m_fPosition.x = 670.f;
			m_fPosition.y = 360.f;

			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_SRV(7))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
				return E_FAIL;


			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			break;


	}

	/**/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**/





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_fSize.x = 200.f;
	m_fSize.y = 200.f;
	m_fPosition.x = m_ficonposition1;
	m_fPosition.y = 530.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	if (m_iCursor == 0 && m_etype == MENU_MAIN)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(0))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_UVROT);

		m_pVIBufferCom->Render();
	}


	m_fSize.x = 48.f;
	m_fSize.y = 48.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 1번아이콘
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_icon, sizeof(_float))))
		return E_FAIL;

    m_pShaderCom->Begin(UI_INVENICON);

	m_pVIBufferCom->Render();


	m_fPosition.x = m_ficonposition2;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_iCursor == 1 && m_etype == MENU_MAIN)
	{
		m_fSize.x = 200.f;
		m_fSize.y = 200.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 1번아이콘
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_UVROT);

		m_pVIBufferCom->Render();
	}

	m_fSize.x = 56.f;
	m_fSize.y = 48.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(1))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_icon, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENICON);

	m_pVIBufferCom->Render();


	m_fPosition.x = m_ficonposition3;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_iCursor == 2 && m_etype == MENU_MAIN)
	{
		m_fSize.x = 200.f;
		m_fSize.y = 200.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 1번아이콘
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_UVROT);

		m_pVIBufferCom->Render();
	}

	m_fSize.x = 128.f;
	m_fSize.y = 128.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_icon, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENICON);

	m_pVIBufferCom->Render();

	m_fPosition.x = m_ficonposition4;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;


	if (m_iCursor == 3 && m_etype == MENU_MAIN)
	{
		m_fSize.x = 200.f;
		m_fSize.y = 200.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 1번아이콘
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_UVROT);

		m_pVIBufferCom->Render();
	}

	m_fSize.x = 64.f;
	m_fSize.y = 64.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom1->Get_SRV(3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_icon, sizeof(_float))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENICON);

	m_pVIBufferCom->Render();






	return S_OK;
	


}

HRESULT CUI_MENU_Back::Render_Inventory1()
{


	m_fSize.x = 914.f + m_fFadeout_inventorybackx;
	m_fSize.y = 300.f + m_fFadeout_inventoryback;
	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENBACK);

	m_pVIBufferCom->Render();
/////////////////////////////////////////////////// 커서 퓽퓽

	if (m_iCursor_inventory1 % 2 == 0)
	m_fPosition.x = 180.f + m_fCursorX;
	else
	m_fPosition.x = 650.f + m_fCursorX;

	m_fSize.x = 64.f + m_fFadeout_cursor;
	m_fSize.y = 64.f + m_fFadeout_cursor;
	
	  if(m_iCursor_inventory1 == 0 || m_iCursor_inventory1 == 1)
	    m_fPosition.y = 230.f;
	else if (m_iCursor_inventory1 == 2 || m_iCursor_inventory1 == 3)
		m_fPosition.y = 260.f;
	else if (m_iCursor_inventory1 == 4 || m_iCursor_inventory1 == 5)
		m_fPosition.y = 290.f;
	else if (m_iCursor_inventory1 == 6|| m_iCursor_inventory1 == 7)
		m_fPosition.y = 320.f;
	else if (m_iCursor_inventory1 == 8 || m_iCursor_inventory1 == 9)
		m_fPosition.y = 350.f;
	else if (m_iCursor_inventory1 == 10 || m_iCursor_inventory1 == 11)
		m_fPosition.y = 380.f;
	else if (m_iCursor_inventory1 == 12|| m_iCursor_inventory1 == 13)
		m_fPosition.y = 410.f;
	else if (m_iCursor_inventory1 == 14|| m_iCursor_inventory1 == 15)
		m_fPosition.y = 440.f;
	else if (m_iCursor_inventory1 == 16|| m_iCursor_inventory1 == 17)
		m_fPosition.y = 470.f;
	else if (m_iCursor_inventory1 == 18|| m_iCursor_inventory1 == 19)
		m_fPosition.y = 500.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);

	m_pVIBufferCom->Render();
/////////////////////////////////////////////////////////////
	
////////////////////////////////////////////////////////////////////////////////////////셀렉티드 슈로릭
	if (m_iCursor_inventory1 % 2 == 0)
	{
		m_fPosition.x = 400.f;// +m_fCursorX;
	}

	else
		m_fPosition.x = 875.f;// +m_fCursorX;

	if(m_iCursor_inventory1 == 0 || m_iCursor_inventory1 ==1)
	m_fPosition.y = 225.f;

	else if (m_iCursor_inventory1 == 2 || m_iCursor_inventory1 == 3)
		m_fPosition.y = 255.f;

	else if (m_iCursor_inventory1 == 4 || m_iCursor_inventory1 == 5)
		m_fPosition.y = 285.f;

	else if (m_iCursor_inventory1 == 6 || m_iCursor_inventory1 == 7)
		m_fPosition.y = 315.f ;
	else if (m_iCursor_inventory1 == 8 || m_iCursor_inventory1 == 9)
		m_fPosition.y = 345.f;
	else if (m_iCursor_inventory1 == 10 || m_iCursor_inventory1 == 11)
		m_fPosition.y = 375.f;
	else if (m_iCursor_inventory1 == 12|| m_iCursor_inventory1 == 13)
		m_fPosition.y = 405.f;
	else if (m_iCursor_inventory1 == 14|| m_iCursor_inventory1 == 15)
		m_fPosition.y = 435.f;
	else if (m_iCursor_inventory1 == 16|| m_iCursor_inventory1 == 17)
		m_fPosition.y = 465.f;

	else if (m_iCursor_inventory1 == 18 || m_iCursor_inventory1 == 19)
		m_fPosition.y = 500.f;

	m_fSize.x = 610.f + m_fFadeout_effect;
	m_fSize.y = 52.f + m_fFadeout_effecty;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_UVROT);

	m_pVIBufferCom->Render();
////////////////////////////////////////////////////////////////////////////////////////////
	switch (m_eInventory_type)
	{
	case INVEN_MAIN:
		Render_Inven_main();
		break;

	case INVEN_USABLE:
		Render_Inven_useable();
		break;

	case INVEN_MATERIAL:
		Render_Inven_material();
		break;

	case INVEN_WEAPON:
		Render_Inven_weapon();
		break;

	case INVEN_ARMOR:
		Render_Inven_armor();
		break;


	}
	
	
	
//////////////////////////////////////////////////////////////////////////////////////

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_GALD()
{
	m_fPosition.x = 1100.f;
	m_fPosition.y = 60.f;
	m_fSize.x = 300.f;
	m_fSize.y = 40.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom9->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_GALDBACK);

	m_pVIBufferCom->Render();

	m_fPosition.x = 1040.f;
	m_fPosition.y = 65.f;
	m_fSize.x = 80.f;
	m_fSize.y = 24.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom9->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	
	m_fPosition.x = 1190.f;
	m_fSize.x = 120.f;
	m_fSize.y = 36.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // 2번아이콘
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();
	m_fPosition.x = 1150.f;
	m_fSize.x = 20.f;
	m_fSize.y = 24.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	//m_fPosition.y += 5.f;

	if (CUI_Manager::Get_Instance()->Get_Gald() >= 10000)
	{
		m_igaldnum = (CUI_Manager::Get_Instance()->Get_Gald() / 10000);

		//m_fPosition.x = 1200.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
	if (CUI_Manager::Get_Instance()->Get_Gald() >= 1000)
	{
		m_igaldnum = ((CUI_Manager::Get_Instance()->Get_Gald() % 10000) / 1000);

		m_fPosition.x += 15.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
	if (CUI_Manager::Get_Instance()->Get_Gald() >= 100)
	{
		m_igaldnum = ((CUI_Manager::Get_Instance()->Get_Gald() % 1000) / 100);

		m_fPosition.x += 15.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}
	if (CUI_Manager::Get_Instance()->Get_Gald() >= 10)
	{
		m_igaldnum = ((CUI_Manager::Get_Instance()->Get_Gald() % 100) / 10);

		m_fPosition.x += 15.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();
	}


		m_igaldnum = (CUI_Manager::Get_Instance()->Get_Gald() % 10);

		m_fPosition.x += 15.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
			return E_FAIL;
		m_pShaderCom->Begin(m_eShaderID);

		m_pVIBufferCom->Render();


	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Player1_Status_MainMenu()
{
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_fPosition.x = 78.f;
	m_fPosition.y = 124.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	_float thrower = (_float)m_PlayersMaxExp[0] -(_float)m_PlayersCurrentExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &thrower, sizeof(_float))))
		return E_FAIL;
	thrower = (_float)m_PlayersMaxExp[0];
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &thrower, sizeof(_float))))
		return E_FAIL;
	_float2 garara = { 1.f,1.f };
	if (FAILED(m_pShaderCom->Set_RawValue("g_WinXY", &garara, sizeof(_float2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_REALEXP);
	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 70.f;
	m_fPosition.y = 125.f;
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
	m_fPosition.x += 14.f;
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



	m_fSize.x = 44.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 130.f;
	m_fPosition.y = 110.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(0))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
		return E_FAIL;*/
	
	
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	
	

	m_fSize.x = 200.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
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

HRESULT CUI_MENU_Back::Render_Player2_Status_MainMenu()
{
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_fPosition.x = 78.f;
	m_fPosition.y = 179.f;
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
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_REALEXP);
	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 70.f;
	m_fPosition.y = 180.f;
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
		m_fPosition.x += 14.f;
	else
		m_fPosition.x += 8.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 44.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 130.f;
	m_fPosition.y = 165.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(1))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fSize.x = 200.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	_float fTESTMAXHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(1)->Get_Info().fMaxHp;
	_float fTESTCURRENTHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(1)->Get_Info().fCurrentHp;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &fTESTCURRENTHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &fTESTMAXHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
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

HRESULT CUI_MENU_Back::Render_Player3_Status_MainMenu()
{
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_fPosition.x = 78.f;
	m_fPosition.y = 234.f;
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
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_REALEXP);
	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 70.f;
	m_fPosition.y = 235.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (m_PlayersLevel[2] >= 10)
	{
		m_ihpnum = (((_uint)m_PlayersLevel[2] % 100) / 10);


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


	m_ihpnum = ((_uint)m_PlayersLevel[2] % 10);
	if (m_PlayersLevel[2] >= 10)
		m_fPosition.x += 14.f;
	else
		m_fPosition.x += 8.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 44.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 130.f;
	m_fPosition.y = 220.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(2))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fSize.x = 200.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	_float fTESTMAXHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Get_Info().fMaxHp;
	_float fTESTCURRENTHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Get_Info().fCurrentHp;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &fTESTCURRENTHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &fTESTMAXHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
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

HRESULT CUI_MENU_Back::Render_Player4_Status_MainMenu()
{
	m_fSize.x = 48.f;
	m_fSize.y = 48.f;
	m_fPosition.x = 78.f;
	m_fPosition.y = 289.f;
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
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom26->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_REALEXP);
	m_pVIBufferCom->Render();

	m_fSize.x = 20.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 70.f;
	m_fPosition.y = 290.f;
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
		m_fPosition.x += 14.f;
	else
		m_fPosition.x += 8.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 44.f;
	m_fSize.y = 28.f;
	m_fPosition.x = 130.f;
	m_fPosition.y = 275.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(3))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos, sizeof(_float))))
		return E_FAIL;
	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fBrightlerp", &m_fbrightlerp, sizeof(_float))))
	return E_FAIL;*/


	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fSize.x = 200.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	_float fTESTMAXHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(3)->Get_Info().fMaxHp;
	_float fTESTCURRENTHP = CPlayerManager::Get_Instance()->Get_EnumPlayer(3)->Get_Info().fCurrentHp;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &fTESTCURRENTHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &fTESTMAXHP, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpbar, sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
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

HRESULT CUI_MENU_Back::Render_Player1_Status_useitem()
{

	m_fPosition.x = 275.f;
	m_fPosition.y = 455.f;
	m_fSize.x = 64.f;
	m_fSize.y = 18.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(11))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x = 270.f;
	m_fPosition.y = 450.f;
	m_fSize.x = 80.f;
	m_fSize.y = 60.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(340.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	m_fSize.x = 48.f;
	m_fSize.y = 36.f;
	
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(340.f- g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(0))))
		return E_FAIL;
	
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	
	
	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[0], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);                                                                                       //for blackbar
	m_pVIBufferCom->Render();
	
	if (m_iCursor_itemuse == 0)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fBluepower, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BLUEHPBAR);
		m_pVIBufferCom->Render();
	}
	if(m_brecover_effect[0])
	{ 
		m_fSize.x = 168.f + m_fRecoverFinish_HpbarEffectX;
		m_fSize.y = 22.f + m_fRecoverFinish_HpbarEffectX; //f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for bluebar
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(1)))) //white
			return E_FAIL;
		m_pShaderCom->Begin();
		m_pVIBufferCom->Render();
	}
	

	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for realhpnar
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;

	if(FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[0], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 22.f;
	m_fSize.y = 26.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 50.f;
	m_fPosition.y -= 18.f;

	_float alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x += 14.f;
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
	m_fPosition.x += 14.f;
	if (m_PlayersCurrentHP[0] >= 100)
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
	m_fPosition.x += 14.f;
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

	m_fPosition.x += 14.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 14.f;
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

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_PlayersMaxHP[0] >= 1000)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[0] >= 100)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[0] >= 10)
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

	m_fPosition.x += 12.f;
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

HRESULT CUI_MENU_Back::Render_Player2_Status_useitem()
{
	m_fPosition.x = 475.f;
	m_fPosition.y = 455.f;
	m_fSize.x = 64.f;
	m_fSize.y = 18.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(11))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x = 470.f;
	m_fPosition.y = 450.f;
	m_fSize.x = 80.f;
	m_fSize.y = 60.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(540.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	

	m_fSize.x = 48.f;
	m_fSize.y = 36.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();
	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(540.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[1], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);                                                                                       //for blackbar
	m_pVIBufferCom->Render();

	if (m_iCursor_itemuse == 1)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fBluepower, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BLUEHPBAR);
		m_pVIBufferCom->Render();
	}
	if (m_brecover_effect[1])
	{
		m_fSize.x = 168.f + m_fRecoverFinish_HpbarEffectX;
		m_fSize.y = 22.f + m_fRecoverFinish_HpbarEffectX; //f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for bluebar
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(1)))) //white
			return E_FAIL;
		m_pShaderCom->Begin();
		m_pVIBufferCom->Render();
	}


	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for realhpnar
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 22.f;
	m_fSize.y = 26.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 50.f;
	m_fPosition.y -= 18.f;


	_float alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x += 14.f;
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
	m_fPosition.x += 14.f;
	if (m_PlayersCurrentHP[1] >= 100)
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
	m_fPosition.x += 14.f;
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

	m_fPosition.x += 14.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 14.f;
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

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_PlayersMaxHP[1] >= 1000)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[1] >= 100)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[1] >= 10)
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

	m_fPosition.x += 12.f;
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

HRESULT CUI_MENU_Back::Render_Player3_Status_useitem()
{
	m_fPosition.x = 675.f;
	m_fPosition.y = 455.f;
	m_fSize.x = 64.f;
	m_fSize.y = 18.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(11))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x = 670.f;
	m_fPosition.y = 450.f;
	m_fSize.x = 80.f;
	m_fSize.y = 60.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(740.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	m_fSize.x = 48.f;
	m_fSize.y = 36.f;

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(740.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[2], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);                                                                                       //for blackbar
	m_pVIBufferCom->Render();

	if (m_iCursor_itemuse == 2)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fBluepower, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BLUEHPBAR);
		m_pVIBufferCom->Render();
	}
	if (m_brecover_effect[2])
	{
		m_fSize.x = 168.f + m_fRecoverFinish_HpbarEffectX;
		m_fSize.y = 22.f + m_fRecoverFinish_HpbarEffectX; //f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for bluebar
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(1)))) //white
			return E_FAIL;
		m_pShaderCom->Begin();
		m_pVIBufferCom->Render();
	}


	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for realhpnar
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[2], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 22.f;
	m_fSize.y = 26.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 50.f;
	m_fPosition.y -= 18.f;


	_float alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x += 14.f;
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
	m_fPosition.x += 14.f;
	if (m_PlayersCurrentHP[2] >= 100)
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
	m_fPosition.x += 14.f;
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

	m_fPosition.x += 14.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 14.f;
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

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_PlayersMaxHP[2] >= 1000)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[2] >= 100)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[2] >= 10)
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

	m_fPosition.x += 12.f;
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

HRESULT CUI_MENU_Back::Render_Player4_Status_useitem()
{
	m_fPosition.x = 875.f;
	m_fPosition.y = 455.f;
	m_fSize.x = 64.f;
	m_fSize.y = 18.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(11))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();


	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fMainAlpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x = 870.f;
	m_fPosition.y = 450.f;
	m_fSize.x = 80.f;
	m_fSize.y = 60.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(940.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom20->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(27);
	m_pVIBufferCom->Render();

	m_fSize.x = 48.f;
	m_fSize.y = 36.f;

	//m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(940.f - g_iWinSizeX * 0.5f, -415.f + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom12->Get_SRV(3))))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 70.f;
	m_fPosition.y += 20.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fMaxHp", &m_PlayersMaxHP[3], sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);                                                                                       //for blackbar
	m_pVIBufferCom->Render();

	if (m_iCursor_itemuse == 3)
	{
		if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_fBluepower, sizeof(_float))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BLUEHPBAR);
		m_pVIBufferCom->Render();
	}
	if (m_brecover_effect[3])
	{
		m_fSize.x = 168.f + m_fRecoverFinish_HpbarEffectX;
		m_fSize.y = 22.f + m_fRecoverFinish_HpbarEffectX; //f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for bluebar
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(1)))) //white
			return E_FAIL;
		m_pShaderCom->Begin();
		m_pVIBufferCom->Render();
	}


	m_fSize.x = 168.f;
	m_fSize.y = 22.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4)))) // for realhpnar
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom10->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_RawValue("g_fCurrentHp", &m_PlayersCurrentHP[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_HPBAR);
	m_pVIBufferCom->Render();

	m_fSize.x = 22.f;
	m_fSize.y = 26.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_fPosition.x -= 50.f;
	m_fPosition.y -= 18.f;


	_float alpha = 1.f;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &alpha, sizeof(_float))))
		return E_FAIL;
	m_fPosition.x += 14.f;
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
	m_fPosition.x += 14.f;
	if (m_PlayersCurrentHP[3] >= 100)
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
	m_fPosition.x += 14.f;
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

	m_fPosition.x += 14.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_ihpnum))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 14.f;
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

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.x += 12.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	m_fSize.x = 18.f;
	m_fSize.y = 22.f;
	m_fPosition.y += 2.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (m_PlayersMaxHP[3] >= 1000)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[3] >= 100)
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
	m_fPosition.x += 12.f;
	if (m_PlayersMaxHP[3] >= 10)
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

	m_fPosition.x += 12.f;
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

HRESULT CUI_MENU_Back::Render_CP()
{
	m_fPosition.x = 190.f;
	m_fPosition.y = 410.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom14->Get_SRV(0))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom11->Get_SRV(0))))
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
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom14->Get_SRV(1))))
		return E_FAIL;
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

HRESULT CUI_MENU_Back::Render_Mainbottombutton()
{
	m_fSize.x = 180.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 500.f;
	m_fPosition.y = 675.f + m_fMain_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 120.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 150.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(3))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 390.f;
	m_fPosition.y = 675.f + m_fMain_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 120.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(2))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 150.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(3))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Orderbottombutton()
{
	m_fSize.x = 180.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 500.f;
	m_fPosition.y = 675.f ;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 120.f;
	m_fSize.x = 180.f;
	m_fSize.y = 26.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(10))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_fSize.x = 180.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 150.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(3))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 390.f;
	m_fPosition.y = 675.f ;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 120.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(2))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 150.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(3))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_bottmline()
{
	//////////////////////////////////////bottomline
	m_fSize.x = 1280.f;
	m_fSize.y = 130.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 715.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENTOPBOTTOM);

	m_pVIBufferCom->Render();
	//////////////////////////////////////bottomline
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Itembottombutton_useable()
{
	m_fSize.x = 180.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 500.f;
	m_fPosition.y = 675.f + m_fInven_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(4))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(5))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(3))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 390.f;
	m_fPosition.y = 675.f + m_fInven_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(3))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();



	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Itembottombutton_default()
{
	m_fSize.x = 180.f;
	m_fSize.y = 20.f;
	m_fPosition.x = 600.f;
	m_fPosition.y = 675.f + m_fInven_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(5))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom16->Get_SRV(3))))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	


	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.x = 490.f;
	m_fPosition.y = 675.f + m_fInven_Bottom_buttonY;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	m_fPosition.x += 200.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(3))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_main()
{
	_uint count = 0;
	_uint real = 0;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory()->size()  < i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 370.f;
		else
			m_fPosition.x = 837.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[i]->eitemname))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}

	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템이름
	////////////////////////////////////////////////////////////////////////////////////////////
	count = 0;
	real = 0;
	m_fSize.x = 32.f + m_fFadeout_itemtype;
	m_fSize.y = 30.f + m_fFadeout_itemtype;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory()->size()<  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 220.f;
		else
			m_fPosition.x = 687.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[i]->eitemtype))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}
	count = 0;
	real = 0;
	m_fPosition.x = 200.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemtype))))
			return E_FAIL;
		m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemtype;

	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템타입
	m_fSize.x = 20.f + m_fFadeout_itemcount;
	m_fSize.y = 20.f + m_fFadeout_itemcount;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{

		if (CUI_Manager::Get_Instance()->Get_Inventory()->size() <  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 600.f;
		else
			m_fPosition.x = 1057.f;

		m_fPosition.y = 230.f + 30.f * real;

		//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[i]->icount % 10))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}


	m_fPosition.x = 400.f;
	m_fPosition.y = 610.f;
	m_fSize.x = 350.f + m_fFadeout_iteminfox;
	m_fSize.y = 66.f + m_fFadeout_iteminfo;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_useable()
{

	_uint count = 0;
	_uint real = 0;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size()  < i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 370.f;
		else
			m_fPosition.x = 837.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[i]->eitemname))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}

	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템이름
	////////////////////////////////////////////////////////////////////////////////////////////
	count = 0;
	real = 0;
	m_fSize.x = 32.f + m_fFadeout_itemtype;
	m_fSize.y = 30.f + m_fFadeout_itemtype;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size()<  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 220.f;
		else
			m_fPosition.x = 687.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[i]->eitemtype))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}
	real = 0;
	count = 0;
	m_fPosition.x = 200.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemtype))))
			return E_FAIL;
		m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemtype;

	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템타입
	m_fSize.x = 20.f + m_fFadeout_itemcount;
	m_fSize.y = 20.f + m_fFadeout_itemcount;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{

		if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() <  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 600.f;
		else
			m_fPosition.x = 1057.f;

		m_fPosition.y = 230.f + 30.f * real;

		//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[i]->icount % 10))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}


	m_fPosition.x = 400.f;
	m_fPosition.y = 610.f;
	m_fSize.x = 350.f + m_fFadeout_iteminfox;
	m_fSize.y = 66.f + m_fFadeout_iteminfo;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_material()
{
	_uint count = 0;
	_uint real = 0;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size()  < i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 370.f;
		else
			m_fPosition.x = 837.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[i]->eitemname))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}

	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템이름
	////////////////////////////////////////////////////////////////////////////////////////////
	count = 0;
	real = 0;
	m_fSize.x = 32.f + m_fFadeout_itemtype;
	m_fSize.y = 30.f + m_fFadeout_itemtype;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size()<  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 220.f;
		else
			m_fPosition.x = 687.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[i]->eitemtype))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}
	real = 0;
	count = 0;
	m_fPosition.x = 200.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[m_iCursor_inventory1]->eitemtype))))
			return E_FAIL;
		m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[m_iCursor_inventory1]->eitemtype;

	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템타입
	m_fSize.x = 20.f + m_fFadeout_itemcount;
	m_fSize.y = 20.f + m_fFadeout_itemcount;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{

		if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size() <  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 600.f;
		else
			m_fPosition.x = 1057.f;

		m_fPosition.y = 230.f + 30.f * real;

		//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[i]->icount % 10))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}


	m_fPosition.x = 400.f;
	m_fPosition.y = 610.f;
	m_fSize.x = 350.f + m_fFadeout_iteminfox;
	m_fSize.y = 66.f + m_fFadeout_iteminfo;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_material()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_material()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_weapon()
{
	_uint count = 0;
	_uint real = 0;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size()  < i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 370.f;
		else
			m_fPosition.x = 837.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[i]->eitemname))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}

	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템이름
	////////////////////////////////////////////////////////////////////////////////////////////
	count = 0;
	real = 0;
	m_fSize.x = 32.f + m_fFadeout_itemtype;
	m_fSize.y = 30.f + m_fFadeout_itemtype;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size()<  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 220.f;
		else
			m_fPosition.x = 687.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[i]->eitemtype))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}
	real = 0;
	count = 0;
	m_fPosition.x = 200.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[m_iCursor_inventory1]->eitemtype))))
			return E_FAIL;
		m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[m_iCursor_inventory1]->eitemtype;

	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템타입
	m_fSize.x = 20.f + m_fFadeout_itemcount;
	m_fSize.y = 20.f + m_fFadeout_itemcount;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{

		if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() <  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 600.f;
		else
			m_fPosition.x = 1057.f;

		m_fPosition.y = 230.f + 30.f * real;

		//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[i]->icount % 10))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}


	m_fPosition.x = 400.f;
	m_fPosition.y = 610.f;
	m_fSize.x = 350.f + m_fFadeout_iteminfox;
	m_fSize.y = 66.f + m_fFadeout_iteminfo;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_weapon()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_weapon()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_armor()
{
	_uint count = 0;
	_uint real = 0;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size()  < i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 370.f;
		else
			m_fPosition.x = 837.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[i]->eitemname))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}

	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템이름
	////////////////////////////////////////////////////////////////////////////////////////////
	count = 0;
	real = 0;
	m_fSize.x = 32.f + m_fFadeout_itemtype;
	m_fSize.y = 30.f + m_fFadeout_itemtype;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{
		if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size()<  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 220.f;
		else
			m_fPosition.x = 687.f;

		m_fPosition.y = 230.f + 30.f * real;

		//m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[i]->eitemtype))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}
	real = 0;
	count = 0;
	m_fPosition.x = 200.f;
	m_fPosition.y = 555.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[m_iCursor_inventory1]->eitemtype))))
			return E_FAIL;
		m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[m_iCursor_inventory1]->eitemtype;

	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////- 템타입
	m_fSize.x = 20.f + m_fFadeout_itemcount;
	m_fSize.y = 20.f + m_fFadeout_itemcount;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	for (int i = 0; i < 20; ++i)
	{

		if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() <  i + 1)
			break;

		if (i % 2 == 0)
			m_fPosition.x = 600.f;
		else
			m_fPosition.x = 1057.f;

		m_fPosition.y = 230.f + 30.f * real;

		//	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		//	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[i]->icount % 10))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_ALPHASET);

		m_pVIBufferCom->Render();

		++count;
		if (count == 2)
		{
			++real;
			count = 0;
		}

	}


	m_fPosition.x = 400.f;
	m_fPosition.y = 610.f;
	m_fSize.x = 350.f + m_fFadeout_iteminfox;
	m_fSize.y = 66.f + m_fFadeout_iteminfo;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Inventory_armor()->size() > m_iCursor_inventory1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_armor()->begin()[m_iCursor_inventory1]->eitemname))))
			return E_FAIL;
	}
	m_pShaderCom->Begin(UI_ALPHASET);

	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Inven_category()
{
	m_fPosition.x = 460.f;
	m_fPosition.y = 140.f;
	m_fSize.x = 32.f + m_fFadeout_categoryx;
	m_fSize.y = 32.f + m_fFadeout_categoryx;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fInventorymiddleAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[0], sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",m_pTextureCom15->Get_SRV(2))))
		return E_FAIL;
	
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();


	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(11))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[1], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
    m_pVIBufferCom->Render();

	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[2], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(1))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[3], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(2))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[4], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(3))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[5], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x += 60.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom15->Get_SRV(5))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fBright", &m_fbrightpos_hpfont[6], sizeof(_float))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();


	switch (m_eInventory_type)
	{
	case 0:
		m_fPosition.x = 520.f;
		break;

	case 1:
		m_fPosition.x = 580.f;
		break;

	case 2:
		m_fPosition.x = 640.f;
		break;

	case 3:
		m_fPosition.x = 700.f;
		break;

	case 4:
		m_fPosition.x = 760.f;
		break;
	}
	
	m_fSize.x = 96.f + m_fFadeout_categoryeffectx;
	m_fSize.y = 96.f + m_fFadeout_categoryeffectx;;
	if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle1", &m_fAngle1, sizeof(_float))))
		return E_FAIL;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom2->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_UVROTSTRONG);

	m_pVIBufferCom->Render();

	m_fPosition.y += 43.f;

	switch (m_eInventory_type)
	{
	case 0:
		m_fPosition.x = 520.f;
		break;

	case 1:
		m_fPosition.x = 580.f;
		break;

	case 2:
		m_fPosition.x = 640.f;
		break;

	case 3:
		m_fPosition.x = 700.f;
		break;

	case 4:
		m_fPosition.x = 760.f;
		break;
	}

	m_fSize.x = 84.f;// +m_fFadeout_categoryeffectx;
	m_fSize.y = 24.f;// +m_fFadeout_categoryeffectx;;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom17->Get_SRV(m_eInventory_type))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_ALPHASET);
	m_pVIBufferCom->Render();

	return S_OK;

}

HRESULT CUI_MENU_Back::Render_Flags()
{

	m_fPosition.x = 40.f;
	switch (m_iLeaderindex)
	{
	case 0: 
		m_fPosition.y = 120.f;
		break;

	case 1:
		m_fPosition.y = 175.f;
		break;

	case 2:
		m_fPosition.y = 230.f;
		break;

	case 3:
		m_fPosition.y = 285.f;
		break;

	}
	m_fSize.x = 46.f;// +m_fFadeout_categoryeffectx;
	m_fSize.y = 46.f;// +m_fFadeout_categoryeffectx;;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom18->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_inven_bottom_item_forUseitem()
{
	//이거 셋로벨
	
	check_HPrecover_item();


	m_fPosition.x = 640.f;
	m_fPosition.y = 360.f;
	m_fSize.x = 850.f;
	m_fSize.y = 300.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(7))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x = 640.f;
	m_fPosition.y = 150.f;
	m_fSize.x = 425.f;
	m_fSize.y = 50.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(8))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x = 550.f;
	m_fPosition.y = 120.f;
	m_fSize.x = 200.f;
	m_fSize.y = 20.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(9))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	m_fPosition.x = 340.f;
	m_fPosition.y = 195.f;
	m_fSize.x = 256.f;
	m_fSize.y = 24.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(10))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();





	m_fPosition.x = 345.f;
	m_fPosition.y = 555.f;
	m_fSize.x = 256.f + m_fFadeout_itemnamex;
	m_fSize.y = 24.f + m_fFadeout_itemname;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	switch(m_eInventory_type)
	{
	case INVEN_MAIN:
		

		if (CUI_Manager::Get_Instance()->Get_Inventory()->size() > m_iCursor_inventory1)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemname))))
				return E_FAIL;
			//m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemtype;
			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fPosition.x = 615.f;
			m_fPosition.y = 150.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();

			m_fSize.x = 24.f;
			m_fSize.y = 24.f;
			m_fPosition.x = 800.f;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->icount % 10))))
				return E_FAIL;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();




			m_fSize.x = 32.f + m_fFadeout_itemtype;
			m_fSize.y = 30.f + m_fFadeout_itemtype;
			m_fPosition.x = 200.f;
			m_fPosition.y = 555.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemtype))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();

			m_fPosition.x = 470.f;
			m_fPosition.y = 150.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();


			m_fPosition.x = 400.f;
			m_fPosition.y = 610.f;
			m_fSize.x = 350.f + m_fFadeout_iteminfox;
			m_fSize.y = 66.f + m_fFadeout_iteminfo;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemname))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();


			m_eSelected_UsableItem = CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemname;
		}


		break;
	case INVEN_USABLE:
		if (CUI_Manager::Get_Instance()->Get_Inventory_useable()->size() > m_iCursor_inventory1)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom4->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemname))))
				return E_FAIL;
			//m_eCurrentItemtype = CUI_Manager::Get_Instance()->Get_Inventory()->begin()[m_iCursor_inventory1]->eitemtype;
			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_fPosition.x = 615.f;
			m_fPosition.y = 150.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();

			m_fSize.x = 24.f;
			m_fSize.y = 24.f;
			m_fPosition.x = 800.f;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom5->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->icount % 10))))
				return E_FAIL;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();

			m_fSize.x = 32.f + m_fFadeout_itemtype;
			m_fSize.y = 30.f + m_fFadeout_itemtype;
			m_fPosition.x = 200.f;
			m_fPosition.y = 555.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom7->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemtype))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();

			m_fPosition.x = 470.f;
			m_fPosition.y = 150.f;
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);
			m_pVIBufferCom->Render();


			m_fPosition.x = 400.f;
			m_fPosition.y = 610.f;
			m_fSize.x = 350.f + m_fFadeout_iteminfox;
			m_fSize.y = 66.f + m_fFadeout_iteminfo;
			m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
			m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
			m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
			if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
				m_pTextureCom8->Get_SRV(CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemname))))
				return E_FAIL;
			m_pShaderCom->Begin(UI_ALPHASET);

			m_pVIBufferCom->Render();

			m_eSelected_UsableItem = CUI_Manager::Get_Instance()->Get_Inventory_useable()->begin()[m_iCursor_inventory1]->eitemname;
			
		}
		break;



	}
	
	
	

	m_fPosition.x = 340.f;
	m_fPosition.y = 360.f;
	m_fSize.x = 172.f;
	m_fSize.y = 270.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();
	if (m_iCursor_itemuse == 0)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(6))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle", &m_fAngle, sizeof(_float))))
			return E_FAIL;

		m_pShaderCom->Begin(26);

		m_pVIBufferCom->Render();
	}

	m_fPosition.x = 540.f;
	m_fPosition.y = 360.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	if (m_iCursor_itemuse == 1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(6))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle", &m_fAngle, sizeof(_float))))
			return E_FAIL;

		m_pShaderCom->Begin(26);

		m_pVIBufferCom->Render();
	}
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
	{
		m_fPosition.x = 740.f;
		m_fPosition.y = 360.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(2))))
			return E_FAIL;
		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();

		if (m_iCursor_itemuse == 2)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(6))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle", &m_fAngle, sizeof(_float))))
				return E_FAIL;

			m_pShaderCom->Begin(26);

			m_pVIBufferCom->Render();
		}
	}
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
	{
		m_fPosition.x = 940.f;
		m_fPosition.y = 360.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(0);

		m_pVIBufferCom->Render();

		if (m_iCursor_itemuse == 3)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom19->Get_SRV(6))))
				return E_FAIL;
			if (FAILED(m_pShaderCom->Set_RawValue("g_fAngle", &m_fAngle, sizeof(_float))))
				return E_FAIL;

			m_pShaderCom->Begin(26);

			m_pVIBufferCom->Render();
		}
	}
	

	

	switch (m_iCursor_itemuse)
	{
	case 0:
		m_fPosition.x = 260.f + m_fCursorX;
			break;

	case 1:
		m_fPosition.x = 460.f + m_fCursorX;
		break;

	case 2:
		m_fPosition.x = 660.f + m_fCursorX;
		break;

	case 3:
		m_fPosition.x = 860.f + m_fCursorX;
		break;
	}

	m_fSize.x = 64.f + m_fFadeout_cursor;
	m_fSize.y = 64.f + m_fFadeout_cursor;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);

	m_pVIBufferCom->Render();
	
	Render_Player1_Status_useitem();
	Render_Player2_Status_useitem();
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
	Render_Player3_Status_useitem();
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
	Render_Player4_Status_useitem();
	return S_OK;

}

void CUI_MENU_Back::fadeinMain()
{
	m_bMainAlphaup = true;
	m_biconfirstmove = true;
	m_fbackfadein = true;
	//m_fInven_Bottom_buttonY = 0.f;
	CUI_RuneEffect::RUNEDESC desc;
	desc.position.x = 110.f;
	desc.position.y = 110.f;
	desc.m_etype = 1;
	m_fMain_Bottom_buttonY = 400.f;
	m_ficonposition4 = m_ficonposition3 = m_ficonposition2 = m_ficonposition1 = 600.f;

	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc);

	desc.position.y += 55.f;
	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc);
	m_iMenubackSection = 0;
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
	{
		desc.position.y += 55.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
			return;
		m_iMenubackSection = 1;
	}
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
	{
		desc.position.y += 55.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
			return;
		m_iMenubackSection = 2;
	}
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 4)
	{
		m_iMenubackSection = 3;
	}

	_uint index = (_uint)CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1;
	for (_uint i = 0; i < index; ++i)
	{
		m_PlayersCurrentHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp;
		m_PlayersMaxHP[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp;

		m_PlayersCurrentExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iCurrentExp;
		m_PlayersMaxExp[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iMaxExp;

		m_PlayersLevel[i] = CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().iLevel;
	}

	


}



void CUI_MENU_Back::pingpong()
{
	if (m_bright)
	{
		m_fCursorX += 0.4f;
		++m_icount;
		if (m_icount >= 17)
		{
			m_icount = 0;
			m_bright = false;
		}
	}
	else
	{
		m_fCursorX -= 0.2f;
		++m_icount;
		if (m_icount >= 34)
		{
			m_icount = 0;
			m_bright = true;
		}
	}
	
	

}

void CUI_MENU_Back::update_useable_inventory()
{
	vector<ITEMINFO*>* inv = CUI_Manager::Get_Instance()->Get_Inventory();
	vector<ITEMINFO*>* inv_useable = CUI_Manager::Get_Instance()->Get_Inventory_useable();//new

	for (auto& iter = inv->begin(); iter != inv->end(); ++iter) //new
	{

		if ((*iter)->eitemtype == ITEMTYPE_JELLY || (*iter)->eitemtype == ITEMTYPE_LEAVES || (*iter)->eitemtype == ITEMTYPE_POTION)
		{
			_bool bshouldpush = true;
			for (auto& iter1 = inv_useable->begin(); iter1 != inv_useable->end(); ++iter1) //new
			{
				if ((*iter1)->eitemname == (*iter)->eitemname)
				{
					(*iter1)->icount += 1;
					bshouldpush = false;
					break;
				}

			}

			if (bshouldpush)
				inv_useable->push_back(*iter);
		}


	}
}

void CUI_MENU_Back::update_material_inventory()
{
	vector<ITEMINFO*>* inv = CUI_Manager::Get_Instance()->Get_Inventory();
	vector<ITEMINFO*>* inv_material = CUI_Manager::Get_Instance()->Get_Inventory_material();//new

	for (auto& iter = inv->begin(); iter != inv->end(); ++iter) //new
	{

		if ((*iter)->eitemtype == ITEMTYPE_GRAIN || (*iter)->eitemtype == ITEMTYPE_VEGITABLE ||
			(*iter)->eitemtype == ITEMTYPE_FRUIT || (*iter)->eitemtype == ITEMTYPE_MEAT || (*iter)->eitemtype == ITEMTYPE_FISH
			|| (*iter)->eitemtype == ITEMTYPE_MATERIAL || (*iter)->eitemtype == ITEMTYPE_CONDIMENT )
		{
			_bool bshouldpush = true;
			for (auto& iter1 = inv_material->begin(); iter1 != inv_material->end(); ++iter1) //new
			{
				if ((*iter1)->eitemname == (*iter)->eitemname)
				{
					(*iter1)->icount += 1;
					bshouldpush = false;
					break;
				}

			}

			if (bshouldpush)
				inv_material->push_back(*iter);
		}


	}
	
}

void CUI_MENU_Back::update_weapon_inventory()
{
	vector<ITEMINFO*>* inv = CUI_Manager::Get_Instance()->Get_Inventory();
	vector<ITEMINFO*>* inv_wepon = CUI_Manager::Get_Instance()->Get_Inventory_weapon();//new

	for (auto& iter = inv->begin(); iter != inv->end(); ++iter) //new
	{

		if ((*iter)->eitemtype == ITEMTYPE_SWORD || (*iter)->eitemtype == ITEMTYPE_GUN || (*iter)->eitemtype == ITEMTYPE_BOOK || (*iter)->eitemtype == ITEMTYPE_GAUNTLET)
		{
			_bool bshouldpush = true;
			for (auto& iter1 = inv_wepon->begin(); iter1 != inv_wepon->end(); ++iter1) //new
			{
				if ((*iter1)->eitemname == (*iter)->eitemname)
				{
					(*iter1)->icount += 1;
					bshouldpush = false;
					break;
				}

			}

			if (bshouldpush)
				inv_wepon->push_back(*iter);
		}


	}
}

void CUI_MENU_Back::update_armor_inventory()
{

	vector<ITEMINFO*>* inv = CUI_Manager::Get_Instance()->Get_Inventory();
	vector<ITEMINFO*>* inv_armor = CUI_Manager::Get_Instance()->Get_Inventory_armor();//new

	for (auto& iter = inv->begin(); iter != inv->end(); ++iter) //new
	{

		if ((*iter)->eitemtype == ITEMTYPE_ARMOR)// || (*iter)->eitemtype == ITEMTYPE_LEAVES || (*iter)->eitemtype == ITEMTYPE_POTION)
		{
			_bool bshouldpush = true;
			for (auto& iter1 = inv_armor->begin(); iter1 != inv_armor->end(); ++iter1) //new
			{
				if ((*iter1)->eitemname == (*iter)->eitemname)
				{
					(*iter1)->icount += 1;
					bshouldpush = false;
					break;
				}

			}

			if (bshouldpush)
				inv_armor->push_back(*iter);
		}


	}
}

void CUI_MENU_Back::use_HPrecover_item()
{
	switch (m_eSelected_UsableItem)
	{

	case ITEMNAME_LEMONJELLY:  //여기서 셰이더 패스를 바꾸는 작업을 통해서 파란색으로 디는걸  하고.. 몇번째 플레이어를 받아왔냐에 따라서 걍 hp를 plus해주자 포인터로
		if(CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_iRecover = 2000;
		
		break;

	case ITEMNAME_HWANGJELLY:
		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_iRecover = 1000;
		break;

	case ITEMNAME_GRAPEJELLY:
		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_iRecover = 2800;
		break;

	case ITEMNAME_PINEAPPLEJELLY:
		break;

	case ITEMNAME_OMEGAELIXIR:
		break;

	case ITEMNAME_PANASIABOTTLE:
		break;

	case ITEMNAME_LIFEBOTTLE:
		CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Revive();
		m_iRecover = 800;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0,true);
		break;

	case ITEMNAME_REDSAGE:
		break;
	
	}
	
	if (m_iRecover > 0)
	{
		if ((m_iRecover + m_PlayersCurrentHP[m_iCursor_itemuse]) >= m_PlayersMaxHP[m_iCursor_itemuse])
			RecoverPlayerHP(m_iCursor_itemuse);
		else
			m_fRecover_Power = (_float)m_iRecover;

		m_brecover_finsh = false;
		m_brecovring = true;
	}
	
	//m_fResultHP = m_PlayersCurrentHP[m_iCursor_itemuse] + m_fRecover_Power;

	//if (m_iCursor_itemuse == 0)  // alpen
		 // 
	//else if (m_iCursor_itemuse == 1)  // sion
	//	RecoverPlayerHP(1, m_iRecover);
	//else if (m_iCursor_itemuse == 2)  // linwell
	//	RecoverPlayerHP(2, m_iRecover);
	//else if (m_iCursor_itemuse == 3)  // linwell
	//	RecoverPlayerHP(3, m_iRecover);

	m_iRecover = 0;


}

void CUI_MENU_Back::Fill_HP(_uint playerindex)
{
	if (m_fRecover_Power > 0.f)
	{
		if (m_fRecover_Power < 50.f)
		{
			m_PlayersCurrentHP[playerindex] += m_fRecover_Power;
			m_brecover_effect[playerindex] = true;
			goto first;
		}
		m_fRecover_Power -= 50.f;
		m_PlayersCurrentHP[playerindex] += 50.f;
	}
	
	if(m_fRecover_Power <= 0.f)
	{
		first:
		m_brecover_finsh = true;
		//m_brecover_effect = true;
		m_brecovring = false;
	}
		

}

void CUI_MENU_Back::check_HPrecover_item()
{
	switch (m_eSelected_UsableItem)
	{

	case ITEMNAME_LEMONJELLY:  //여기서 셰이더 패스를 바꾸는 작업을 통해서 파란색으로 디는걸  하고.. 몇번째 플레이어를 받아왔냐에 따라서 걍 hp를 plus해주자 포인터로
		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_fRecoverItempower = 2000;

		break;

	case ITEMNAME_HWANGJELLY:
		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_fRecoverItempower = 1000;
		break;

	case ITEMNAME_GRAPEJELLY:
		if (CPlayerManager::Get_Instance()->Get_EnumPlayer(m_iCursor_itemuse)->Get_Info().fCurrentHp > 0)
		m_fRecoverItempower = 2800;
		break;

	case ITEMNAME_PINEAPPLEJELLY:
		break;

	case ITEMNAME_OMEGAELIXIR:
		break;

	case ITEMNAME_PANASIABOTTLE:
		break;

	case ITEMNAME_LIFEBOTTLE:
		m_fRecoverItempower = 800;
		break;

	case ITEMNAME_REDSAGE:
		break;

	}

	if (!m_brecovring)
	{
		if ((m_fRecoverItempower + m_PlayersCurrentHP[m_iCursor_itemuse]) >= m_PlayersMaxHP[m_iCursor_itemuse])
			m_fBluepower = m_PlayersMaxHP[m_iCursor_itemuse];
		else
			m_fBluepower = m_fRecoverItempower + m_PlayersCurrentHP[m_iCursor_itemuse];
	}
	
}

void CUI_MENU_Back::Enter_Main_to_Another_Menu()
{
	
	m_ficonposition4 = m_ficonposition3 = m_ficonposition2 = m_ficonposition1 = 600.f;
//	if (m_etype == MENU_INVENTORY1)
		m_bfadein_inventory = true;
   if (m_etype == MENU_QUEST)
		m_bfadein_Quest = true;
	CUI_RuneEffect::RUNEDESC desc;
	desc.position.x = 605.f;
	desc.position.y = 70.f;
	desc.m_etype = 1;


	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
		return;
	m_fMainAlpha = 0.f;
	m_fMain_Bottom_buttonY = 400.f;
	
}

HRESULT CUI_MENU_Back::Render_Questmenu()
{

	/*if (FAILED(m_pShaderCom->Set_RawValue("g_fAlpha", &m_fQuestAlpha, sizeof(_float))))
		return E_FAIL; */
	m_fSize.x = 400.f;
	m_fSize.y = 310.f;
	m_fPosition.x = 300.f;
	m_fPosition.y = 360.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENBACK);

	m_pVIBufferCom->Render();

	m_fSize.x = 600;
	m_fSize.y = 120.f;
	m_fPosition.x = 900.f;
	m_fPosition.y = 180.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENBACK);

	m_pVIBufferCom->Render();

	m_fSize.y = 200.f;
	m_fPosition.y = 350.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENBACK);

	m_pVIBufferCom->Render();
	/////////////////////////////////////////////////// 커서 퓽퓽

	m_fSize.x = 850.f;
	m_fSize.y = 4.f;// +m_fFadeout_liney;
	m_fPosition.x = 1030.f;
	m_fPosition.y = 180.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom6->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_MENULINE);

	m_pVIBufferCom->Render();

	m_fSize.x = 850.f;
	m_fSize.y = 4.f;// +m_fFadeout_liney;
	m_fPosition.x = 1030.f;
	m_fPosition.y = 510.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom6->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_MENULINE);

	m_pVIBufferCom->Render();

	
	m_fPosition.x = 100.f + m_fCursorX;


	m_fSize.x = 64.f + m_fFadeout_cursor;
	m_fSize.y = 64.f + m_fFadeout_cursor;

	m_fPosition.y = 230.f + 25.f * m_iCursor_Quest;
	

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom3->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_CP_GUAGE_BLACK);

	m_pVIBufferCom->Render();



	m_fPosition.x = 250.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 230.f;// *m_iCursor_Quest;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_QuestIndex() >= 1)
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom21->Get_SRV(0))))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;

	}
	
	m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (CUI_Manager::Get_Instance()->Get_QuestIndex() >= 2)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom21->Get_SRV(1))))
				return E_FAIL;
		}
		else
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (CUI_Manager::Get_Instance()->Get_QuestIndex() >= 3)
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom21->Get_SRV(2))))
				return E_FAIL;
		}
		else
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
				return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();

		m_fPosition.y += 25.f;
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_ALPHASET);
		m_pVIBufferCom->Render();





	m_fPosition.x = 770.f;
	m_fSize.x = 280.f;
	m_fSize.y = 28.f;
	m_fPosition.y = 160.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (1<= CUI_Manager::Get_Instance()->Get_QuestIndex())
	{
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom21->Get_SRV(m_iCursor_Quest)))) /// quest name
			return E_FAIL;
	}
	else
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
			return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();
	/////////////////////////////////////////////////////questname/???

	

	//////////////////////////////////////questinfo
	
		m_fPosition.x = 750.f;
		m_fSize.x = 240.f;
		m_fSize.y = 24.f;
		m_fPosition.y = 230.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;
		if (1 <= CUI_Manager::Get_Instance()->Get_QuestIndex())
		{
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom25->Get_SRV(0)))) //////snowfield name
				return E_FAIL;
		}
		else
			if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(0))))
				return E_FAIL;

		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();
		if (1 <= CUI_Manager::Get_Instance()->Get_QuestIndex())
		{
			switch (m_iCursor_Quest)
			{
			case 0 :
				Render_Quest1Rewards();
				break;

			case 1:
				Render_Quest2Rewards();
				break;

			case 2:
				Render_Quest3Rewards();
				break;
			}
			
			if (m_bQuestInformation)
			{
				m_fPosition.x = 830.f;
				m_fSize.x = 420.f;
				m_fSize.y = 200.f;
				m_fPosition.y = 365.f;
				m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
				m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
				m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
				if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
					return E_FAIL;

				if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom24->Get_SRV(m_iCursor_Quest))))
					return E_FAIL;
				m_pShaderCom->Begin(UI_BRIGHT);
				m_pVIBufferCom->Render();
			}
			else
			{
				switch (m_iCursor_Quest)
				{
				case 0:
					Render_Quest1TODO();
					break;

				case 1:
					Render_Quest2TODO();
					break;

				case 2:
					Render_Quest3TODO();
					break;
				}
	     	}
		}


		m_fPosition.x = 720.f;
		m_fSize.x = 220.f;
		m_fSize.y = 24.f;
		m_fPosition.y = 495.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(2))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();

		



		return S_OK;
}

HRESULT CUI_MENU_Back::Render_bottmline_Quest()
{
	//////////////////////////////////////bottomline
	m_fSize.x = 1280.f;
	m_fSize.y = 130.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 715.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENTOPBOTTOM);

	m_pVIBufferCom->Render();
	//////////////////////////////////////bottomline
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_bottmline_Battleorder()
{
	//////////////////////////////////////bottomline
	m_fSize.x = 1280.f;
	m_fSize.y = 130.f;
	m_fPosition.x = 640.f;
	m_fPosition.y = 715.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_INVENTOPBOTTOM);

	m_pVIBufferCom->Render();

	m_fSize.x = 1400.f ;
	m_fSize.y = 4.f ;
	m_fPosition.x = 1000.f;
	m_fPosition.y = 480.f;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom6->Get_SRV(0))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_GradationTexture", m_pTextureCom6->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_MENULINE);

	m_pVIBufferCom->Render();
	//////////////////////////////////////bottomline
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest1Rewards()
{
	m_fPosition.x = 800.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 525.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();



	m_fPosition.x = 645.f;
	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 526.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(7))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x = 770.f;
	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x = 649.f;
	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest2Rewards()
{
	m_fPosition.x = 800.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 525.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();



	m_fPosition.x = 645.f;
	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 526.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x = 770.f;
	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x = 649.f;
	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(ITEMNAME_SONG2MUSHROOM))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest3Rewards()
{
	m_fPosition.x = 800.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 525.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(4))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();



	m_fPosition.x = 645.f;
	m_fSize.x = 24.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 526.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(7))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x += 16.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));

	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom5->Get_SRV(m_igaldnum))))
		return E_FAIL;
	m_pShaderCom->Begin(m_eShaderID);

	m_pVIBufferCom->Render();


	m_fPosition.x = 770.f;
	m_fSize.x = 200.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom4->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x = 649.f;
	m_fSize.x = 32.f;
	m_fSize.y = 32.f;
	m_fPosition.y = 565.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom7->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest1TODO()
{
	m_fPosition.x = 730.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 275.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x = 840.f;
	m_fSize.x = 380.f;
	m_fSize.y = 28.f;
	m_fPosition.y = 300.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom23->Get_SRV(0))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Quest1_Lettuce() >= 3)
		m_pShaderCom->Begin(UI_TODOCOMPLETE);
	else
		m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	if (CUI_Manager::Get_Instance()->Get_Quest1_Lettuce() >= 3)
	{
		m_fPosition.x = 635.f;
		m_fSize.x = 36.f;
		m_fSize.y = 36.f;
		m_fPosition.y = 300.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();
	}

	m_fPosition.x = 840.f;
	m_fSize.x = 380.f;
	m_fSize.y = 28.f;
	m_fPosition.y = 340.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom23->Get_SRV(1))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_Quest1_Herb() >= 3)
		m_pShaderCom->Begin(UI_TODOCOMPLETE);
	else
		m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();


	if (CUI_Manager::Get_Instance()->Get_Quest1_Herb() >= 3)
	{
		m_fPosition.x = 635.f;
		m_fSize.x = 36.f;
		m_fSize.y = 36.f;
		m_fPosition.y = 340.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();
	}
	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest2TODO()
{
	m_fPosition.x = 730.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 275.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x = 840.f;
	m_fSize.x = 380.f;
	m_fSize.y = 28.f;
	m_fPosition.y = 300.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom23->Get_SRV(2))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_QuestComplete(1))
		m_pShaderCom->Begin(UI_TODOCOMPLETE);
	else
		m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	if (CUI_Manager::Get_Instance()->Get_QuestComplete(1))
	{
		m_fPosition.x = 635.f;
		m_fSize.x = 36.f;
		m_fSize.y = 36.f;
		m_fPosition.y = 300.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();
	}




	return S_OK;
}

HRESULT CUI_MENU_Back::Render_Quest3TODO()
{
	m_fPosition.x = 730.f;
	m_fSize.x = 220.f;
	m_fSize.y = 24.f;
	m_fPosition.y = 275.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(1))))
		return E_FAIL;
	m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	m_fPosition.x = 840.f;
	m_fSize.x = 380.f;
	m_fSize.y = 28.f;
	m_fPosition.y = 300.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom23->Get_SRV(3))))
		return E_FAIL;
	if (CUI_Manager::Get_Instance()->Get_QuestComplete(2))
		m_pShaderCom->Begin(UI_TODOCOMPLETE);
	else
		m_pShaderCom->Begin(UI_BRIGHT);
	m_pVIBufferCom->Render();

	if (CUI_Manager::Get_Instance()->Get_QuestComplete(2))
	{
		m_fPosition.x = 635.f;
		m_fSize.x = 36.f;
		m_fSize.y = 36.f;
		m_fPosition.y = 300.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom22->Get_SRV(3))))
			return E_FAIL;
		m_pShaderCom->Begin(UI_BRIGHT);
		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_MENU_Back::Render_BattleOrder()
{
	Render_OrderFlags();


	m_fPosition.x = 350.f;
	m_fPosition.y = 200.f;
	m_fSize.x = 400.f;
	m_fSize.y = 28.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
		m_pTextureCom27->Get_SRV(0))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.y += 50.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
		m_pTextureCom27->Get_SRV(1))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.y += 50.f;
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
		m_pTextureCom27->Get_SRV(2))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	m_fPosition.x = 850.f;
	m_fPosition.y = 200.f;
	m_fSize.x = 400.f;
	m_fSize.y = 28.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
		m_pTextureCom27->Get_SRV(6))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	if (m_bAIcanuseItem)
	{
		m_fPosition.x = 630.f;
		m_fPosition.y = 195.f;
		m_fSize.x = 48.f;
		m_fSize.y = 48.f;
		m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
		m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
		if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom27->Get_SRV(7))))
			return E_FAIL;

		m_pShaderCom->Begin(UI_BRIGHT);

		m_pVIBufferCom->Render();

}
	

	m_fPosition.x = 730.f;
	m_fPosition.y = 460.f;
	m_fSize.x = 500.f;
	m_fSize.y = 36.f ;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	
		if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
			m_pTextureCom27->Get_SRV(m_iOrdercursor))))
			return E_FAIL;
	
	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();
	//return S_OK;

	m_fPosition.x = 660.f;
	m_fPosition.y = 550.f;
	m_fSize.x = 600.f;
	m_fSize.y = 110.f;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture",
		m_pTextureCom27->Get_SRV(m_iOrdercursor+3))))
		return E_FAIL;

	m_pShaderCom->Begin(UI_BRIGHT);

	m_pVIBufferCom->Render();

	Render_Orderbottombutton();
	return S_OK;
}

void CUI_MENU_Back::Return_to_MainMenu()
{
	m_bfadeout_inventory = false;
	m_etype = MENU_MAIN;   //from inventory to main return to main
	m_bMainAlphaup = true;
	m_biconfirstmove = true;
	//m_fInven_Bottom_buttonY = 0.f;
	CUI_RuneEffect::RUNEDESC desc;
	desc.position.x = 110.f;
	desc.position.y = 110.f;
	desc.m_etype = 1;


	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
		return;

	desc.position.y += 55.f;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
		return;

	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
	{
		desc.position.y += 55.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
			return;
	}
	if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
	{
		desc.position.y += 55.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_SNOWFIELD, TEXT("test"), &desc)))
			return;
	}
}

HRESULT CUI_MENU_Back::Render_OrderFlags()
{
	switch (m_iOrdercursor)
	{
	case 0:
		break;

	case 1:
		CBattleManager::Get_Instance()->Set_AImode(CAIState::AI_MODE::ATTACK_LOCKONMODE);
		break;

	case 2:
		CBattleManager::Get_Instance()->Set_AImode(CAIState::AI_MODE::ATTACK_FREEMODE);
		break;

	}
	m_fPosition.x = 110.f;
	switch (m_iOrdercursor)
	{
	case 0:
		m_fPosition.y = 198.f;
		break;

	case 1:
		m_fPosition.y = 248.f;
		break;

	case 2:
		m_fPosition.y = 298.f;
		break;

	case 3:
		m_fPosition.y = 348.f;
		break;

	}
	m_fSize.x = 46.f;// +m_fFadeout_categoryeffectx;
	m_fSize.y = 46.f;// +m_fFadeout_categoryeffectx;;
	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_fSize.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_fSize.y);
	m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_fPosition.x - g_iWinSizeX * 0.5f, -m_fPosition.y + g_iWinSizeY * 0.5f, 0.f, 1.f));
	if (FAILED(m_pShaderCom->Set_RawValue("g_WorldMatrix", &m_pTransformCom->Get_World4x4_TP(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom18->Get_SRV(0))))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}
