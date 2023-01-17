#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CAttack_Elemental_Charge : public CIceWolfState
{
public:
	CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

public:

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool		m_bCharge_Loop_Finish = false;
	_float		m_fTarget_Distance;
	STATE_ID	m_eStateId_Charge = STATE_END;
	_vector		m_vCharge_End_BeforePos;
	_bool		m_bEntry_Charge_End = false;

	_uint		m_iRand = 0.f;


	_float		m_fIdleAttackTimer = 0.0f;
	_bool		m_bAnimFinish = false;
};

END
END