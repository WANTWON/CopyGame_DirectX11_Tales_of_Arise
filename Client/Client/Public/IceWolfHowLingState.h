#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CHowLingState : public CHawkState
{
public:
	CHowLingState(class CIce_Wolf* pIceWolf);
	
	virtual CHawkState* AI_Behaviour(_float fTimeDelta) override;
	virtual CHawkState* Tick(_float fTimeDelta) override;
	virtual CHawkState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fHowLingMoveTimer = 0.f;
	_float m_fHowLingAttackTimer = 1.5f;
	_uint  m_iRand = 0.f;

};
END
END
