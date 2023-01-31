#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CIdleState : public CBerserkerState
{
public:
	CIdleState(class CBerserker* pBerserker, FIELD_STATE_ID ePreState);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
	_uint  m_iRand = 0;
	FIELD_STATE_ID	m_ePreState_Id;
};
END
END