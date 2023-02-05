#pragma once

#include "AIState.h"
#include "Sion.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAIRunawayfromTarget final : public CAIState
{
public:
	CAIRunawayfromTarget(class CPlayer* pPlayer, STATE_ID eStateType ,_uint playerid);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool m_bStopRunning = false;
};
END
END