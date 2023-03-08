#pragma once

#include "IceWolfState.h"

BEGIN(Client)
BEGIN(IceWolf)

class CBattle_Damage_LargeB_State : public CIceWolfState
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
	CBattle_Damage_LargeB_State(class CIce_Wolf* pIceWolf, STATE_ID StateId, HITTYPE eType, _vector vCauserPos, _float fMoveLength);

	virtual CIceWolfState* Tick(_float fTimeDelta) override;
	virtual CIceWolfState* LateTick(_float fTimeDelta) override;

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
	_bool			m_bDowning = false;
	CCollider*		m_pAtkColliderCom = nullptr;
	HITTYPE			m_eHitType = HIT_END;
	_vector			m_vCauserPos;
	_float			m_fTime = 0.f;
	_float			m_fMoveLength = 0.f;

private:
	_matrix m_StartMatrix;

private:
	void Move(_float fTimeDelta);
};

END
END