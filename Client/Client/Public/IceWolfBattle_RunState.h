#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_RunState : public CIceWolfState
{
public:
	CBattle_RunState(class CIce_Wolf* pIceWolf, STATE_ID ePreState, CBaseObj* pCurTarget = nullptr);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint		m_iRand = 0;
	_float		m_fTarget_Distance;
	_float		m_fMinTarget_Distance;
	_float		m_fRandTime = 0.f;
	_bool		m_bTargetSetting = false;
	_bool		m_bAttacknormal = false;
	_bool		m_b_IsTargetInsight = false;
	_bool		m_bStartTarget = false;
	STATE_ID	m_ePreState_Id;
	
};

END
END