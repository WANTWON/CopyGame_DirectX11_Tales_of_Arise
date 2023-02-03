#include "stdafx.h"
#include "..\Public\BattleManager.h"
#include "GameInstance.h"
#include "BaseObj.h"

IMPLEMENT_SINGLETON(CBattleManager);


CBattleManager::CBattleManager()
{
	m_FightedMonster.reserve(0);
}


void CBattleManager::Free()
{
}
