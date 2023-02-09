#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)
class CBattleWalkState final : public CAstralDoubt_State
{
public:
	CBattleWalkState(class CAstralDoubt* pIceWolf, FIELD_STATE_ID ePreState);

	virtual CAstralDoubt_State* AI_Behaviour(_float fTimeDelta) override;
	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta);

	virtual void Enter() override;
	virtual void Exit() override;

	


private:
	_float m_fWalkMoveTimeAcc = 0.f;
	_float  m_fMoveTime = 0;
	FIELD_STATE_ID	m_ePreState_Id;


};
END
END