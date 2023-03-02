#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CRinwellDownState final : public CRinwellState
{
public:
	CRinwellDownState(class CAiRinwell* pRinwell);

	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;
};

END
END