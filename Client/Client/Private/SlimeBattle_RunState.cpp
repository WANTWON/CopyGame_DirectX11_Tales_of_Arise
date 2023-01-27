#include "stdafx.h"

#include "SlimeBattle_RunState.h"
#include "GameInstance.h"
#include "SlimeBattle_Atk_TackleState.h"
#include "SlimeIdleState.h"

using namespace Slime;

CBattle_RunState::CBattle_RunState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{


		//if (m_fIdleAttackTimer > 15.f && 0 == m_iRand)
		//	return new CAtk_TackleState(m_pOwner, STATE_ID::STATE_TACKLE_START);
		///*else if (m_fIdleAttackTimer > 3.f && 1 == m_iRand)
		//	return new CAttackNormalState(m_pOwner);*/

		//else m_fIdleAttackTimer += fTimeDelta;
	
	
	//else if (m_fTarget_Distance >= 15.f)
	//{
	//	return new CIdleState(m_pOwner);
	//}

	



	/*if (6.f == m_fTarget_Distance)
		return new CAtk_TackleState(m_pOwner, Client::CSlimeState::STATE_ID::STATE_TACKLE_START);*/


	return nullptr;
}

CSlimeState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);
	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			
	//m_pOwner->Check_Navigation(); // ÀÚÀ¯

	//if(1.f <= m_fTarget_Distance)
	//	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	//AI_Behaviour(fTimeDelta);

	return nullptr;
}

CSlimeState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	//m_iRand = rand() % 1;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	m_pOwner->Check_Navigation();


		if (2.f < m_fTarget_Distance)
		{
			m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

			/*if (10 >= m_fIdleAttackTimer)
			{
				return new CAtk_TackleState(m_pOwner, Client::CSlimeState::STATE_TACKLE_START);
			}

			else
				m_fIdleMoveTimer += fTimeDelta;*/
		}


	
	//if (3 < m_fTarget_Distance)
	//	return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_LOOP);
	//else if (3 >= m_fTarget_Distance)
	//	return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
	//else
	//{
	//	_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();

	//	if (1.f <= m_fTarget_Distance)
	//	{
	//		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	//		//if (m_fIdleAttackTimer > 10.f && 0 == m_iRand)
	//		//	return new CAtk_TackleState(m_pOwner, STATE_ID::STATE_TACKLE_START);
	//		///*else if (m_fIdleAttackTimer > 3.f && 1 == m_iRand)
	//		//return new CAttackNormalState(m_pOwner);*/

	//		//else m_fIdleAttackTimer += fTimeDelta;
	//	}
	/*}*/
	
	

	


	//if (15.f > m_fTarget_Distance)
	//{
	//	if (1.f <= m_fTarget_Distance)
	//		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//	}
	//if (8.f >= m_fTarget_Distance)
	//{
	//	if (m_iRand == 0)
	//		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	//	/*else if (m_iRand == 1)
	//		return new CAtk_TackleState(m_pOwner, Client::CSlimeState::STATE_ID::STATE_TACKLE_START);*/
	//}

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

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_RUN);

	//m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_RunState::Exit()
{

}