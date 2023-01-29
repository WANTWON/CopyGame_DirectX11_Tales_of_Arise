#include "stdafx.h"

#include "HawkTurnR_State.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"

using namespace Hawk;

CTurnR_State::CTurnR_State(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CTurnR_State::Tick(_float fTimeDelta)
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

CHawkState * CTurnR_State::LateTick(_float fTimeDelta)
{

	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{

		return new CIdleState(m_pOwner, CHawkState::FIELD_STATE_ID::STATE_TURN_R);
	}
	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::TURN_R);
}

void CTurnR_State::Exit()
{
	
}