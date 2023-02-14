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
	void Set_BattleMode(_bool type, MONSTER_ID eMonsterType = MONSTER_END, _bool IsBossBattle = false);
	void Add_Monster(CBaseObj* pMonster) { m_FieldMonster.push_back(pMonster); }
	void Out_Monster(CBaseObj* pMonster); 

	void Add_BattleMonster(CBaseObj* pMonster) { m_BattleMonster.push_back(pMonster); }
	void Out_BattleMonster(CBaseObj* pMonster);

public: /* Getter */
	_bool Get_IsBattleMode() { return m_isBattleMode; }
	_bool Get_IsBossBattle() { return m_bIsBossBattle; }
	MONSTER_ID Get_MonsterType() { return m_eMonsterType; }
	vector<MONSTER_ID> Get_FightedMonster() { return m_FightedMonster; }
	vector<CBaseObj*> Get_AllMonster() { return m_FieldMonster; }
	vector<CBaseObj*> Get_BattleMonster() { return m_BattleMonster; }
	CBaseObj* Get_MinDistance_Monster(_vector vPosition);

	_bool IsAllMonsterDead();

public: /* For Lock On */
	void Set_LackonMonster(CBaseObj* pMonster) { m_pLockon = pMonster; }
	void Set_BossMonster(CBaseObj* pMonster) { m_pBoss = pMonster; }
	CBaseObj* Get_LackonMonster() { return m_pLockon; }
	CBaseObj* Get_BossMonster() { return m_pBoss; }
	void Update_LockOn();

public:
	void Clear_Monster() { m_FieldMonster.clear(); };

private:
	_bool	   m_isBattleMode = false;
	_bool	   m_bIsBossBattle = false;
	MONSTER_ID m_eMonsterType = MONSTER_END;
	vector<MONSTER_ID> m_FightedMonster;
	vector<CBaseObj*> m_FieldMonster;
	vector<CBaseObj*> m_BattleMonster;
	
	CBaseObj* m_pLockon = nullptr;
	CBaseObj* m_pBoss = nullptr;

public:
	virtual void Free() override;
};
END

