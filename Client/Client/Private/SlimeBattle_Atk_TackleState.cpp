#include "stdafx.h"

#include "SlimeBattle_Atk_TackleState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"

using namespace Slime;

CAtk_TackleState::CAtk_TackleState(CSlime* pSlime, STATE_ID eStateType)
{
	m_pOwner = pSlime;
	m_eStateId = eStateType;

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CSlimeState * CAtk_TackleState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	
	

	return nullptr;
}

CSlimeState * CAtk_TackleState::Tick(_float fTimeDelta)
{


	AI_Behaviour(fTimeDelta);

	m_pOwner->Check_Navigation(); // ÀÚÀ¯

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	if (true == m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CSlimeState::STATE_TACKLE_START:
			return new CAtk_TackleState(m_pOwner, STATE_ID::STATE_TACKLE_START);
			break;
		
		case Client::CSlimeState::STATE_TACKLE_DOWN:
			return new CAtk_TackleState(m_pOwner, STATE_ID::STATE_TACKLE_START);
			break;

		case Client::CSlimeState::STATE_TACKLE_ARISE:
			return new CBattle_RunState(m_pOwner);
			break;

		}
	}
		

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CSlimeState * CAtk_TackleState::LateTick(_float fTimeDelta)
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

void CAtk_TackleState::Enter()
{

	switch (m_eStateId)
	{
	case STATE_TACKLE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ATTACK_TACKLE);
		break;

	case STATE_TACKLE_DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::DOWN_F);
		break;

	case STATE_TACKLE_ARISE:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ARISE_F);
		break;


	}
	
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAtk_TackleState::Exit()
{

}