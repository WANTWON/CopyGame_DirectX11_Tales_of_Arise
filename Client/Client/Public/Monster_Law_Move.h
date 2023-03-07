#pragma once

#include "MonsterLawState.h"

BEGIN(Client)
BEGIN(MonsterLaw)
class CMonster_Law_Move final : public CMonsterLawState
{
public:
	CMonster_Law_Move(class CMonsterLaw* pPlayer);

	virtual CMonsterLawState* Tick(_float fTimeDelta) override;
	virtual CMonsterLawState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

	//void ChaseTarget(_float timedelta);



private:
	_bool m_bStopRunning = false;
};
END
END