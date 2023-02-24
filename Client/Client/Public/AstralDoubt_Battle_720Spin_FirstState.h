#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattle_720Spin_FirstState : public CAstralDoubt_State
{
public:
	CBattle_720Spin_FirstState(class CAstralDoubt* pAstral_Doubt, STATE_ID ePreState = STATE_ID::STATE_IDLE);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	//_float			m_fTimeDeltaAcc = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	STATE_ID	m_ePreState_Id;
	STATE_ID	m_ePreTurn_Id;

	_float			m_fIdleTime;

	CCollider*  m_pAtkColliderCom = nullptr;
	CCollider*	m_p2th_AtkColliderCom = false;
	CCollider*	m_p3th_AtkColliderCom = false;
	CCollider*	m_p4th_AtkColliderCom = false;
	CCollider*	m_p5th_AtkColliderCom = false;
	CCollider*	m_p6th_AtkColliderCom = false;
};
END
END