#include "stdafx.h"

#include "SlimeBattle_IdleState.h"
#include "GameInstance.h"
#include "SlimeWalkState.h"
#include "SlimeBattle_RunState.h"

using namespace Slime;

CBattle_IdleState::CBattle_IdleState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	

	return nullptr;
}

CSlimeState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	m_fRedayAttackTimer += fTimeDelta;

	if (m_fRedayAttackTimer >= 2.f)
	{
		return new CBattle_RunState(m_pOwner, STATE_IDLE);

	}

	
	return nullptr;
}

CSlimeState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	



	/*if (m_pTarget)
		return new CBattle_RunState(m_pOwner);*/


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

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_IDLE);
	
}

void CBattle_IdleState::Exit()
{

}