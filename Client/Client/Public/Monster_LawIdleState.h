#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)

class CMonster_LawIdleState final : public CMonsterLawState
{
public:
	CMonster_LawIdleState(class CMonsterLaw* pLaw);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_bool	m_bAirMove = false;
	_float	m_fTime = 0.f;
	_float	m_fWaitingTime = 0.f;

	
};

END
END