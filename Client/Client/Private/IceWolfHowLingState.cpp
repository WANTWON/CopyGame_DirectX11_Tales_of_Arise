#include "stdafx.h"
#include "..\Public\IceWolfHowLingState.h"
#include "IceWolfChaseState.h"
#include "IceWolfIdleState.h"

using namespace IceWolf;

CHowLingState::CHowLingState(CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CHowLingState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CIceWolfState * CHowLingState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CIceWolfState * CHowLingState::LateTick(_float fTimeDelta)
{
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner, CIceWolfState::FIELD_STATE_ID::STATE_HOWLING);

	return nullptr;
}

void CHowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_HOWLING);
}

void CHowLingState::Exit()
{
}



