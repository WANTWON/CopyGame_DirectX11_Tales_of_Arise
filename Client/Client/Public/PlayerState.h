#pragma once

#include "Player.h"

BEGIN(Client)
class CPlayerState
{
public:
	enum STATETYPE
	{
		STATETYPE_START,
		STATETYPE_MAIN,
		STATETYPE_END,
		STATETYPE_DEFAULT
	};

	enum STATE_ID
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_ATTACK,
		STATE_END
	};

	STATE_ID Get_StateId() { return m_eStateId; }

	virtual ~CPlayerState() {};
	virtual CPlayerState* HandleInput() { return nullptr; };
	virtual CPlayerState* Tick(_float fTimeDelta) PURE;
	virtual CPlayerState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

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
};
END