#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAIPoseState final : public CAIState
{
public:
	CAIPoseState(class CPlayer* pPlayer, STATE_ID eStateType);
	
	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter(void) override;
	virtual void Exit(void) override;
};
END
END