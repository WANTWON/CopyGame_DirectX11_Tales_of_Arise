#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CDeadState final : public CAIState
{
public:
	CDeadState(class CPlayer* pPlayer);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
END
END