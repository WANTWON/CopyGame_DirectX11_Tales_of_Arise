#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CWalkState final : public CIceWolfState
{
public:
	CWalkState(class CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreTurn);

	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;



private:
	_float m_fWalkMoveTimer = 0.f;
	_float m_fWalkAttackTimer = 1.5f;
	_uint  m_iRand = 0;
	FIELD_STATE_ID	m_ePreState_Id;
	FIELD_STATE_ID	m_ePreTurn_Id;
};
END
END