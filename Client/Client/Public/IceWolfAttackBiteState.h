#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CAttackBiteState : public CIceWolfState
{
public:
	CAttackBiteState(class CIce_Wolf* pIceWolf, CBaseObj* pCurTarget = nullptr);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float		m_fTarget_Distance;
	_uint		 m_iRand = 0;
	_uint		m_iCollisionCount = 0;
	_float		m_fIdleAttackTimer = 0.f;
	_bool		m_bTargetSetting = false;
	_bool		m_bCollision = false;
private:
	_matrix m_StartMatrix;
};

END
END