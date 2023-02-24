#pragma once

#include "Player.h"

BEGIN(Client)
class CState abstract
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_JUMP,

		STATE_NORMAL_ATTACK1,
		STATE_NORMAL_ATTACK2,
		STATE_NORMAL_ATTACK3,
		STATE_NORMAL_ATTACK4,
		STATE_NORMAL_ATTACK5,

		STATE_SKILL_ATTACK_E,
		STATE_SKILL_ATTACK_R,
		STATE_SKILL_ATTACK_F,
		STATE_SKILL_ATTACK_CE,
		STATE_SKILL_ATTACK_CR,
		STATE_SKILL_ATTACK_CF,

		STATE_SKILL_BOOST,
		STATE_OVERLIMIT,

		STATE_HIT,
		STATE_DEAD,
		STATE_CHASE,
		STATE_DODGE,
		STATE_POSE,
		STATE_END
	};

public:
	STATE_ID Get_StateID(void) { return m_eStateID; }
	_float Get_Time(void) { return m_fTime; }

public:
	virtual ~CState() {};

public:
	virtual CState* HandleInput(void) PURE;
	virtual CState* Tick(_float fTimeDelta) PURE;
	virtual CState* Late_Tick(_float fTimeDelta) PURE;
	virtual CState* EventInput(void) PURE;
	virtual void Exit(void) PURE;

public:
	virtual void Enter(void);

public:
	CState* ChangeState(CState* pCurrentState, CState* pNewState);

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateID = STATE_END;

	CPlayer* m_pOwner = nullptr;

	_bool m_bIsAnimationFinished = false;
	
	_bool m_bIsFly = false;
	_uint m_ePlayerID = 0;
	_float m_fTime = 0.f;
};
END