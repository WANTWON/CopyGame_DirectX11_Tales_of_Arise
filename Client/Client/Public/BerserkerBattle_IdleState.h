#pragma once

#include "BerserkerState.h"

BEGIN(Client)
class CEffect;
BEGIN(Berserker)
class CBattle_IdleState : public CBerserkerState
{
public:
	CBattle_IdleState(class CBerserker* pBerserker, STATE_ID eStateId = STATE_ID::STATE_END);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	void Update_Effects();
	void Remove_Effects();

private:
	_float		m_fIdleMoveTimer = 0.f;
	_float		m_fIdleAttackTimer = 1.5f;
	_uint		m_iRand = 0;
	_bool		m_bAnimFinish = false;
	_bool		m_bTargetSetting = false;
	_float		m_fRedayAttackTimer = 0.f;
	STATE_ID	m_eState_Id = STATE_ID::STATE_END;

	/* Effect */
	_bool m_bHowl = false;
	vector<CEffect*> m_Howl;
};
END
END