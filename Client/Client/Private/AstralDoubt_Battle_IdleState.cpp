#include "stdafx.h"

#include "AstralDoubt_Battle_IdleState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_TurnState.h"
#include "AstralDoubt_Battle_WalkState.h"

using namespace Astral_Doubt;

CBattle_IdleState::CBattle_IdleState(CAstralDoubt* pAstralDoubt, STATE_ID  ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	return nullptr;
}

CAstralDoubt_State * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	
	m_pOwner->Check_Navigation();
	m_fTimeDeltaAcc += fTimeDelta;

	
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		if (m_bIsAnimationFinished)
		{
			return new CBattleWalkState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_IDLE);
		}

		else
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			m_pOwner->Check_Navigation();
		}
	}

	else
	{
		if (m_fTimeDeltaAcc > m_fRandTime)
			return new CBattleWalkState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_IDLE);
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	if(m_ePreState_Id)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ARISE_B);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	if (m_ePreState_Id == STATE_ID::STATE_DOWN)
	{
		m_pOwner->Set_FinishGoingDown();
		m_pOwner->Set_FinishDownState();
	}
}