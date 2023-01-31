#include "stdafx.h"

#include "HawkWalkState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkTurnR_State.h"

using namespace Hawk;

CWalkState::CWalkState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CWalkState::Tick(_float fTimeDelta)
{

	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.6f);
	

	return nullptr;
}

CHawkState * CWalkState::LateTick(_float fTimeDelta)
{
	
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 5.5f)
		return new CTurnR_State(m_pOwner);

	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_WALK_F);
}

void CWalkState::Exit()
{

}