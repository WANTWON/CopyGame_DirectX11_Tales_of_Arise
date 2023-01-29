#include "stdafx.h"

#include "HawkBraveState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"

using namespace Hawk;

CBraveState::CBraveState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CBraveState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CBraveState::Tick(_float fTimeDelta)
{
	Find_Target();

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

CHawkState * CBraveState::LateTick(_float fTimeDelta)
{

	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CHawkState::FIELD_STATE_ID::STATE_BRAVE);
	}
	return nullptr;
}

void CBraveState::Enter()
{

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_BRAVE);
}

void CBraveState::Exit()
{
	
}