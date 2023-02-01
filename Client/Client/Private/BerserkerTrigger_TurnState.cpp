#include "stdafx.h"

#include "BerserkerTrigger_TurnState.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerWalkState.h"

using namespace Berserker;

CTrigger_TurnState::CTrigger_TurnState(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CBerserkerState * CTrigger_TurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CTrigger_TurnState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		//m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CBerserkerState * CTrigger_TurnState::LateTick(_float fTimeDelta)
{
	
	/*if (m_pTarget)
		return new CChaseState(m_pOwner);*/

	m_fWalkMoveTimer += fTimeDelta;

	if (m_bIsAnimationFinished)
		return new CWalkState(m_pOwner, FIELD_STATE_END, true);
	return nullptr;
}

void CTrigger_TurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);
}

void CTrigger_TurnState::Exit()
{
	
}