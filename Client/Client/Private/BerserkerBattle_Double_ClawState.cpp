#include "stdafx.h"

#include "BerserkerBattle_Double_ClawState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"

using namespace Berserker;

CBattle_Double_ClawState::CBattle_Double_ClawState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Double_ClawState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::Tick(_float fTimeDelta)
{
	Find_BattleTarget();

	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	if (false == m_bIsAnimationFinished)
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	

	if (m_bIsAnimationFinished)
	{	
		
		return new CBattle_IdleState(m_pOwner);
	}
	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
		
	return nullptr;
}

void CBattle_Double_ClawState::Enter()
{
	Find_BattleTarget();

	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_DOUBLE_CLAW);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

	
}

void CBattle_Double_ClawState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}