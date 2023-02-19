#include "stdafx.h"
#include "..\Public\PlayerManager.h"
#include "BaseObj.h"
#include "Player.h"
#include "GameInstance.h"
#include "BattleManager.h"
#include "Monster.h"
//#include ""

IMPLEMENT_SINGLETON(CPlayerManager);


CPlayerManager::CPlayerManager()
{
}

void CPlayerManager::Set_ActivePlayer(CPlayer * pPlayer)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if(iLevel == LEVEL_LOADING)
		iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	if (m_pActivePlayer != nullptr && !m_bBattleMode)
	{
		_vector vPosition = m_pActivePlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pActivePlayer = pPlayer;
		m_pActivePlayer->Set_State(CTransform::STATE_TRANSLATION, vPosition);	
	}
	else
		m_pActivePlayer = pPlayer;

	m_pActivePlayer->Change_Navigation(iLevel);
	m_pActivePlayer->Compute_CurrentIndex(iLevel);
	m_pActivePlayer->Check_Navigation();


	m_pAIPlayers.clear();
	list<CGameObject*>* pPlayerLists =  CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));
	for (auto& iter : *pPlayerLists)
	{
		if (iter == m_pActivePlayer)
			continue;
		m_pAIPlayers.push_back(dynamic_cast<CPlayer*>(iter));
	}
}

void CPlayerManager::Set_ActivePlayer(_uint iIndex)
{
	list<CGameObject*>* pPlayerLists = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));
	_uint index = iIndex;

	if (iIndex >= pPlayerLists->size())
		index = ALPHEN;

	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_CurrentLevelIndex();
	if (iLevel == LEVEL_LOADING)
		iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	if (m_pActivePlayer != nullptr && !m_bBattleMode)
	{
		_vector vPosition = m_pActivePlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pActivePlayer = m_AllPlayers[index];
		m_pActivePlayer->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
		m_pActivePlayer = m_AllPlayers[index];

	m_pActivePlayer->Change_Navigation(iLevel);
	m_pActivePlayer->Compute_CurrentIndex(iLevel);
	m_pActivePlayer->Check_Navigation();


	m_pAIPlayers.clear();
	for (auto& iter : *pPlayerLists)
	{
		if (iter == m_pActivePlayer)
			continue;
		m_pAIPlayers.push_back(dynamic_cast<CPlayer*>(iter));
		dynamic_cast<CPlayer*>(iter)->AI_check();
	}
}

void CPlayerManager::Save_LastPosition()
{
	m_vLastPos = m_pActivePlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
}

PLAYER_MODE CPlayerManager::Check_ActiveMode(CPlayer * pPlayer)
{
	if (pPlayer == nullptr)
		return ACTIVE;

	if (m_bBattleMode == true)
	{
		return pPlayer == m_pActivePlayer ? ACTIVE : AI_MODE;
	}
	else
		return pPlayer == m_pActivePlayer ? ACTIVE : UNVISIBLE;

	return ACTIVE;
}

CPlayer * CPlayerManager::Get_EnumPlayer(_uint eID)
{
	list<CGameObject*>* pPlayerLists = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));
	for (auto& iter : *pPlayerLists)
	{
		if (eID == dynamic_cast<CPlayer*>(iter)->Get_PlayerID())
			return dynamic_cast<CPlayer*>(iter);
	}
	
	return nullptr;
}

void CPlayerManager::Set_Ai_Check()
{

	for (auto& iter : m_pAIPlayers)
	{
		iter->AI_check();
	}
	
}

void CPlayerManager::Set_SmashAttack()
{
	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return;

	

	if (dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage >= 4.f)
	{
		
		if (CGameInstance::Get_Instance()->Key_Down(DIK_1) && CGameInstance::Get_Instance()->Key_Down(DIK_2))
		{
			HANDLE hFile = 0;
			_ulong dwByte = 0;
			NONANIMDESC Active1;
			NONANIMDESC Active2;
			NONANIMDESC AIplayer1;
			NONANIMDESC AIplayer2;
			NONANIMDESC Monster;
			_uint iNum = 0;

			hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Strike_Position.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			/* 타일의 개수 받아오기 */
			ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

			ReadFile(hFile, &(Active1), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(Active2), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(AIplayer1), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(AIplayer2), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(Monster), sizeof(NONANIMDESC), &dwByte, nullptr);
			CloseHandle(hFile);

			CBattleManager::Get_Instance()->Get_LackonMonster()->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Monster.vPosition), 1.f));

			Get_EnumPlayer(0)->Set_IsActionMode(true); //원래는 액션카메라가 돌 때 자동적으로 멈추는데, 얘만 애니메이션을 돌리려고 true 한거임
			Get_EnumPlayer(0)->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active1.vPosition), 1.f));
			Get_EnumPlayer(0)->Get_Transform()->LookAt( XMVectorSetW(XMLoadFloat3(&Monster.vPosition),1.f));
			Get_EnumPlayer(0)->SmashAttack(CPlayer::ALPHEN_SION);

			Get_EnumPlayer(1)->Set_IsActionMode(true);
			Get_EnumPlayer(1)->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active2.vPosition), 1.f));
			Get_EnumPlayer(1)->Get_Transform()->LookAt(XMVectorSetW(XMLoadFloat3(&Monster.vPosition), 1.f));
			Get_EnumPlayer(1)->SmashAttack(CPlayer::ALPHEN_SION);

			Get_EnumPlayer(2)->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer1.vPosition), 1.f));
			Get_EnumPlayer(2)->Get_Transform()->LookAt(XMVectorSetW(XMLoadFloat3(&Monster.vPosition), 1.f));

			/*Get_EnumPlayer(3)->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer2.vPosition), 1.f));
			Get_EnumPlayer(3)->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMVectorSetW(XMLoadFloat3(&Monster.vPosition), 1.f));*/
			CCameraManager* pCameraManager = CCameraManager::Get_Instance();
			pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
			pCameraManager->Play_ActionCamera(TEXT("Striketest.dat"), XMMatrixIdentity());

			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
		}
		
		
		if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 2)
		{
			if (CGameInstance::Get_Instance()->Key_Down(DIK_1) && CGameInstance::Get_Instance()->Key_Down(DIK_3))
			{
				Get_EnumPlayer(0)->SmashAttack(CPlayer::ALPHEN_RINWELL);
				Get_EnumPlayer(2)->SmashAttack(CPlayer::ALPHEN_RINWELL);
				dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
			}

			else if (CGameInstance::Get_Instance()->Key_Down(DIK_2) && CGameInstance::Get_Instance()->Key_Down(DIK_3))
			{
				Get_EnumPlayer(1)->SmashAttack(CPlayer::SION_RINWELL);
				Get_EnumPlayer(2)->SmashAttack(CPlayer::SION_RINWELL);
				dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
			}
		}

		 if (CPlayerManager::Get_Instance()->Get_AIPlayers().size() >= 3)
		{
		 if (CGameInstance::Get_Instance()->Key_Down(DIK_1) && CGameInstance::Get_Instance()->Key_Down(DIK_4))
		{
			Get_EnumPlayer(0)->SmashAttack(CPlayer::ALPHEN_LAW);
			Get_EnumPlayer(3)->SmashAttack(CPlayer::ALPHEN_LAW);
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
		}

		else if (CGameInstance::Get_Instance()->Key_Down(DIK_2) && CGameInstance::Get_Instance()->Key_Down(DIK_4))
		{
			Get_EnumPlayer(1)->SmashAttack(CPlayer::SION_LAW);
			Get_EnumPlayer(3)->SmashAttack(CPlayer::SION_LAW);
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
		}
		else if (CGameInstance::Get_Instance()->Key_Down(DIK_3) && CGameInstance::Get_Instance()->Key_Down(DIK_4))
		{
			Get_EnumPlayer(1)->SmashAttack(CPlayer::RINWELL_LAW);
			Get_EnumPlayer(4)->SmashAttack(CPlayer::RINWELL_LAW);
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Reset_Lockonguage();
		}
	   }
		
		
	}


}

void CPlayerManager::Free()
{

}
