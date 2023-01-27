#include "stdafx.h"

#include "SlimeWalkState.h"
#include "GameInstance.h"


using namespace Slime;

CWalkState::CWalkState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CSlimeState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (10 >= m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
	}
	return nullptr;
}

CSlimeState * CWalkState::Tick(_float fTimeDelta)
{


	AI_Behaviour(fTimeDelta);

	m_pOwner->Check_Navigation(); // ÀÚÀ¯

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CSlimeState * CWalkState::LateTick(_float fTimeDelta)
{
	

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

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_WALK_F);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CWalkState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}