#include "stdafx.h"

#include "BerserkerTurnR_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
using namespace Berserker;

CTurnR_State::CTurnR_State(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CBerserkerState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CTurnR_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{

	}

	return nullptr;
}

CBerserkerState * CTurnR_State::LateTick(_float fTimeDelta)
{
	
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 1.f)
		return new CIdleState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_TURN_R);

	else
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);
}

void CTurnR_State::Exit()
{

}