#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_TurnState : public CIceWolfState
{
public:
	CBattle_TurnState(class CIce_Wolf* pIceWolf, STATE_ID ePreState = STATE_END, CBaseObj* pCurTarget = nullptr);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint			m_iRand = 0;
	_float			m_fRedayAttackTimer = 0.f;
	_bool			m_bTargetSetting = false;
	STATE_ID		m_ePreState;
	_float			m_fIdleTime;


};

END
END