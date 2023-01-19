#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CTurnLeftState final : public CHawkState
{
public:
	CTurnLeftState(class CIce_Wolf* pIceWolf);
	
	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTimeDletaAcc = 0.f;
	_float m_fTurnLeftMoveTimer = 0.f;
	_float m_fTurnLeftAttackTimer = 1.5f;
	_uint  m_iRand = 0;
	_uint  m_iRadian = 0;
	_float m_fRadian = 0.f;
};

END
END