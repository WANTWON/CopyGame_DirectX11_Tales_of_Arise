#pragma once

#include "AIState.h"
#include "Sion.h"

BEGIN(Client)
BEGIN(AIPlayer)
class CAIAttackNormalState final : public CAIState
{
public:
	CAIAttackNormalState(class CPlayer* pPlayer , STATE_ID eStateType ,CBaseObj* pTarget);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool m_bIsStateEvent = false;
};
END
END