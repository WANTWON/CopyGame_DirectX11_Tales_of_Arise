#include "stdafx.h"

#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_TurnState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_IdleState.h"

using namespace Astral_Doubt;

CTurnState::CTurnState(CAstralDoubt* pAstralDoubt)
{
	m_pOwner = pAstralDoubt;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 3000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CTurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CTurnState::Tick(_float fTimeDelta)
{
	Find_Target();
	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 0.8f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
		
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	

	return nullptr;
}

CAstralDoubt_State * CTurnState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;


	
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);


	if (m_bIsAnimationFinished)
	{
		//나의 트리거 박스랑 충돌해 있을때(트리거 박스 안에 있을 때)
		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
		{
			if (m_pTarget)
			{
				return new CChaseState(m_pOwner);
			}

			else
			{
				if (m_fTimeDeltaAcc > m_fRandTime)
				{
					switch (rand() % 2)
					{
					case 0:
						return new CWalkState(m_pOwner, FIELD_STATE_END);
					case 1:
						return new CDetectStop_State(m_pOwner);
					}
				}
			}
		}

		else
		{
			if (bIs_TargetInFront)
				return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_TURN, true);

			else
				return new CTurnState(m_pOwner);
		}
	}

	return nullptr;
}

void CTurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	switch (rand()%2)
	{
	case 0:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_TURN_LEFT);
		break;

	case 1:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_TURN_RIGHT);
		break;
	default:
		break;
	}

}

void CTurnState::Exit()
{

}