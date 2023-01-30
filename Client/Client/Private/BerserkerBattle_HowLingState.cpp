#include "stdafx.h"

#include "BerserkerBattle_HowLingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_HowLingState::CBattle_HowLingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_HowLingState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_HowLingState::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	return nullptr;
}

CBerserkerState * CBattle_HowLingState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		m_pOwner->Set_FinishHowling();
		m_pOwner->Set_OnAngry();
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_QUADRUPLE);
		
	}

	else
	{

			m_pOwner->Check_Navigation();
		
		m_pOwner->Set_OnGoingHowling();
	}
	return nullptr;
}

void CBattle_HowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_HOWLING);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_HowLingState::Exit()
{

}