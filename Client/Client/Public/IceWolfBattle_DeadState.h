#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_DeadState : public CIceWolfState
{
public:
	CBattle_DeadState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fIdleAttackTimer = 0.f;
	_bool		m_bDeadAnimFinish = false;

private:
	_matrix m_StartMatrix;
};

END
END