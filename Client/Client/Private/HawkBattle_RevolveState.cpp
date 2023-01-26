#include "stdafx.h"
#include "HawkBattle_RevolveState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_GrabState.h"

using namespace Hawk;

CBattle_RevolveState::CBattle_RevolveState(CHawk* pHawk)
{

	m_pOwner = pHawk;
}

CHawkState * CBattle_RevolveState::AI_Behaviour(_float fTimeDelta)
{
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (9 < m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta *2.3f, vTargetPosition);
	}

	return nullptr;
}

CHawkState * CBattle_RevolveState::Tick(_float fTimeDelta)
{
	Find_BattleTarget();

	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	

	//if (5 < m_fTarget_Distance)
	//{
	//	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//}
	

		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

		
			if (m_bIsAnimationFinished)
			{
				if (80 >= m_fTarget_Distance)
				{
					m_pOwner->Get_Transform()->LookAt(vTargetPosition);
					return new CBattle_GrabState(m_pOwner);
						
				}

				else
					return new CBattle_IdleState(m_pOwner);
			}

			else
			{
				_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));
				m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix/*, m_pOwner->Get_Navigation()*/);
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
	//		return new CBattle_RevolveState(m_pOwner, STATE_ID::STATE_GRAB_END);
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

CHawkState * CBattle_RevolveState::LateTick(_float fTimeDelta)
{
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	//
	////2. 충돌체크
	m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());

	return nullptr;
}

void CBattle_RevolveState::Enter()
{

	m_eStateId = STATE_ID::STATE_BATTLE;

	//m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::/*ATTACK_ROTATION*/FLAPPING);

	////
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	////
}

void CBattle_RevolveState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}