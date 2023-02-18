#include "stdafx.h"
#include "..\Public\PlayerManager.h"
#include "BaseObj.h"
#include "Player.h"
#include "GameInstance.h"

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

void CPlayerManager::Free()
{

}
