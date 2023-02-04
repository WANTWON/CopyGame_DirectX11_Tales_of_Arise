#pragma once

#include "Player.h"
#include "Monster.h"
BEGIN(Client)
class CAIState
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

	virtual ~CAIState() {};
	virtual CAIState* HandleInput() { return nullptr; };
	virtual CAIState* Tick(_float fTimeDelta) PURE;
	virtual CAIState* LateTick(_float fTimeDelta) PURE;

	virtual void Enter() PURE;
	virtual void Exit() {m_pOwner->Get_Model()->Reset();}

	CAIState* ChangeState(CAIState* pCurrentState, CAIState* pNewState)
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

	_float Get_Target_Distance()
	{
		_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
		return fDistance;
	}

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;
	STATE_ID m_ePreStateID = STATE_END;
	_uint m_iCurrentAnimIndex = 0;
	_uint m_eCurrentPlayerID = CPlayer::PLAYERID::SION;
	CPlayer* m_pOwner = nullptr;

	_float m_fTimer = 0.f;

	_bool m_bIsAnimationFinished = false;
	CBaseObj* m_pTarget = nullptr;
};
END