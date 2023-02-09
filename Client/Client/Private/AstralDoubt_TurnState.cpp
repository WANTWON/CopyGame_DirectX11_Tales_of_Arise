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
	m_fRandTime = ((rand() % 8000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CTurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CTurnState::Tick(_float fTimeDelta)
{
	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		else
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	else
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}


	if (m_pCurTarget == nullptr)
		return nullptr;

	return nullptr;
}

CAstralDoubt_State * CTurnState::LateTick(_float fTimeDelta)
{


	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fTurnR_Time)
		m_iRand = rand() % 2;
	



	//나의 트리거 박스랑 충돌안했을떄
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		if (m_bIsAnimationFinished)
			return new CWalkState(m_pOwner, FIELD_STATE_END, true);
	}

	//수정 코드 
	else
	{
		m_fTimeDeltaAcc += fTimeDelta;
		
		if (m_pCurTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDeltaAcc > m_fRandTime)
		{
			switch (rand() % 3)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 1:
				return new CDetectStop_State(m_pOwner);
			case 2:
				return new CIdleState(m_pOwner);


			default:
				break;
			}
		}
	}







	//if (m_pCurTarget)
	//{
	//	return new CChaseState(m_pOwner);
	//}

	//if (m_bIsAnimationFinished)
	//{
	//	//나의 트리거 박스랑 충돌안했을떄
	//	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	//	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	//		return new CWalkState(m_pOwner, FIELD_STATE_END, true);
	//	else
	//	{
	//		switch (m_iRand)
	//		{
	//		case 0:
	//			return new CWalkState(m_pOwner, FIELD_STATE_END, false);
	//		case 1:
	//			return new CIdleState(m_pOwner);
	//		case 2:
	//			return new CDetectStop_State(m_pOwner);
	//		default:
	//			break;
	//		}
	//	}

	//}
		

	return nullptr;
}

void CTurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	switch (rand()%2)
	{
	case 0:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::TURN_L);
		break;

	case 1:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::TURN_R);
		break;
	default:
		break;
	}

}

void CTurnState::Exit()
{

}