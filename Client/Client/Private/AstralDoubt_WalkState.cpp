#include "stdafx.h"

#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_TurnState.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_IdleState.h"

using namespace Astral_Doubt;

CWalkState::CWalkState(CAstralDoubt* pIceWolf, FIELD_STATE_ID ePreState, _bool bTriggerTurn)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_bTriggerTurn = bTriggerTurn;

	m_fTimeDeltaAcc = 0;
	m_fMoveTime = ((rand() % 4000) *0.001f )*((rand() % 100) * 0.01f);
	
}

CAstralDoubt_State * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CWalkState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
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

	if (!m_bIsAnimationFinished)
		m_pOwner->Check_Navigation();

	return nullptr;
}

CAstralDoubt_State * CWalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();


	//나의 트리거 박스랑 충돌안했을떄
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		//돌게하고

		if (false == m_bTriggerTurn)
		return new CTurnState(m_pOwner);
		
		// 그 트리거 박스의 위치 방향으로 이동하는 상태를 세팅한다.
		
		else
		{
			_vector vPosition = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.5f, m_pOwner->Get_Navigation());
			m_pOwner->Get_Transform()->LookAt(vPosition);
			
		}
	}

	//수정 코드 
	else
	{
		m_bTriggerTurn = false;

		m_fTimeDeltaAcc += fTimeDelta;

		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.5f, m_pOwner->Get_Navigation());

		if (m_pCurTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDeltaAcc > m_fMoveTime)
		{
			switch (rand() % 3)
			{
			case 0:
				return new CDetectStop_State(m_pOwner);
			case 1:
				return new CIdleState(m_pOwner);
				
			default:
				break;
			}
		}
	}


	return nullptr;
}


void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_WALK_F);
	
}

void CWalkState::Exit()
{
	
}

