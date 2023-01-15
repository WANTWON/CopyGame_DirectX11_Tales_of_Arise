#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CIdleState : public CIceWolfState
{
public:
	CIdleState(class CIce_Wolf* pIceWolf);

	virtual CIceWolfState* AI_Behaviour() override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float fTimer = 0.f;
};
END
END