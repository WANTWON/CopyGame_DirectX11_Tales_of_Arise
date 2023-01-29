#pragma once

#include "BerserkerState.h"

BEGIN(Client)
BEGIN(Berserker)
class CBattle_Damage_LargeB_State : public CBerserkerState
{


public:
	CBattle_Damage_LargeB_State(class CBerserker* pBerserker, _bool bOnAngry = false);

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
	_bool				m_bAngry = false;
	EMOTION_STATE	    m_eEmotion = EMO_CALM;
	
private:
	_matrix m_StartMatrix;
};
END
END