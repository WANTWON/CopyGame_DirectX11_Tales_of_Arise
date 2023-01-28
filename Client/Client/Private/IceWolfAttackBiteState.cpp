#include "stdafx.h"
#include "..\Public\IceWolfAttackBiteState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_HowLingState.h"

using namespace IceWolf;

CAttackBiteState::CAttackBiteState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CAttackBiteState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackBiteState::Tick(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);

	m_fDegreeToTarget = RadianToTarget();
	
	
	return nullptr;
}

CIceWolfState * CAttackBiteState::LateTick(_float fTimeDelta)
{
	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());
	if (m_bCollision)
		m_iCollisionCount = 1;


	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		m_pOwner->Get_Transform()->Go_Straight(1.f);

		switch (m_iRand)
		{
		case 0:
			return new CBattle_SomerSaultState(m_pOwner);
			break;
		case 1:
			return new CBattle_BackStepState(m_pOwner);
			break;

		default:
			break;
		}
		
		
	}
	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	//if (m_fDegreeToTarget >= 30)
	//{
	//	return new CTurnLeftState(m_pOwner);
	//	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 0.5f);
	//}

	return nullptr;
}

void CAttackBiteState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_BITE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}


void CAttackBiteState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//m_pOwner->Get_Transform()->Go_Straight(1.1f);
}


