#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattle_IdleState : public CAstralDoubt_State
{
public:
	CBattle_IdleState(class CAstralDoubt* pAstral_Doubt, STATE_ID ePreState);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	//_float			m_fTimeDeltaAcc = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	_float			m_fToTargetDot_Begin = 0.f;
	_float			m_fToTargetDot_Later = 0.f;
	STATE_ID		m_ePreState_Id;
	FIELD_STATE_ID	m_ePreTurn_Id;

	_float			m_fRandTime;
};
END
END