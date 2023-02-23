#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_WalkState : public CBerserkerState
{
public:
	CBattle_WalkState(class CBerserker* pBerserker);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;
	_bool		m_bAnimFinish = false;
	_bool		m_bTargetSetting = false;
	_float		m_fRedayAngryTimer = 0.f;
	_float		m_fTarget_Distance;
	_bool		m_b_IsTargetInsight = false;
private:
	_matrix m_StartMatrix;
};
END
END