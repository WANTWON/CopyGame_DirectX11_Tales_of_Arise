#include "stdafx.h"

#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_TurnState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_IdleState.h"

using namespace Astral_Doubt;

CTurnState::CTurnState(CAstralDoubt* pAstralDoubt)
{
	m_pOwner = pAstralDoubt;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 3000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CTurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CTurnState::Tick(_float fTimeDelta)
{
	Find_Target();
	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 0.8f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);

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

CAstralDoubt_State * CTurnState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;


	return nullptr;
}

void CTurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	switch (rand()%2)
	{
	case 0:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_TURN_LEFT);
		break;

	case 1:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_TURN_RIGHT);
		break;
	default:
		break;
	}

}

void CTurnState::Exit()
{

}