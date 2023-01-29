#pragma once

#include "SlimeState.h"

BEGIN(Client)
BEGIN(Slime)
class CIdleState : public CSlimeState
{
public:
	CIdleState(class CSlime* pSlime, FIELD_STATE_ID ePreState);

	virtual CSlimeState* AI_Behaviour(_float fTimeDelta) override;
	virtual CSlimeState* Tick(_float fTimeDelta) override;
	virtual CSlimeState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float			m_fIdleMoveTimer = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	_uint			m_iRand = 0;
	_float			m_fTarget_Distance;
	_float			m_fRedayAttackTimer = 0.f;
	FIELD_STATE_ID	m_ePreState_Id;
private:
	_matrix m_StartMatrix;
};
END
END