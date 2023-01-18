#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_WalkState : public CHawkState
{
public:
	CBattle_WalkState(class CIce_Wolf* pIceWolf);
	
	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	
	_float		m_fTarget_Distance;
	_uint		m_iRand = 0;
	_float		m_fIdleAttackTimer = 0.f;
	
};

END
END