#include "stdafx.h"

#include "BerserkerBattle_Shock_WaveState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_TurnState.h"

using namespace Berserker;

CBattle_Shock_WaveState::CBattle_Shock_WaveState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Shock_WaveState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}





	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::LateTick(_float fTimeDelta)
{
	
	if (m_bIsAnimationFinished)
	{

		if (m_bIsAnimationFinished)
		{
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE); 
			//return new CBattle_RunState(m_pOwner, STATE_ID::STATE_POUNCING);
		}


		/*if (m_fTarget_Distance > 7)
			return new CBattle_Multiple_FireState(m_pOwner);

		else
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_QUADRUPLE);*/
		
	}

	return nullptr;
}

void CBattle_Shock_WaveState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_SHOCK_WAVE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Shock_WaveState::Exit()
{

}