#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattle_HeadBeamState : public CAstralDoubt_State
{
public:
	CBattle_HeadBeamState(class CAstralDoubt* pAstral_Doubt, STATE_ID ePreState = STATE_ID::STATE_IDLE);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;
	
	virtual void Enter() override;
	virtual void Exit() override;

private:
	void AimTarget(_float fTimeDelta);

private:
	_float			m_fBeamTimeDeltaAcc = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	STATE_ID	m_ePreState_Id;
	STATE_ID	m_ePreTurn_Id;

	_float		m_fIdleTime;
	_float		m_fDot;

	_bool		m_bUpdatTargetPos = false;
	_bool		m_bStartTargetPos = false;
	_bool		m_bIs_TargetInRight = false;

	_vector		m_vTargetPos;
	_vector     m_vTargetUpdatPos;
	_vector		m_vTargetDir;
	_vector		m_vLook;


	CTransform* m_pMonSterTransform;


};
END
END