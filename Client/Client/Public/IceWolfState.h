#pragma once

#include "Ice_Wolf.h"

BEGIN(Client)
class CIceWolfState
{
public:
	enum STATE_ID
	{
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_ATTACK,
		STATE_END
	};

	STATE_ID Get_StateId() { return m_eStateId; }

	virtual ~CIceWolfState() {};
	virtual CIceWolfState* AI_Behaviour() { return nullptr; };
	virtual CIceWolfState* Tick(_float fTimeDelta) PURE;
	virtual CIceWolfState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() PURE;

	CIceWolfState* ChangeState(CIceWolfState* pCurrentState, CIceWolfState* pNewState)
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
	CIce_Wolf* m_pOwner = nullptr;

	_bool m_bIsAnimationFinished = false;
};
END