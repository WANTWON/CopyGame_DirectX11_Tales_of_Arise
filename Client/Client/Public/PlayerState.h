#pragma once

#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Weapon.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)
class CPlayerState
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_JUMP,
		STATE_NORMAL_ATTACK1,
		STATE_NORMAL_ATTACK2,
		STATE_NORMAL_ATTACK3,
		STATE_NORMAL_ATTACK4,
		STATE_SKILL_ATTACK1,
		STATE_SKILL_ATTACK2,
		STATE_SKILL_ATTACK3,
		STATE_COLLECT,
		STATE_END
	};

	STATE_ID Get_StateId() { return m_eStateId; }

	virtual ~CPlayerState() {};
	virtual CPlayerState* HandleInput() { return nullptr; };
	virtual CPlayerState* Tick(_float fTimeDelta) PURE;
	virtual CPlayerState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter()
	{
		m_bIsFly = m_pOwner->Get_IsFly();
	}
	virtual void Exit() 
	{ 
		m_pOwner->Get_Model()->Reset();

		if (0 != m_iSkillEvent)
			m_iSkillEvent = 0;
	}

	CPlayerState* ChangeState(CPlayerState* pCurrentState, CPlayerState* pNewState)
	{
		if (pCurrentState)
		{
			pCurrentState->Exit();
			Safe_Delete(pCurrentState);
		}

		pCurrentState = pNewState;
		pCurrentState->Enter();

		return pCurrentState;
	}

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;
	CPlayer* m_pOwner = nullptr;

	_bool m_bIsAnimationFinished = false;
	_bool m_bIsStateEvent = false;
	_int m_iSkillEvent = 0;
	// 공중인지 판단하는 변수
	_bool m_bIsFly = false;
};
END