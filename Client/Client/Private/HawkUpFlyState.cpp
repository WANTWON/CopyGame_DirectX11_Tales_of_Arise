#include "stdafx.h"

#include "HawkUpFlyState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"
#include "HawkWalkState.h"

using namespace Hawk;

CUpFlyState::CUpFlyState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CUpFlyState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CUpFlyState::Tick(_float fTimeDelta)
{
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	if (!m_bIsAnimationFinished)
	{
		_float fTranslationLength, fRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &fTranslationLength, &fRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((fTranslationLength * 0.01f), fRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CHawkState * CUpFlyState::LateTick(_float fTimeDelta)
{

	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CHawkState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	}
	return nullptr;
}

void CUpFlyState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::SYMBOL_DETECT_STOP);
}

void CUpFlyState::Exit()
{
	
}