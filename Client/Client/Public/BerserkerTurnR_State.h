#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CTurnR_State : public CBerserkerState
{
public:
	CTurnR_State(class CBerserker* pBerserker);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTurnR_Time = 0.f;
	_float m_fWalkMoveTimer = 0.f;
	_uint  m_iRand = 0;
};
END
END