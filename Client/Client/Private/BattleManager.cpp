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
	 m_eMonsterType = eMonsterType; 
	 m_FightedMonster.push_back(eMonsterType);
	 m_bIsBossBattle = IsBossBattle;
}

CBaseObj * CBattleManager::Get_MinDistance_Monster(_vector vPosition)
{
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();

	list<CGameObject*>* pMonsterList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_BATTLE, TEXT("Layer_Monster"));
	
	CBaseObj* pTarget = nullptr;
	_float fMinDistance = MAXDISTANCE;
	for (auto& iter : *pMonsterList)
	{
		if (dynamic_cast<CMonster*>(iter)->Get_Dissolve() || dynamic_cast<CMonster*>(iter)->Get_Stats().m_fCurrentHp <= 0)
			continue;
		
		_float fDistance = XMVectorGetX(XMVector3Length(vPosition - dynamic_cast<CMonster*>(iter)->Get_TransformState(CTransform::STATE_TRANSLATION)));

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			pTarget = dynamic_cast<CBaseObj*>(iter);
		}
	}
	return pTarget;
}

void CBattleManager::Update_LockOn()
{
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();

	list<CGameObject*>* pMonsterList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_BATTLE, TEXT("Layer_Monster"));
	if (pMonsterList == nullptr || pMonsterList->size() <= 1)
	{
		Set_LackonMonster(nullptr);
		return;
	}

	for (auto& iter : *pMonsterList)
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
}
