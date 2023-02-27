#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_Shock_WaveState : public CBerserkerState
{
public:
	CBattle_Shock_WaveState(class CBerserker* pBerserker);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;
	_float		m_fAtkCollision_Delay = 0.f;
	_float		m_f2th_AtkCollision_Delay = 0.f;

	_bool		m_bCollision = false;
	_bool		m_b2th_Collision = false;
	_bool		m_bAnimFinish = false;
	_bool		m_bTargetSetting = false;

	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = nullptr;
private:
	_matrix m_StartMatrix;
};
END
END