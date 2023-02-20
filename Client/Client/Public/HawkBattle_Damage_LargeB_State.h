#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CBattle_Damage_LargeB_State : public CHawkState
{
public:
	CBattle_Damage_LargeB_State(class CHawk* pHawk, STATE_ID eStateId);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_float		m_fTarget_Distance;
	STATE_ID	m_eState_Id = STATE_END;
private:
	_matrix m_StartMatrix;
};
END
END