#include "stdafx.h"

#include "BerserkerBattle_Quadruple_ClawState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_PouncingState.h"
using namespace Berserker;

CBattle_Quadruple_ClawState::CBattle_Quadruple_ClawState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Quadruple_ClawState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CBerserkerState * CBattle_Quadruple_ClawState::Tick(_float fTimeDelta)
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
	


	if (m_bIsAnimationFinished)
	{
		if (m_fTarget_Distance > 6)
			return new CBattle_Multiple_FireState(m_pOwner);

		else
			return new CBattle_PouncingState(m_pOwner);

	}



	return nullptr;
}

CBerserkerState * CBattle_Quadruple_ClawState::LateTick(_float fTimeDelta)
{
	
	

	return nullptr;
}

void CBattle_Quadruple_ClawState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_QUADRUPLE_CLAW);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Quadruple_ClawState::Exit()
{

}