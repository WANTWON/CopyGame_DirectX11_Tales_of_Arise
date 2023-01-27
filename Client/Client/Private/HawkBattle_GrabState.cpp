#include "stdafx.h"
#include "HawkBattle_GrabState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_GrabEndState.h"
#include "HawkBattle_GrabStartState.h"

using namespace Hawk;

CBattle_GrabState::CBattle_GrabState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	//m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CHawkState * CBattle_GrabState::AI_Behaviour(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (6 < m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	}

	return nullptr;
}

CHawkState * CBattle_GrabState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	
	if (m_bIsAnimationFinished)
	{
		return new CBattle_GrabStartState(m_pOwner);
	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix);
		m_pOwner->Check_Navigation();
	}

	//if (m_bIsAnimationFinished)
	//{
	//	switch (m_eStateId)
	//	{
	//	case Client::CHawkState::STATE_GRAB_START:
	//		if (10 > m_fTarget_Distance)
	//			return new CBattle_IdleState(m_pOwner);

	//		else
	//			return new CBattle_IdleState(m_pOwner);
	//		break;
	//	
	//		

	//		/*case Client::CHawkState::STATE_GRAB_LOOP:
	//		return new CBattle_GrabState(m_pOwner, STATE_ID::STATE_GRAB_END);
	//		break;
	//	case Client::CHawkState::STATE_GRAB_END:
	//		return new CBattle_IdleState(m_pOwner);
	//		break;
	//	*/}
	//}
	//else
	//{
	//	_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));
	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
	//	m_pOwner->Check_Navigation();
	//}

	return nullptr;
}

CHawkState * CBattle_GrabState::LateTick(_float fTimeDelta)
{
	Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	////1 . 콜라이더 업데이트를 해준다
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	//
	////2. 충돌체크

	if (m_bIsAnimationFinished)
	{
		m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());
		if (m_bCollision)
			m_iCollisionCount = 1;

		if (1 == m_iCollisionCount)
		{
			return new CBattle_GrabStartState(m_pOwner);
		}

		else
		{
			return new CBattle_IdleState(m_pOwner);
		}

	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}




//	if (m_bCollision)
	//{
	//	if (m_bIsAnimationFinished)
	//	{
	//		switch (m_eStateId)
	//		{
	//		case Client::CHawkState::STATE_GRAB_START:
	//			return new CBattle_GrabState(m_pOwner, STATE_ID::STATE_GRAB_START);

	//			if (pCollider->Collision(m_pTarget->Get_Collider()))
	//				return new CBattle_GrabState(m_pOwner, STATE_ID::STATE_GRAB_END);
	//			else
	//				return new CBattle_IdleState(m_pOwner);
	//			break;

	//		case Client::CHawkState::STATE_GRAB_LOOP:
	//			return new CBattle_GrabState(m_pOwner, STATE_ID::STATE_GRAB_END);
	//			break;

	//		case Client::CHawkState::STATE_GRAB_END:
	//			return new CBattle_IdleState(m_pOwner);
	//			break;
	//		}
	//	}
	//}
	//	else
	//	{
	//		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));
	//		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
	//		m_pOwner->Check_Navigation();
	//	}
	
	

	return nullptr;
}

void CBattle_GrabState::Enter()
{
	//switch (m_eStateId)
	//{
	//case STATE_GRAB_START:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_START1);
	//	break;
	////case STATE_GRAB_LOOP:
	////	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_LOOP1);
	////	break;
	//case STATE_GRAB_END:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_END);
	//	break;
	//}

	m_eStateId = STATE_ID::STATE_BATTLE;

//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_GRAB_START2);

	////
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	////


}

void CBattle_GrabState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
	m_pOwner->Check_Navigation();
}