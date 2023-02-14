#pragma once

#include "AstralDoubt_State.h"

BEGIN(Client)
BEGIN(Astral_Doubt)

class CBattle_Hit_AndDead : public CAstralDoubt_State
{
public:
	enum DAMAGE_ANIM
	{
		ANIM_DAMAGE_LARGE_B,
		ANIM_DAMAGE_LARGE_F,
		ANIM_DAMAGE_LARGE_L,
		ANIM_DAMAGE_LARGE_R,
		ANIM_DAMAGE_LOOP,
		ANIM_DAMAGE_SMALL_B,
		ANIM_DAMAGE_SMALL_F,
		ANIM_DAMAGE_SMALL_L,
		ANIM_DAMAGE_SMALL_R,

	};
public:
	CBattle_Hit_AndDead(class CAstralDoubt* pAstralDoubt, STATE_ID StateId, _bool bThirdHit = false);

	virtual CAstralDoubt_State* Tick(_float fTimeDelta) override;
	virtual CAstralDoubt_State* LateTick(_float fTimeDelta) override;

	virtual void Enter() override;
	virtual void Exit() override;

private:
	_uint			m_iRand = 0;
	_float			m_fIdleAttackTimer = 0.f;
	_float			m_fCntChance= 0.f;
	_float			m_fCntChanceTime = 0.f;
	DAMAGE_ANIM		m_eDamageAnim = ANIM_DAMAGE_LARGE_B;
	_bool			m_bThirdHit = false;
	_bool			m_bDeadAnimFinish = false;
	_bool			m_bDeadSound = false;
private:
	_matrix m_StartMatrix;
};

END
END