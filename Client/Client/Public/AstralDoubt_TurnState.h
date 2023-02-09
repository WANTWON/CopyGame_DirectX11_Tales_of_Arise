#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CTurnState : public CAstralDoubt_State
{
public:
	CTurnState(class CAstralDoubt* pAstralDoubt);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float m_fTurnR_Time = 0.f;
	_float m_fRandTime = 0.f;
	_uint  m_iRand = 0;
};
END
END