#include "stdafx.h"

#include "BerserkerBattle_PouncingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
using namespace Berserker;

CBattle_PouncingState::CBattle_PouncingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_PouncingState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_PouncingState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	m_iRand = rand() % 2;

	if (m_bIsAnimationFinished)
	{
		if(m_iRand = 0)
			return new CBattle_Multiple_FireState(m_pOwner);

		else
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_QUADRUPLE);
	}


	return nullptr;
}

CBerserkerState * CBattle_PouncingState::LateTick(_float fTimeDelta)
{
	
	return nullptr;
}

void CBattle_PouncingState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_POUNCING);

}

void CBattle_PouncingState::Exit()
{

}