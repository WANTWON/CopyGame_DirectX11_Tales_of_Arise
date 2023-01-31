#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)
class CWalkState final : public CIceWolfState
{
public:
	CWalkState(class CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState, _bool bTriggerTurn = false);

	virtual CIceWolfState* AI_Behaviour(_float fTimeDelta) override;
	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

	


private:
	_float m_fWalkMoveTimeAcc = 0.f;
	_float  m_fMoveTime = 0;
	FIELD_STATE_ID	m_ePreState_Id;
	FIELD_STATE_ID	m_ePreTurn_Id;
	_bool m_bTriggerTurn = false;
};
END
END