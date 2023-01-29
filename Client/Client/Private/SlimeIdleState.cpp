#include "stdafx.h"

#include "SlimeIdleState.h"
#include "GameInstance.h"
#include "SlimeWalkState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeChaseState.h"
#include "SlimeTurnR_State.h"

using namespace Slime;

CIdleState::CIdleState(CSlime* pSlime, FIELD_STATE_ID ePreState)
{
	m_pOwner = pSlime;
	m_ePreState_Id = ePreState;
}

CSlimeState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CSlimeState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{

		m_pOwner->Check_Navigation();
	}

	
	return nullptr;
}

CSlimeState * CIdleState::LateTick(_float fTimeDelta)
{
	m_fIdleMoveTimer += fTimeDelta;

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	else
	{
		if (m_fIdleMoveTimer > 1.5f)
		{
			switch (m_ePreState_Id)
			{
			case CSlimeState::FIELD_STATE_ID::FIELD_STATE_IDLE:
				return new CWalkState(m_pOwner);

			case CSlimeState::FIELD_STATE_ID::STATE_WALK:
				return new CTurnR_State(m_pOwner);
				break;

			case CSlimeState::FIELD_STATE_ID::STATE_TURN_R:
				return new CWalkState(m_pOwner);
				break;

			//case CSlimeState::FIELD_STATE_ID::STATE_BRAVE:
			//	return new CWalkState(m_pOwner);
			default:
				break;
			}

		}

	}



	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_IDLE);
	
}

void CIdleState::Exit()
{

}