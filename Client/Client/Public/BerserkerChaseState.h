#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CChaseState : public CBerserkerState
{
public:
	CChaseState(class CBerserker* pIceWolf);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fIdleMoveTimer = 0.f;
	_float m_fIdleAttackTimer = 1.5f;
	_float m_fTarget_Distance;
	_uint  m_iRand = 0;
};
END
END