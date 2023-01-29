#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CIdleState : public CHawkState
{
public:
	CIdleState(class CHawk* pIceWolf, FIELD_STATE_ID ePreState);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float			m_fIdleMoveTimer = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	_uint			m_iRand = 0;
	FIELD_STATE_ID	m_ePreState_Id;
};
END
END