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
	void Set_BattleMode(_bool type, MONSTER_ID eMonsterType = MONSTER_END) { m_isBattleMode = type; m_eMonsterType = eMonsterType; }
	_bool Get_IsBattleMode() { return m_isBattleMode; }
	MONSTER_ID Get_MonsterType() {return m_eMonsterType;}

private:
	_bool	   m_isBattleMode = false;
	MONSTER_ID m_eMonsterType = MONSTER_END;

public:
	virtual void Free() override;
};
END

