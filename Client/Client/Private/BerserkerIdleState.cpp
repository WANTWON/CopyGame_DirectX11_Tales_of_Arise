#include "stdafx.h"

#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerChaseState.h"
#include "BerserkerWalkState.h"
#include "BerserkerHowLing_State.h"
#include "BerserkerTurnR_State.h"

using namespace Berserker;

CIdleState::CIdleState(CBerserker* pIceWolf, FIELD_STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fTimeDletaAcc = 0;
	m_fIdleTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	
	m_pOwner->Check_Navigation();

	return nullptr;
}

CBerserkerState * CIdleState::LateTick(_float fTimeDelta)
{
	m_fIdleMoveTimer += fTimeDelta;

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}
	else
	{
		if (m_fTimeDletaAcc > m_fIdleTime)
		{
			switch (rand() % 4)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 1:
				return new CIdleState(m_pOwner);
			case 2:
				return new CHowLing_State(m_pOwner);
			case 3:
				return new CTurnR_State(m_pOwner);
			default:
				break;
			}
		}
	}




	///////이전 코드 
	/*else
	{
		if (m_fIdleMoveTimer > 5.f)
		{
			switch (m_ePreState_Id)
			{
			case CBerserkerState::FIELD_STATE_ID::FIELD_STATE_IDLE:
				return new CWalkState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_HOWLING);
				break;

			case CBerserkerState::FIELD_STATE_ID::STATE_TURN_R:
				return new CWalkState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_TURN_R);
				break;

			case CBerserkerState::FIELD_STATE_ID::STATE_HOWLING:
				return new CWalkState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_HOWLING);
				break;

			case CBerserkerState::FIELD_STATE_ID::STATE_CHASE:
				return new CHowLing_State(m_pOwner);
				break;
			default:
				break;
			}

		}

	}*/


	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_IDLE);
}

void CIdleState::Exit()
{
	
}