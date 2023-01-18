#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_IdleState : public CHawkState
{
public:
	CBattle_IdleState(class CIce_Wolf* pIceWolf);
	
	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0.f;
	_float		m_fIdleAttackTimer = 0.f;
	_bool		m_bAnimFinish = false;
};

END
END