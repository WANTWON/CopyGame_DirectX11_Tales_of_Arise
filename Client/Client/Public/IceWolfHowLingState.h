#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CHowLingState : public CIceWolfState
{
public:
	CHowLingState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fHowLingMoveTimer = 0.f;
	_float m_fHowLingAttackTimer = 1.5f;
	_uint  m_iRand = 0;

};
END
END
