#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)

class CBaseObj;
class CBattleManager : public CBase
{
	DECLARE_SINGLETON(CBattleManager)
public:

private:
	explicit CBattleManager();
	virtual ~CBattleManager() = default;

public:
	void Set_BattleMode(_bool type, MONSTER_ID eMonsterType = MONSTER_END) { m_isBattleMode = type; m_eMonsterType = eMonsterType; m_FightedMonster.push_back(eMonsterType); }
	_bool Get_IsBattleMode() { return m_isBattleMode; }
	MONSTER_ID Get_MonsterType() {return m_eMonsterType;}
	vector<MONSTER_ID> Get_FightedMonster() {return m_FightedMonster;}
private:
	_bool	   m_isBattleMode = false;
	MONSTER_ID m_eMonsterType = MONSTER_END;
	vector<MONSTER_ID> m_FightedMonster;

public:
	virtual void Free() override;
};
END

