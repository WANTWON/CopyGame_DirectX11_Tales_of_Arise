#include "stdafx.h"

#include "HawkSitOnState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"
#include "HawkWalkState.h"
#include "HawkUpFlyState.h"
using namespace Hawk;

CSitOnState::CSitOnState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CSitOnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CSitOnState::Tick(_float fTimeDelta)
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
	
	return nullptr;
}

CHawkState * CSitOnState::LateTick(_float fTimeDelta)
{

	if (m_fTarget_Distance <= 20 )
		return new CUpFlyState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		return new CSitOnState(m_pOwner);
	}
	return nullptr;
}

void CSitOnState::Enter()
{
	m_eStateId = STATE_ID::STATE_DORMANT;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::SYMBOL_STOP);
}

void CSitOnState::Exit()
{
	
}