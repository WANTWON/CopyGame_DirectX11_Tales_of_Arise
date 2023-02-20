#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_Damage_LargeB_State : public CBerserkerState
{


public:
	CBattle_Damage_LargeB_State(class CBerserker* pBerserker, _bool bAngry, _bool bBerserkerMode, _bool bEvadeChance, STATE_ID eStateId = STATE_ID::STATE_END);

	virtual CBerserkerState* AI_Behaviour(_float fTimeDelta) override;
	virtual CBerserkerState* Tick(_float fTimeDelta) override;
	virtual CBerserkerState* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_float				m_fIdleMoveTimer = 0.f;
	_float				m_fIdleAttackTimer = 1.5f;
	_uint				m_iRand = 0;
	_bool				m_bAnimFinish = false;
	_bool				m_bBerserkerMode = false;
	EMOTION_STATE	    m_eEmotion = EMO_CALM;
	_bool				m_bEvadeChance = false;
	STATE_ID			m_eStateId = STATE_ID::STATE_END;
private:
	_matrix m_StartMatrix;
};
END
END