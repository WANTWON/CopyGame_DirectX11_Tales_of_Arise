#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CTurnRightState final : public CIceWolfState
{
public:
	CTurnRightState(class CIce_Wolf* pIceWolf);
	
	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTimeDletaAcc = 0.f;
	_float m_fTurnRightTime = 0.f;
	_uint  m_iRand = 0;
	_uint  m_iRadian = 0;
	_float m_fRadian = 0.f;
};

END
END