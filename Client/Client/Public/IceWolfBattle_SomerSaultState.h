#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_SomerSaultState : public CIceWolfState
{
public:
	CBattle_SomerSaultState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fRedayAttackTimer = 0.f;
	_bool		m_bTargetSetting = false;
private:
	_matrix m_StartMatrix;
};

END
END