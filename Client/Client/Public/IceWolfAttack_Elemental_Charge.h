#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CAttack_Elemental_Charge : public CIceWolfState
{
public:
	CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType, _bool bPreDownState, CBaseObj* pCurTarget = nullptr);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

public:

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_bool		m_bCharge_Loop_Finish = false;
	_float		m_fTarget_Distance = 0.f;
	_float		m_fOriginTarget_Distance = 0.f;
	STATE_ID	m_eStateId_Charge = STATE_END;
	_bool		m_bEntry_Charge_End = false;

	_uint		m_iRand = 0;


	_float		m_fIdleAttackTimer = 0.0f;
	_float		m_fRandTime = 0.f;
	_bool		m_bAnimFinish = false;
	_bool		m_bTargetSetting = false;
	_bool		m_bPreDownState = false;
	CBaseObj*	m_pCurTarget = nullptr;
	CCollider*  m_pAtkColliderCom = false;
private:
	_matrix m_StartMatrix;
};

END
END