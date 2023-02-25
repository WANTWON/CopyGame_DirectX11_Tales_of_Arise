#pragma once

#include "Player.h"
#include "Monster.h"

BEGIN(Client)
class CAIState
{
public:
	enum AI_MODE
	{
		ATTACK_LOCKONMODE,
		ATTACK_FREEMODE
	};

public:
	virtual ~CAIState() {};
	
public:
	virtual CAIState* HandleInput() PURE;
	virtual CAIState* Tick(_float fTimeDelta) PURE;
	virtual CAIState* Late_Tick(_float fTimeDelta) PURE;
	virtual CAIState* EventInput(void) PURE;
	virtual void Enter() PURE;

public:
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

		m_eAIMode = AI_MODE(CBattleManager::Get_Instance()->Get_AImode());
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
	STATE_ID m_ePreStateID = STATE_END;
	AI_MODE m_eAIMode = ATTACK_FREEMODE;
	_uint m_iCurrentAnimIndex = 0;
	_uint m_eCurrentPlayerID = CPlayer::PLAYERID::SION;

	_float m_fTimer = 0.f;

	_bool m_bRangerRunaway = false;
	_bool m_bIsStateEvent = false;
	CBaseObj* m_pTarget = nullptr;

	_bool m_bLookatOnetime = true;
};
END