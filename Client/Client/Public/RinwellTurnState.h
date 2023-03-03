#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)
class CRinwellTurnState final : public CRinwellState
{
public:
	CRinwellTurnState(class CAiRinwell* pRinwell, STATE_ID eStateID, STATETYPE eStateType = STATETYPE_DEFAULT);
	
	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	STATE_ID m_eNextID;
	STATETYPE m_eNextStateType;

	_float m_fRatio = 0.f;
};
END
END