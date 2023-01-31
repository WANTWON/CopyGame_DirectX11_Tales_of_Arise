#pragma once

#include "HawkState.h"

BEGIN(Client)
BEGIN(Hawk)
class CBattle_IdleState : public CHawkState
{
public:
	CBattle_IdleState(class CHawk* pHawk, STATE_ID ePreBattleState);

	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fNextMotionTimer = 0.f;
	_float		m_fStartBattleModeDelay = 0.f;
	_float		m_fRedayAttackTimer = 0.f;
	_float		m_fTarget_Distance;
	_bool		m_bTargetSetting = false;
	STATE_ID	m_ePreBattleState;
private:
	_matrix m_StartMatrix;
};
END
END