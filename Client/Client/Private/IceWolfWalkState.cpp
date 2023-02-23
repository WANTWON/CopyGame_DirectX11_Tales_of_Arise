#include "stdafx.h"

#include "IceWolfWalkState.h"
#include "GameInstance.h"
#include "IceWolfIdleState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfChaseState.h"
#include "IceWolfHowLingState.h"

using namespace IceWolf;

CWalkState::CWalkState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState, _bool bTriggerTurn)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_bTriggerTurn = bTriggerTurn;

	m_fTimeDeltaAcc = 0;
	m_fMoveTime = ((rand() % 10000) *0.001f )*((rand() % 100) * 0.01f);
	
}

CIceWolfState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CWalkState::Tick(_float fTimeDelta)
{
	Find_Target();
	//m_pCurTarget = m_pOwner->Check_FiledTarget();
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	return nullptr;
}

CIceWolfState * CWalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	//나의 트리거 박스랑 충돌안했을떄
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		//돌게하고

		if (false == m_bTriggerTurn)
		return new CTurnLeftState(m_pOwner);
		
		// 그 트리거 박스의 위치 방향으로 이동하는 상태를 세팅한다.
		
		else
		{
			_vector vPosition = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.17f, m_pOwner->Get_Navigation());
			m_pOwner->Get_Transform()->LookAt(vPosition);
			
		}
	}

	//수정 코드 
	else
	{
		m_bTriggerTurn = false;

		m_fTimeDeltaAcc += fTimeDelta;

		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.17f, m_pOwner->Get_Navigation());

		if (m_pTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDeltaAcc > m_fMoveTime)
		{
			switch (rand() % 4)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 1:				
				return new CHowLingState(m_pOwner);
			case 2:
				return new CTurnLeftState(m_pOwner);
			case 3:
				return new CIdleState(m_pOwner);
		

			default:
				break;
			}
		}
	}



	//원본코드
	/*else
	{
		m_fWalkMoveTimer += fTimeDelta;

		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
		
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.6f, m_pOwner->Get_Navigation());

		if (m_pTarget)
			return new CChaseState(m_pOwner);

		if (m_fWalkMoveTimer > 10.5f)
		{
			switch (m_ePreState_Id)
			{
			case CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE:
				return new CTurnLeftState(m_pOwner);

			case CIceWolfState::FIELD_STATE_ID::STATE_TURN:
				return new CHowLingState(m_pOwner);

			default:
				break;
			}
		}
	}*/


	return nullptr;
}


void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);
	
}

void CWalkState::Exit()
{
	
}

