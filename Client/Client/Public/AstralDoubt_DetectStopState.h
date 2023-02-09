#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CDetectStop_State : public CAstralDoubt_State
{
public:
	CDetectStop_State(class CAstralDoubt* pAstralDoubt);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fRandTime = 0.f;
	_float m_fWalkMoveTimer = 0.f;
	_uint  m_iRand = 0;
};
END
END