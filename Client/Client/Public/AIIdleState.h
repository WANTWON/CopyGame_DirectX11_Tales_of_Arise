#pragma once

#include "AIState.h"
#include "Sion.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CIdleState final : public CAIState
{
public:
	CIdleState(class CPlayer* pPlayer, STATE_ID eStateType);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
END
END