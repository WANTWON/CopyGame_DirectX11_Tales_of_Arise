#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CIdleState : public CIceWolfState
{
public:
	CIdleState(class CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState);

	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float			m_fIdleTimeAcc = 0.f;
	_float			m_fIdleAttackTimer = 1.5f;
	FIELD_STATE_ID	m_ePreState_Id;
	FIELD_STATE_ID	m_ePreTurn_Id;

	_float			m_fIdleTime;
};
END
END