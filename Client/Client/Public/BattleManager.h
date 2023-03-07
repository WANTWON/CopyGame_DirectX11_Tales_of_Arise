#pragma once
#include "Base.h"
#include "Client_Defines.h"
//#include "AIState.h"

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
	void Return_AllPoolingMonster_AtClear();

public: /* Getter */
	_bool Get_IsBattleMode() { return m_isBattleMode; }
	_bool Get_IsBossBattle() { return m_bIsBossBattle; }
	MONSTER_ID Get_MonsterType() { return m_eMonsterType; }
	vector<MONSTER_ID> Get_FightedMonster() { return m_FightedMonster; }
	vector<CBaseObj*> Get_AllMonster() { return m_FieldMonster; }
	vector<CBaseObj*> Get_BattleMonster() { return m_BattleMonster; }
	CBaseObj* Get_MinDistance_Monster(_vector vPosition);

	_bool Get_IsStrike() { return m_bStrike; }
	void Set_IsStrike(_bool tof) { m_bStrike = tof; }

	_bool IsAllMonsterDead();

public: /* For Lock On */
	void Set_LackonMonster(CBaseObj* pMonster) { m_pLockon = pMonster; }
	void Set_BossMonster(CBaseObj* pMonster) { m_pBoss = pMonster; }
	CBaseObj* Get_LackonMonster() { return m_pLockon; }
	CBaseObj* Get_BossMonster() { return m_pBoss; }
	void Update_LockOn();

	_int Get_AImode(){return m_eAImode;}
	void Set_AImode(_int mode) { m_eAImode = mode; }

	_bool Get_AIuseItem() { return m_bAIuseItem; }
	void Set_AIuseItem(_bool tof) { m_bAIuseItem = tof; }

	_bool Get_IsOneonOneMode() { return m_bIsOneonOneMode; }
	void Set_OneonOneMode(_bool tof) { m_bIsOneonOneMode = tof; }

	_bool Get_IsHitLeg() { return m_bHitLeg; }
	void Set_IsHitLeg(_bool tof) { m_bHitLeg = tof; }
	_float Get_HitLegTimer() { return m_fHitLegTimer; }
	void Set_HitLegTimer(_float fTime) { m_fHitLegTimer = fTime; }

	_int Get_LawBattlePhase() { return m_iLawBattelPhase; }
	void Set_LawBattlePhase(_int phase) { m_iLawBattelPhase = phase; }

public:
	void Clear_Monster() { m_FieldMonster.clear(); };
	void Clear_AllMonster() { m_FieldMonster.clear(); };

private:
	_bool	   m_isBattleMode = false;
	_bool	   m_bIsBossBattle = false;
	MONSTER_ID m_eMonsterType = MONSTER_END;
	vector<MONSTER_ID> m_FightedMonster;
	vector<CBaseObj*> m_FieldMonster;
	vector<CBaseObj*> m_BattleMonster;
	
	CBaseObj* m_pLockon = nullptr;
	CBaseObj* m_pBoss = nullptr;
	_bool m_bStrike = false;

	_int m_eAImode = 0;

	_bool m_bAIuseItem = false;
	//CAIState::AI_MODE m_eAImode = ATTACK_FREEMODE;

	//For Only LawbattleMode == (1:1 BattleMode)
	_bool m_bIsOneonOneMode = false;
	
	_bool m_bHitLeg = false;
	_float m_fHitLegTimer = 0.f;

	_int m_iLawBattelPhase = 0;


public:
	virtual void Free() override;
};
END

