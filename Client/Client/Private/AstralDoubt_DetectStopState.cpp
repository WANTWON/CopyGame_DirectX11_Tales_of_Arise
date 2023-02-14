#include "stdafx.h"

#include "AstralDoubt_DetectStopState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_IdleState.h"
#include "AstralDoubt_TurnState.h"

using namespace Astral_Doubt;

CDetectStop_State::CDetectStop_State(CAstralDoubt* pAstralDoubt)
{
	m_pOwner = pAstralDoubt;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CDetectStop_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CDetectStop_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CAstralDoubt_State * CDetectStop_State::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);


	_bool bIs_TargetInFront = false;

	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);

	if (m_pTarget)
		return new CChaseState(m_pOwner);


	if (m_bIsAnimationFinished)
	{
		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
		{
				if (m_fTimeDeltaAcc > m_fRandTime)
				{
					switch (rand() % 2)
					{
					case 0:
						return new CWalkState(m_pOwner, FIELD_STATE_END);
					case 1:
						return new CIdleState(m_pOwner);


					default:
						break;
					}
				}
	
		}

		else
		{
			if (bIs_TargetInFront)
				return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_DETECTSTOP, true);

			else
				return new CTurnState(m_pOwner);

		}

	}
		
	return nullptr;
}

void CDetectStop_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_LOOKOUT);
}

void CDetectStop_State::Exit()
{

}