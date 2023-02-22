#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_Double_ClawState : public CBerserkerState
{
public:
	CBattle_Double_ClawState(class CBerserker* pBerserker);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;

	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = nullptr;

private:
	_matrix m_StartMatrix;
};
END
END