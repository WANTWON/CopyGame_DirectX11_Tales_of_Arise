#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CWalkState : public CHawkState
{
public:
	CWalkState(class CHawk* pIceWolf, _bool bTriggerTurn = false);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
	_float m_fMoveTime = 0.f;
	
	_uint  m_iRand = 0;
	_bool  m_bTriggerTurn = false;
};
END
END