#pragma once

#include "AIState.h"

BEGIN(Client)
class CEffect;
BEGIN(AIPlayer)
class CAI_Rinwell_Fascinated final : public CAIState
{
public:
	CAI_Rinwell_Fascinated(class CPlayer* pPlayer);

	virtual CAIState* Tick(_float fTimeDelta) override;
	virtual CAIState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool m_bStopRunning = false;

	ITEM_NAME m_eItemname = ITEMNAME_END;
	_float m_fEventStart = -1.f;
};
END
END