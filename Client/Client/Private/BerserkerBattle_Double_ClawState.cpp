#include "stdafx.h"

#include "BerserkerBattle_Double_ClawState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_WalkState.h"

using namespace Berserker;

CBattle_Double_ClawState::CBattle_Double_ClawState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Double_ClawState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::Tick(_float fTimeDelta)
{
	//Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::LateTick(_float fTimeDelta)
{

	if (m_bIsAnimationFinished)
	{	
		return new CBattle_WalkState(m_pOwner);
	}
	
	
		
	return nullptr;
}

void CBattle_Double_ClawState::Enter()
{

	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_DOUBLE_CLAW);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

	
}

void CBattle_Double_ClawState::Exit()
{

}