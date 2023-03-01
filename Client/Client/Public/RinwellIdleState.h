#pragma once

#include "RinwellState.h"

BEGIN(Client)
BEGIN(AiRinwell)

class CRinwellIdleState final :	public CRinwellState
{
public:
	CRinwellIdleState(class CAiRinwell* pRinwell);
	
	virtual CRinwellState* Tick(_float fTimeDelta) override;
	virtual CRinwellState* LateTick(_float fTimeDelta);

	virtual void Enter(void) override;
	virtual void Exit(void) override;

private:
	_bool	m_bAirMove = false;
	_float	m_fTime = 0.f;
};

END
END