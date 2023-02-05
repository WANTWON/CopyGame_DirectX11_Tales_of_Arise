#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_ChaseState final : public CAIState
{
public:
	CAI_ChaseState(class CPlayer* pPlayer, STATE_ID eStateType, _uint playerid, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

	void ChaseTarget(_float timedelta);
	
	

private:
	_bool m_bStopRunning = false;
};
END
END