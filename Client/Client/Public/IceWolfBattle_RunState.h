#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_RunState : public CIceWolfState
{
public:
	CBattle_RunState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	
	_float		m_fTarget_Distance;
	_uint		m_iRand = 0;
	_float		m_fIdleAttackTimer = 0.f;
	
};

END
END