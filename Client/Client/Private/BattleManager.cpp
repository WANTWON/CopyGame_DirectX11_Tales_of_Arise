#include "stdafx.h"
#include "..\Public\BattleManager.h"


IMPLEMENT_SINGLETON(CBattleManager);


CBattleManager::CBattleManager()
{
	m_FightedMonster.reserve(0);
}

void CBattleManager::Free()
{
}
