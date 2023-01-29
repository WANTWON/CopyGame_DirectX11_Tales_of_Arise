#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CHowLing_State : public CBerserkerState
{
public:
	CHowLing_State(class CBerserker* pIceWolf);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
	_float m_fWalkMoveTimer = 0.f;
	_uint  m_iRand = 0;
};
END
END