#include "stdafx.h"
#include "..\Public\BattleManager.h"
#include "GameInstance.h"
#include "BaseObj.h"
#include "PlayerManager.h"
#include "Monster.h"

IMPLEMENT_SINGLETON(CBattleManager);


CBattleManager::CBattleManager()
{
	m_FightedMonster.reserve(0);
}


void CBattleManager::Set_BattleMode(_bool type, MONSTER_ID eMonsterType, _bool IsBossBattle)
{
	 m_isBattleMode = type; 
	 if (m_eMonsterType != eMonsterType)
	 {
		 m_eMonsterType = eMonsterType;
		 m_FightedMonster.push_back(eMonsterType);
	}
	 m_bIsBossBattle = IsBossBattle;
}

void CBattleManager::Out_Monster(CBaseObj * pMonster)
{
	for (auto& iter = m_FieldMonster.begin(); iter != m_FieldMonster.end();)
	{
		if ((*iter) != nullptr)
		{
			if ((*iter) == pMonster)
			{
				iter = m_FieldMonster.erase(iter);
				return;
			}
			else
				iter++;
		}
	}
}

void CBattleManager::Out_BattleMonster(CBaseObj * pMonster)
{
	for (auto& iter = m_BattleMonster.begin(); iter != m_BattleMonster.end();)
	{
		if ((*iter) != nullptr)
		{
			if ((*iter) == pMonster)
			{
				iter = m_BattleMonster.erase(iter);
			}
			else
				iter++;
		}
	}
}

void CBattleManager::Return_AllPoolingMonster_AtClear()
{
	for (auto& iter = m_FieldMonster.begin(); iter != m_FieldMonster.end();)
	{
		CGameInstance::Get_Instance()->ReAdd_GameObject(LEVEL_BATTLE, TEXT("Layer_Pooling"), *iter);
		iter = m_FieldMonster.erase(iter);
	}
}

CBaseObj * CBattleManager::Get_MinDistance_Monster(_vector vPosition)
{
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();

	CBaseObj* pTarget = nullptr;
	_float fMinDistance = MAXDISTANCE;
	for (auto& iter : m_BattleMonster)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0 || iter == pLockOn)
			continue;
		
		_float fDistance = XMVectorGetX(XMVector3Length(vPosition - dynamic_cast<CMonster*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			pTarget = dynamic_cast<CBaseObj*>(iter);
		}
	}

	if (pTarget == nullptr)
		return pLockOn;
	else
	return pTarget;
}

//CBaseObj * CBattleManager::Get_MinDistance_Monster_includeLockon(_vector vPosition)
//{
//	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
//
//	CBaseObj* pTarget = nullptr;
//	_float fMinDistance = MAXDISTANCE;
//	for (auto& iter : m_BattleMonster)
//	{
//		if (dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0)
//			continue;
//
//		_float fDistance = XMVectorGetX(XMVector3Length(vPosition - dynamic_cast<CMonster*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));
//
//		if (fDistance < fMinDistance)
//		{
//			fMinDistance = fDistance;
//			pTarget = dynamic_cast<CBaseObj*>(iter);
//		}
//	}
//	return pTarget;
//}

_bool CBattleManager::IsAllMonsterDead()
{
	if (m_BattleMonster.size() <= 0)
		return true;

	for (auto& iter : m_BattleMonster)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0)
			continue;
		else
			return false;

	}
	
	return true;
}

void CBattleManager::Update_LockOn()
{
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();

	if (m_BattleMonster.size() <= 1)
	{
		Set_LackonMonster(nullptr);
		return;
	}

	for (auto& iter : m_BattleMonster)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Dissolve() || dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0)
			continue;
		Set_LackonMonster(dynamic_cast<CBaseObj*>(iter));
		return;

	}
	Set_LackonMonster(nullptr);
	return;

}

void CBattleManager::Free()
{
	
	 m_FieldMonster.clear();
	 m_BattleMonster.clear();

}
