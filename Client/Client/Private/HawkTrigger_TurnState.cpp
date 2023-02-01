#include "stdafx.h"

#include "HawkTrigger_TurnState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"
#include "HawkWalkState.h"

using namespace Hawk;

CTrigger_TurnState::CTrigger_TurnState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CTrigger_TurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CTrigger_TurnState::Tick(_float fTimeDelta)
{
	Find_Target();

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

CHawkState * CTrigger_TurnState::LateTick(_float fTimeDelta)
{

	/*if (m_pTarget)
		return new CChaseState(m_pOwner);*/

	if (m_bIsAnimationFinished)
		return new CWalkState(m_pOwner, true);
	return nullptr;
}

void CTrigger_TurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::TURN_R);
}

void CTrigger_TurnState::Exit()
{
	
}