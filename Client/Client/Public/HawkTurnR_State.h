#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CTurnR_State : public CHawkState
{
public:
	CTurnR_State(class CHawk* pIceWolf);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
	_uint  m_iRand = 0;
};
END
END