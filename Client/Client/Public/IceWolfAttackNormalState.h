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
	_uint		m_iRand = 0;
	_float		m_fTarget_Distance;
	_float		m_fRandTime = 0.f;
	_bool		m_bTargetSetting = false;
	_bool		m_bAddGoDistance = false;
private:
	_matrix m_StartMatrix;
};

END
END