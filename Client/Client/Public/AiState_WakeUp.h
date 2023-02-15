#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAiState_WakeUp final : public CAIState
{
public:
	CAiState_WakeUp(class CPlayer* pPlayer);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;
};
END
END