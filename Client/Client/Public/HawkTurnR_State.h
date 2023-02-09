#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CTurn_State : public CHawkState
{
public:
	CTurn_State(class CHawk* pIceWolf);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fTurnR_Time = 0.f;
	_uint  m_iRand = 0;

};
END
END