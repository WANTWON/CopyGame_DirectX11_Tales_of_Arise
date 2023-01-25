#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CAttackNormalState : public CIceWolfState
{
public:
	CAttackNormalState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fTarget_Distance;
	_uint		 m_iRand = 0;
	_float		m_fIdleAttackTimer = 0.f;
	_bool		m_bAnimFinish = false;

private:
	_matrix m_StartMatrix;
};

END
END