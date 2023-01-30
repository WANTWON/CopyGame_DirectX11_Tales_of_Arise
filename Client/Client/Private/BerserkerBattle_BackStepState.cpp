#include "stdafx.h"

#include "BerserkerBattle_BackStepState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_BackStepState::CBattle_BackStepState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_BackStepState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_BackStepState::Tick(_float fTimeDelta)
{
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	
	else if (m_bIsAnimationFinished)
	{
		return new CBattle_RunState(m_pOwner, STATE_QUADRUPLE);
	}
	return nullptr;
}

CBerserkerState * CBattle_BackStepState::LateTick(_float fTimeDelta)
{

	return nullptr;
}

void CBattle_BackStepState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_STEP_BACK);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_BackStepState::Exit()
{

}