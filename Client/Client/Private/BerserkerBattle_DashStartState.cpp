#include "stdafx.h"

#include "BerserkerBattle_DashStartState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_FireBallState.h"

using namespace Berserker;

CBattle_DashStartState::CBattle_DashStartState(CBerserker* pBerserker)
{
	
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_DashStartState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_DashStartState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	if (m_bIsAnimationFinished)
	{
			return new CBattle_FireBallState(m_pOwner);
			
	}
	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	

	return nullptr;
}

CBerserkerState * CBattle_DashStartState::LateTick(_float fTimeDelta)
{

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_bTargetSetting = true;
	}

	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	m_bCollision = pCollider->Collision(m_pTarget->Get_Collider());
	if (m_bCollision)
		m_iCollisionCount = 1;

	
	

	
	Safe_Release(pCollider);
	return nullptr;
}

void CBattle_DashStartState::Enter()
{

	//switch (m_eStateId)
	//{
	//case STATE_DASH_START:
	//	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_DASH_CLAW_START);
	//	break;
	//case STATE_DASH_LOOP:
	//	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_DASH_CLAW_LOOP);
	//	break;
	//case STATE_DASH_SCRATCHES:
	//	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_DASH_CLAW_END);
	//	break;
	//}

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_DASH_CLAW_END);
	////
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	////
}

void CBattle_DashStartState::Exit()
{
	
}