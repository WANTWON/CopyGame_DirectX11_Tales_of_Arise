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

public: /* Settter */
	void Set_BattleMode(_bool type, MONSTER_ID eMonsterType = MONSTER_END) { m_isBattleMode = type; m_eMonsterType = eMonsterType; m_FightedMonster.push_back(eMonsterType); }
	void Add_Monster(CBaseObj* pMonster) { m_AllMonster.push_back(pMonster); }

public: /* Getter */
	_bool Get_IsBattleMode() { return m_isBattleMode; }
	MONSTER_ID Get_MonsterType() { return m_eMonsterType; }
	vector<MONSTER_ID> Get_FightedMonster() { return m_FightedMonster; }
	vector<CBaseObj*> Get_AllMonster() { return m_AllMonster; }
	CBaseObj* Get_MinDistance_Monster(_vector vPosition);

public: /* For Lock On */
	void Set_LackonMonster(CBaseObj* pMonster) { m_pLockon = pMonster; }
	CBaseObj* Get_LackonMonster() { return m_pLockon; }
	void Update_LockOn();

public:
	void Clear_Monster() { m_AllMonster.clear(); };

private:
	_bool	   m_isBattleMode = false;
	MONSTER_ID m_eMonsterType = MONSTER_END;
	vector<MONSTER_ID> m_FightedMonster;
	vector<CBaseObj*> m_AllMonster;
	
	CBaseObj* m_pLockon = nullptr;

public:
	virtual void Free() override;
};
END

