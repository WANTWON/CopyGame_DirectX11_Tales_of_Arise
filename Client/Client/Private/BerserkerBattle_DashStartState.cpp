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

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CBerserkerState * CBattle_DashStartState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CBattle_FireBallState(m_pOwner);

	return nullptr;
}

void CBattle_DashStartState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_DASH_CLAW_END);

}

void CBattle_DashStartState::Exit()
{
	
}