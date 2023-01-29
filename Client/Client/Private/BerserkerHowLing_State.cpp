#include "stdafx.h"

#include "BerserkerHowLing_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
using namespace Berserker;

CHowLing_State::CHowLing_State(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CBerserkerState * CHowLing_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CHowLing_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	return nullptr;
}

CBerserkerState * CHowLing_State::LateTick(_float fTimeDelta)
{
	
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	/*if (m_fWalkMoveTimer > 1.5f)
		return new CIdleState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_HOWLING);*/

	return nullptr;
}

void CHowLing_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_HOWLING);
}

void CHowLing_State::Exit()
{

}