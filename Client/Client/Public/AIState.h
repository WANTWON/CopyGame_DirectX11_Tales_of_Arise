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
		STATE_JUMP,
		STATE_NORMAL_ATTACK1,
		STATE_NORMAL_ATTACK2,
		STATE_NORMAL_ATTACK3,
		STATE_NORMAL_ATTACK4,
		STATE_NORMAL_ATTACK5,
		STATE_ATTACK,
		STATE_BOOSTATTACK,
		STATE_DEAD,
		STATE_HIT,
		STATE_DODGE,
		STATE_THUNDERFIELD,
		STATE_METEOR,
		STATE_GALEFORCE,
		STATE_DIVINE_SABER,
		STATE_HOLYRANCE,
		STATE_BANGJEON,
		
		STATE_SKILL_ATTACK_E,
		STATE_SKILL_ATTACK_R,
		STATE_SKILL_ATTACK_F,
		STATE_POSE,
		STATE_END
		
	};

	enum AI_MODE
	{
		ATTACK_LOCKONMODE,
		ATTACK_FREEMODE
	};

	STATE_ID Get_StateId() { return m_eStateId; }
	_float Get_Time(void) { return m_fTime; }
	CPlayer* Get_Owner(void) { return m_pOwner; }

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
		if (!m_pTarget)
		{
			CBaseObj* pObj = CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
			m_pTarget = static_cast<CMonster*>(pObj);
		}

		if (!m_pTarget)
			return 5.f;

		_vector vMonsterPosition = m_pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
		_vector vPlayerPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

		_float fDistance = XMVectorGetX(XMVector3Length(vMonsterPosition - vPlayerPosition));
		return fDistance;
	}

	_bool CheckTarget()
	{

		m_eAIMode = CBattleManager::Get_Instance()->Get_AImode();
		if (CBattleManager::Get_Instance()->IsAllMonsterDead())
			return false;
		if (nullptr == CBattleManager::Get_Instance()->Get_LackonMonster())
			return false;

		switch (m_eAIMode)
		{
		case ATTACK_LOCKONMODE:
			if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
			{
				m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
			}
			else
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			}

			if (m_pTarget == nullptr)
				return false;
			break;
		case ATTACK_FREEMODE:
			m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
			(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		}

		return true;
	}
	

protected:
	STATETYPE m_eStateType = STATETYPE_DEFAULT;
	STATE_ID m_eStateId = STATE_END;
	STATE_ID m_ePreStateID = STATE_END;
	_uint m_eAIMode = ATTACK_FREEMODE;
	_uint m_iCurrentAnimIndex = 0;
	_uint m_eCurrentPlayerID = CPlayer::PLAYERID::SION;
	CPlayer* m_pOwner = nullptr;

	_float m_fTimer = 0.f;

	_bool m_bIsAnimationFinished = false;
	_bool m_bRangerRunaway = false;
	_bool m_bIsStateEvent = false;
	CBaseObj* m_pTarget = nullptr;

	_bool m_bLookatOnetime = true;

	// 공중인지 판단하는 변수
	_bool m_bIsFly = false;
	_float m_fTime = 0.f;
	
};
END