#include "stdafx.h"
#include "..\Public\UI_Manager.h"
#include "Obj_UI.h"
#include "GameInstance.h"
#include "InvenTile.h"
#include "Player.h"
#include "PlayerState.h"
#include "UIButton.h"
#include "Npc.h"
#include "MarinNpc.h"
#include "CraneGameNpc.h"
#include "InvenItem.h"
#include "FieldNpc.h"
#include "Npc.h"
#include "UIName.h"
#include "UIIcon.h"

IMPLEMENT_SINGLETON(CUI_Manager)

CUI_Manager::CUI_Manager()
{
}

void CUI_Manager::Initialize_PlayerState()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	for (int i = 0; i < 2; ++i)
	{
		CPlayerState::STATEDESC StateDesc;
		StateDesc.m_eType = CPlayerState::KEY;
		StateDesc.fPosition = _float2(30.f + (i % 8) * 30.f, 90.f);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerState"), LEVEL_STATIC, TEXT("Layer_State"), &StateDesc)))
			return;

	}

	


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Inventory()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Up(DIK_RIGHT))
	{
		
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSelect.wav"), SOUND_SYSTEM, g_fUIVolume + 0.3f);
		m_iInvenIndex++;

		if (m_iInvenIndex >= m_InvenTile.size())
			m_iInvenIndex = 0;
	}
		

	if (pGameInstance->Key_Up(DIK_LEFT))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSelect.wav"), SOUND_SYSTEM, g_fUIVolume + 0.3f);

		m_iInvenIndex--;

		if (m_iInvenIndex < 0)
			m_iInvenIndex = (int)m_InvenTile.size() -1 ;
	}
	

	if (pGameInstance->Key_Up(DIK_DOWN))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSelect.wav"), SOUND_SYSTEM, g_fUIVolume + 0.3f);
		m_iInvenIndex += 4;

		if (m_iInvenIndex >= m_InvenTile.size())
			m_iInvenIndex %= 4;
	}

	if (pGameInstance->Key_Up(DIK_UP))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSelect.wav"), SOUND_SYSTEM, g_fUIVolume + 0.3f);
		m_iInvenIndex -= 4;

		if (m_iInvenIndex < 0)
			m_iInvenIndex += 12;
	}
		
	dynamic_cast<CInvenTile*>(m_pPickedTile)->Set_SelectTile(false);
	m_pPickedTile = m_InvenTile[m_iInvenIndex];
	dynamic_cast<CInvenTile*>(m_pPickedTile)->Set_SelectTile(true);


	if (pGameInstance->Key_Up(DIK_X))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_ShortCut_EquipDecide.wav"), SOUND_SYSTEM, g_fUIVolume);
		if(m_EquipTile[EQUIP_X] != nullptr)
			dynamic_cast<CInvenTile*>(m_EquipTile[EQUIP_X])->Set_TileState(CInvenTile::STATE_DEFAULT);

		m_EquipTile[EQUIP_X] = m_pPickedTile;
		dynamic_cast<CInvenTile*>(m_EquipTile[EQUIP_X])->Set_TileState(CInvenTile::STATE_EQUIP);
	}
		

	if (pGameInstance->Key_Up(DIK_Y))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_ShortCut_EquipDecide.wav"), SOUND_SYSTEM, g_fUIVolume);
		if (m_EquipTile[EQUIP_Y] != nullptr)
			dynamic_cast<CInvenTile*>(m_EquipTile[EQUIP_Y])->Set_TileState(CInvenTile::STATE_DEFAULT);

		m_EquipTile[EQUIP_Y] = m_pPickedTile;
		dynamic_cast<CInvenTile*>(m_EquipTile[EQUIP_Y])->Set_TileState(CInvenTile::STATE_EQUIP);
	}
		

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Map()
{
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Get_CurrentLevelIndex() != LEVEL_GAMEPLAY)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	if (pGameInstance->Key_Up(DIK_RIGHT))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_CheckPoint.wav"), SOUND_SYSTEM, g_fUIVolume);
		_int iIndex = 0;
		_float fMinDistance = MIN_DISTANCE;
		for (auto& iter : m_WarpGroup)
		{
			_float fXDistance = iter->Get_Position().x - m_WarpGroup[m_iWarpIndex]->Get_Position().x;
			if (fXDistance < fMinDistance && fXDistance > 0)
			{
				fMinDistance = fXDistance;
				m_iMInMaxIndex = iIndex;
			}
			iIndex++;
		}

		if (fMinDistance == MIN_DISTANCE)
		{
			iIndex = 0;
			_float fMaxDistance = MAX_DISTANCE;
			for (auto& iter : m_WarpGroup)
			{
				_float fXDistance = m_WarpGroup[m_iWarpIndex]->Get_Position().x - iter->Get_Position().x;
				if (fXDistance > fMaxDistance && fXDistance > 0)
				{
					fMaxDistance = fXDistance;
					m_iMInMaxIndex = iIndex;
				}
				iIndex++;
			}
		}
	}
	else if (pGameInstance->Key_Up(DIK_LEFT))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_CheckPoint.wav"), SOUND_SYSTEM, g_fUIVolume);
		_int iIndex = 0;
		_float fMaxDistance = MAX_DISTANCE;
		for (auto& iter : m_WarpGroup)
		{
			_float fXDistance = iter->Get_Position().x - m_WarpGroup[m_iWarpIndex]->Get_Position().x;
			if (fXDistance > fMaxDistance && fXDistance < 0)
			{
				fMaxDistance = fXDistance;
				m_iMInMaxIndex = iIndex;
			}
			iIndex++;
		}

		if (fMaxDistance == MAX_DISTANCE)
		{
			iIndex = 0;
			_float fMinDistance = MIN_DISTANCE;
			for (auto& iter : m_WarpGroup)
			{
				_float fXDistance = m_WarpGroup[m_iWarpIndex]->Get_Position().x - iter->Get_Position().x;
				if (fXDistance < fMinDistance && fXDistance < 0)
				{
					fMinDistance = fXDistance;
					m_iMInMaxIndex = iIndex;
				}
				iIndex++;
			}
		}
	}
	else if (pGameInstance->Key_Up(DIK_UP))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_CheckPoint.wav"), SOUND_SYSTEM, g_fUIVolume);
		_int iIndex = 0;
		_float fMaxDistance = MAX_DISTANCE;
		for (auto& iter : m_WarpGroup)
		{
			_float fYDistance = iter->Get_Position().y - m_WarpGroup[m_iWarpIndex]->Get_Position().y;
			if (fYDistance > fMaxDistance && fYDistance < 0)
			{
				fMaxDistance = fYDistance;
				m_iMInMaxIndex = iIndex;
			}
			iIndex++;
		}

		if (fMaxDistance == MAX_DISTANCE)
		{
			iIndex = 0;
			_float fMinDistance = MIN_DISTANCE;
			for (auto& iter : m_WarpGroup)
			{
				_float fYDistance = m_WarpGroup[m_iWarpIndex]->Get_Position().y - iter->Get_Position().y;
				if (fYDistance < fMinDistance && fYDistance < 0)
				{
					fMinDistance = fYDistance;
					m_iMInMaxIndex = iIndex;
				}
				iIndex++;
			}

		}
	}
	else if (pGameInstance->Key_Up(DIK_DOWN))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Sys_CheckPoint.wav"), SOUND_SYSTEM, g_fUIVolume);
		_int iIndex = 0;
		_float fMinDistance = MIN_DISTANCE;
		for (auto& iter : m_WarpGroup)
		{
			_float fYDistance = iter->Get_Position().y - m_WarpGroup[m_iWarpIndex]->Get_Position().y;
			if (fYDistance < fMinDistance && fYDistance > 0)
			{
				fMinDistance = fYDistance;
				m_iMInMaxIndex = iIndex;
			}
			iIndex++;
		}

		if (fMinDistance == MIN_DISTANCE)
		{
			iIndex = 0;
			_float fMaxDistance = MAX_DISTANCE;
			for (auto& iter : m_WarpGroup)
			{
				_float fYDistance = m_WarpGroup[m_iWarpIndex]->Get_Position().y - iter->Get_Position().y;
				if (fYDistance > fMaxDistance && fYDistance > 0)
				{
					fMaxDistance = fYDistance;
					m_iMInMaxIndex = iIndex;
				}
				iIndex++;
			}
		}
	}

	m_iWarpIndex = m_iMInMaxIndex;
	dynamic_cast<CUIIcon*>(m_pPickedWarp)->Set_TexutureNum(CUIIcon::ICON_WARP);
	m_pPickedWarp = m_WarpGroup[m_iWarpIndex];
	dynamic_cast<CUIIcon*>(m_pPickedWarp)->Set_TexutureNum(CUIIcon::ICON_WARP_ON);


	if (pGameInstance->Key_Up(DIK_SPACE))
	{
		pGameInstance->PlaySounds(TEXT("6_UI_Map_SensorOn.wav"), SOUND_SYSTEM, g_fUIVolume);
		_vector vWarpPosition =  dynamic_cast<CUIIcon*>(m_pPickedWarp)->Get_IconDesc().pTarget->Get_TransformState(CTransform::STATE_POSITION);
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayer->Set_WarpPosition(vWarpPosition);
		pPlayer->Set_AnimState(CPlayer::WARP_ST);

		Set_UI_OpenType(UI_END);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_PlayerState()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	/* HP Update */
	_int iMaxHp = (pPlayer->Get_Info().iMaxHp) / 4;
	if (iMaxHp > m_HpList.size())
	{
		for (auto& iter : m_HpList)
			Safe_Release(iter);
		m_HpList.clear();

	
		if ((pPlayer->Get_Info().iMaxHp) % 4 != 0)
			iMaxHp++;
		int iCol = 0;
		for (int i = 0; i < iMaxHp; ++i)
		{
			CPlayerState::STATEDESC StateDesc;
			StateDesc.m_eType = CPlayerState::HP;
			StateDesc.fPosition = _float2(30.f + (i % 8) * 30.f, 30.f + iCol * 30.f);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerState"), LEVEL_STATIC, TEXT("Layer_Hp"), &StateDesc)))
				return;

			if (i % 8 == 7)
				++iCol;
		}
		
		

	}

	_int iCurrentFullHp = (pPlayer->Get_Info().iCurrentHp) / 4;
	_int iCurrentLastHp = (pPlayer->Get_Info().iCurrentHp) % 4;

	for (int i = 0; i < iCurrentFullHp; ++i)
		dynamic_cast<CPlayerState*>(m_HpList[i])->Set_TextureNum(CPlayerState::HP100);
	
	if (iCurrentLastHp == 0)
	{
		if (m_HpList.size() <= iCurrentFullHp)
		{
			RELEASE_INSTANCE(CGameInstance);
			return;
		}
		dynamic_cast<CPlayerState*>(m_HpList[iCurrentFullHp])->Set_TextureNum(CPlayerState::HP0);
	}
	if (iCurrentLastHp == 1)
		dynamic_cast<CPlayerState*>(m_HpList[iCurrentFullHp])->Set_TextureNum(CPlayerState::HP25);
	if (iCurrentLastHp == 2)
		dynamic_cast<CPlayerState*>(m_HpList[iCurrentFullHp])->Set_TextureNum(CPlayerState::HP50);
	if (iCurrentLastHp == 3)
		dynamic_cast<CPlayerState*>(m_HpList[iCurrentFullHp])->Set_TextureNum(CPlayerState::HP75);


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Coin()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	

	int iRuby = pPlayer->Get_Info().iCoin;
	int iRubyShare = iRuby;
	int iRubyShareSize = 0;

	while (true)
	{
		int iDivid = iRubyShare % 10;
		iRubyShare = iRubyShare / 10;
		++iRubyShareSize;

		if (iRubyShare == 0)
			break;
	}


	if (m_iRubyShareSize != 0 && iRubyShareSize == m_iRubyShareSize)
	{
		if (m_RubyList.empty())
			return;

		int iShare = iRuby;

		int i = 0;
	
		while (true)
		{
			int iDivid = iShare % 10;

			dynamic_cast<CPlayerState*>(m_RubyList[m_iRubyShareSize - i])->Set_TextureNum(iDivid);

			iShare = iShare / 10;
			++i;

			if (iShare == 0)
				break;
		}

	}
	else
	{

		for (auto& iter : m_RubyList)
			iter->Set_Dead(true);
		m_RubyList.clear();

		CPlayerState::STATEDESC StateDesc;
		StateDesc.m_eType = CPlayerState::RUBY;
		StateDesc.fPosition = _float2(300.f, 30.f);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerState"), LEVEL_STATIC, TEXT("Layer_Number"), &StateDesc)))
			return;

		int iShare = iRuby;

		int i = 0;

		while (true)
		{
			int iDivid = iShare % 10;

			CPlayerState::STATEDESC StateDesc;
			StateDesc.m_eType = CPlayerState::NUMBER;
			StateDesc.fPosition = _float2(320.f + i * 20.f, 30.f);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerState"), LEVEL_STATIC, TEXT("Layer_Number"), &StateDesc)))
				return;

			iShare = iShare / 10;
			++i;

			if (iShare == 0)
				break;
		}

		m_iRubyShareSize = i;
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Message()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (m_bTalking == true)
	{
		if (m_vecMsgDecs.front().eMsgType == MUST_CHOICE)
		{
			Setting_ChoiceButton();

			if (pGameInstance->Key_Up(DIK_UP))
			{
				m_iChoiceIndex--;

				if (m_iChoiceIndex < 0)
					m_iChoiceIndex = 0;
				pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSwitch_On.wav"), SOUND_SYSTEM, g_fUIVolume);
			}

			if (pGameInstance->Key_Up(DIK_DOWN))
			{
				m_iChoiceIndex++;

				if (m_iChoiceIndex >= m_vecChoiceButton.size())
					m_iChoiceIndex = (_int)(m_vecChoiceButton.size() - 1);
				pGameInstance->PlaySounds(TEXT("6_UI_Sys_SlideSwitch_On.wav"), SOUND_SYSTEM, g_fUIVolume);

			}

			if (m_vecChoiceButton.size() != 0)
			{
				for (auto& iter : m_vecChoiceButton)
				{
					if (iter == m_vecChoiceButton[m_iChoiceIndex])
						continue;

					iter->Set_Picked(false);
				}
				if (m_vecChoiceButton[m_iChoiceIndex] != nullptr)
					m_vecChoiceButton[m_iChoiceIndex]->Set_Picked(true);

			}
			
			if (pGameInstance->Key_Up(DIK_SPACE) && m_vecChoiceButton.size() != 0)
			{
				m_pTalkingNpc->Send_Answer_toNPC(dynamic_cast<CUIButton*>(m_vecChoiceButton[m_iChoiceIndex])->Get_TextureType());
				m_vecMsgDecs.pop_front();
				pGameInstance->PlaySounds(TEXT("6_UI_Sys_SoftwareKeyboard_Decide.wav"), SOUND_SYSTEM, g_fUIVolume);

			
			}
		}
		else
		{
			if (CGameInstance::Get_Instance()->Key_Up(DIK_SPACE))
			{
				pGameInstance->PlaySounds(TEXT("6_UI_Sys_Talk_Next.wav"), SOUND_SYSTEM, g_fUIVolume);
				m_vecMsgDecs.pop_front();
			}
				

			if (m_vecMsgDecs.size() == 0)
			{
  				if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::MARIN)
				{
					m_bNpcGet = true;
					if (dynamic_cast<CMarinNpc*>(m_pTalkingNpc)->Get_IsGet() == true)
					{
						dynamic_cast<CMarinNpc*>(m_pTalkingNpc)->Set_GetMode();
						(m_pTalkingNpc)->GiveItemMode();
					}
					
				}
				else if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::CRANE_GAME)
				{
					m_bPlayGame = dynamic_cast<CCraneGameNpc*>(m_pTalkingNpc)->Get_GameStart();
				}
				else if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::MADAM)
				{
					if ((m_pTalkingNpc)->Get_GiveItem() == true)
						(m_pTalkingNpc)->GiveItemMode();
				}
				else if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::MOTHER)
				{
					if ((m_pTalkingNpc)->Get_GiveItem() == true)
						(m_pTalkingNpc)->GiveItemMode();
				}
				else if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::SHOP)
				{
					if ((m_pTalkingNpc)->Get_GiveItem() == true)
						(m_pTalkingNpc)->GiveItemMode();
				}
				else if (m_pTalkingNpc != nullptr && m_pTalkingNpc->Get_NpcID() == CNpc::TARIN)
				{
					if ((m_pTalkingNpc)->Get_GiveItem() == true)
						(m_pTalkingNpc)->GiveItemMode();
				}
			}
		}

		if (m_vecMsgDecs.size() == 0)
		{
			CGameObject* pTarget = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
			pPlayer->Set_PlayerState_Defaut();

			
			pGameInstance->PlaySounds(TEXT("6_UI_Sys_Talk_End.wav"), SOUND_SYSTEM, g_fUIVolume);
			m_pTalkingNpc = nullptr;
			m_bTalking = false;
			m_bChoice = false;
			m_bNpcGet = false;
		}

	}
	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Tick_Name(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CUIName* pName = dynamic_cast<CUIName*>(m_pNameBox);

	if (m_fNameTime > 1.f && !m_bFirst)
	{
		LEVEL iLevelIndex = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

		switch (iLevelIndex)
		{
		case Client::LEVEL_GAMEPLAY:
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

			_float fPositionZ = XMVectorGetZ(pPlayer->Get_TransformState(CTransform::STATE_POSITION));
			if (fPositionZ < 30.f)
				pName->Set_TexType(CUIName::NAME_FOREST);
			else
				pName->Set_TexType(CUIName::NAME_FIELD);
			pName->Set_Position(_float2(150, 150));
			break;
		}
		case Client::LEVEL_TOWER:
			pName->Set_TexType(CUIName::NAME_TOWER);
			pName->Set_Position(_float2(g_iWinSizeX*0.5f, g_iWinSizeY*0.5f));
			break;
		case Client::LEVEL_ROOM:
			pName->Set_Position(_float2(150, 150));
			switch (m_eRoomType)
			{
			case Client::CUI_Manager::MARINHOUSE:
				pName->Set_TexType(CUIName::NAME_MARINHOUSE);
				break;
			case Client::CUI_Manager::SHOP:
				pName->Set_TexType(CUIName::NAME_SHOP);
				break;
			case Client::CUI_Manager::CRANEGAME:
				pName->Set_TexType(CUIName::NAME_CRANEGAME);
				break;
			case Client::CUI_Manager::TELEPHONE:
				pName->Set_TexType(CUIName::NAME_TELLROOM);
				break;
			default:
				break;
			}
			break;
		case Client::LEVEL_TAILCAVE:
			pName->Set_TexType(CUIName::NAME_TAILCAVE);
			pName->Set_Position(_float2(g_iWinSizeX*0.5f, g_iWinSizeY*0.5f));
			break;
		default:
			break;
		}

		
		pName->Set_Visible(true);
		m_bFirst = true;
	}

	m_fNameTime += fTimeDelta;

	if(m_fNameTime > 4.f)
		pName->Set_Visible(false);

	RELEASE_INSTANCE(CGameInstance);
}

void CUI_Manager::Setting_ChoiceButton()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (!m_bChoice)
	{
		if (m_vecMsgDecs.front().eChoiceType == BUY_NOBUY)
		{
		CUIButton::BUTTONDESC ButtonDesc;
		ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
		ButtonDesc.iTexNum = CUIButton::BUY;
		ButtonDesc.vPosition = _float2(1100, 550);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
			return;

		ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
		ButtonDesc.iTexNum = CUIButton::NOBUY;
		ButtonDesc.vPosition = _float2(1100, 610);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
			return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == TALK_NOTTALK)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::NPC_TALK;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::LOVEU;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::BACK;
			ButtonDesc.vPosition = _float2(1100, 670);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_PERFUME)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::PERFUME_COMPLETE;
			ButtonDesc.vPosition = _float2(1100, 490);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::PERFUME_FAIL1;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::PERFUME_FAIL2;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::PERFUME_FAIL3;
			ButtonDesc.vPosition = _float2(1100, 670);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_CHANGE)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHANGE_COMPLETE;
			ButtonDesc.vPosition = _float2(1100, 490);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHANGE_FAIL1;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHANGE_FAIL2;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHANGE_FAIL3;
			ButtonDesc.vPosition = _float2(1100, 670);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_VECTOR)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CROSS_FAIL1;
			ButtonDesc.vPosition = _float2(1100, 490);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CROSS_COMPLETE;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CROSS_FAIL2;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_DOT)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::DOT_FAIL1;
			ButtonDesc.vPosition = _float2(1100, 490);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::DOT_COMPLETE;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::DOT_FAIL2;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_CHARM)
		{
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHARM_1;
			ButtonDesc.vPosition = _float2(1100, 490);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHARM_2;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHARM_3;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::CHARM_4;
			ButtonDesc.vPosition = _float2(1100, 670);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;
		}
		else if (m_vecMsgDecs.front().eChoiceType == MARIN_DATEME)
		{
		
			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::DATE_WITH_ME;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::SORRY;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

		}
		else if (m_vecMsgDecs.front().eChoiceType == DO_UNDO)
		{

			CUIButton::BUTTONDESC ButtonDesc;
			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::DO;
			ButtonDesc.vPosition = _float2(1100, 550);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

			ButtonDesc.eButtonType = CUIButton::BTN_CHOICE;
			ButtonDesc.iTexNum = CUIButton::UNDO;
			ButtonDesc.vPosition = _float2(1100, 610);

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, TEXT("Layer_UIChoice"), &ButtonDesc)))
				return;

		}
		m_bChoice = true;
	}

	

	RELEASE_INSTANCE(CGameInstance);
}


void CUI_Manager::Tick_UI(_float fTimeDelta)
{
	Tick_PlayerState();
	Tick_Coin();
	Tick_Message();
	Tick_Name(fTimeDelta);
}

void CUI_Manager::Set_UI_OpenType(UITYPE eUIType)
{
	if (m_eUIType == eUIType)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Event_MamoNum_CountDownEnd.wav"), SOUND_SYSTEM, 1.f);
		m_eUIType = UI_END;
	}
	else
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Event_MamoNum_CountDown.wav"), SOUND_SYSTEM, 1.f);
		m_eUIType = eUIType;
	}
		

	for(_uint i =0; i < m_InvenTile.size() ; ++i)
		dynamic_cast<CInvenTile*>(m_InvenTile[i])->Set_SelectTile(false);

	m_pPickedTile = m_InvenTile[0];
	dynamic_cast<CInvenTile*>( m_pPickedTile)->Set_SelectTile(true);


	for (_uint i = 0; i < m_WarpGroup.size(); ++i)
		dynamic_cast<CUIIcon*>(m_WarpGroup[i])->Set_TexutureNum(CUIIcon::ICON_WARP);

	if (m_WarpGroup.size() != 0)
	{
		m_pPickedWarp = m_WarpGroup[0];
		dynamic_cast<CUIIcon*>(m_pPickedWarp)->Set_TexutureNum(CUIIcon::ICON_WARP);
	}
	
}

void CUI_Manager::Set_EquipItem(EQUIP_BT eEquipBt, CObj_UI * pObj)
{
	m_EquipTile[eEquipBt] = pObj;
}

void CUI_Manager::Set_NextMessage()
{
	if (m_vecMsgDecs.size() != 0)
	{
		m_vecMsgDecs.pop_front();
		CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Sys_Talk_Next.wav"), SOUND_SYSTEM, g_fUIVolume);
	}
		

	 if (m_vecMsgDecs.size() == 0)
	 {
		 CGameObject* pTarget = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		 CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
		 pPlayer->Set_PlayerState_Defaut();


		 m_pTalkingNpc = nullptr;
		 m_bTalking = false;
		 m_bChoice = false;
		 m_bNpcGet = false;
	 }
}

void CUI_Manager::Get_Key()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayerState::STATEDESC StateDesc;
	StateDesc.m_eType = CPlayerState::KEY;
	StateDesc.fPosition = _float2(30.f + (m_KeyList.size() % 8) * 30.f, 90.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PlayerState"), LEVEL_STATIC, TEXT("Layer_State"), &StateDesc)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

_bool CUI_Manager::Get_Is_HaveItem(LEVEL eLevel, _tchar * LayerTag, _uint ItemtexTureNum)
{
	list<CGameObject*>* pQuestItemList = CGameInstance::Get_Instance()->Get_ObjectList(eLevel, LayerTag);
	for (auto& iter : *pQuestItemList)
	{
		if (dynamic_cast<CInvenItem*>(iter)->Get_TextureNum() == ItemtexTureNum)
		{
			return true;
		}
	}

	return false;
}

void CUI_Manager::Use_Key()
{
	m_KeyList.back()->Set_Dead(true);
	m_KeyList.pop_back();
}

void CUI_Manager::Clear_ChoiceButton()
{
	for (auto& iter : m_vecChoiceButton)
	{
		dynamic_cast<CUIButton*>(iter)->Set_Visible(false);
	}

	m_vecChoiceButton.clear();
	m_iChoiceIndex = 0;
	m_bChoice = false;
}

void CUI_Manager::Clear_WarpGroup()
{
	m_WarpGroup.clear();
}


void CUI_Manager::Free()
{
}
