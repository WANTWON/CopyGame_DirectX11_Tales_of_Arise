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

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 8000 + 4000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	return nullptr;
}

CBerserkerState * CIdleState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
	{
		if (m_pTarget)
		{
			return new CChaseState(m_pOwner);
		}
		else
		{
			if (m_fTimeDeltaAcc > m_fIdleTime)
			{
				switch (rand() % 3)
				{
				case 0:
					return new CWalkState(m_pOwner, FIELD_STATE_END);
				case 1:
					return new CIdleState(m_pOwner);
				case 2:
					return new CHowLing_State(m_pOwner);
			
				default:
					break;
				}
			}
		}
	}

	else
	{
		return new CTurnR_State(m_pOwner);
	}


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