#include "stdafx.h"

#include "HawkBattle_BombingState.h"
#include "GameInstance.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_BombingState::CBattle_BombingState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_BombingState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CHawkState * CBattle_BombingState::Tick(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	


	return nullptr;
}

CHawkState * CBattle_BombingState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (10 < m_fTarget_Distance)
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	if (true == m_bIsAnimationFinished)
		return new CBattle_IdleState(m_pOwner);

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	

	//if (m_pTarget)
	//{
	//	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//	return new CBattle_HowLingState(m_pOwner);



	//	if (m_fIdleAttackTimer > 1.5f)
	//	{
	//		/*if (!m_bHasSpottedTarget)
	//		return new CAggroState();
	//		else
	//		return new CAttackState();*/
	//	}
	//	else
	//		m_fIdleAttackTimer += fTimeDelta;
	//}

	/*else
	{

		m_iRand = rand() % 3;
		if (m_fIdleMoveTimer > 3.f && m_iRand == 0)
			return new CTurnLeftState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 1)
			return new CTurnRightState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 2)
			return new CWalkFrontState(m_pOwner);

		else
			m_fIdleMoveTimer += fTimeDelta;

	}*/

	return nullptr;
}

void CBattle_BombingState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

//	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_BOMBING_DUP);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_BombingState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	m_bAnimFinish = false;
}