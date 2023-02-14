#pragma once

#include "AIState.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAI_DodgeState final : public CAIState
{
public:
	CAI_DodgeState(class CPlayer* pPlayer, CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;




};
END
END