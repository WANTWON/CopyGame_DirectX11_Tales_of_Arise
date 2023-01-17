#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CWalkFrontState final : public CIceWolfState
{
public:
	CWalkFrontState(class CIce_Wolf* pIceWolf);

	virtual CIceWolfState* AI_Behaviour() override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;



private:
	_float m_fWalkMoveTimer = 0.f;
	_float m_fWalkAttackTimer = 1.5f;
	_uint  m_iRand = 0;
};
END
END