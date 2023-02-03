#include "stdafx.h"
#include "..\Public\BattleManager.h"
#include "GameInstance.h"
#include "BaseObj.h"
#include "PlayerManager.h"

IMPLEMENT_SINGLETON(CBattleManager);


CBattleManager::CBattleManager()
{
	m_FightedMonster.reserve(0);
}


void CBattleManager::Update_LockOn()
{
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();

	list<CGameObject*>* pMonsterList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_BATTLE, TEXT("Layer_Monster"));
	if (pMonsterList == nullptr || pMonsterList->size() == 1)
	{
		Set_LackonMonster(nullptr);
		return;
	}

	for (auto& iter : *pMonsterList)
	{
		if (iter == m_pLockon)
			continue;
		Set_LackonMonster(dynamic_cast<CBaseObj*>(iter));
		return;

	}

}

void CBattleManager::Free()
{
}
