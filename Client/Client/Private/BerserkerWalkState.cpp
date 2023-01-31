#include "stdafx.h"

#include "BerserkerWalkState.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerHowLing_State.h"
using namespace Berserker;

CWalkState::CWalkState(CBerserker* pBerserker, FIELD_STATE_ID ePreState)
{
	m_pOwner = pBerserker;
	m_ePreState_Id = ePreState;
}

CBerserkerState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CWalkState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");



	m_pOwner->Check_Navigation();
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.6f);

	


	return nullptr;
}

CBerserkerState * CWalkState::LateTick(_float fTimeDelta)
{
	
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 6.5f)
		switch (m_ePreState_Id)
		{
		case CBerserkerState::FIELD_STATE_ID::STATE_TURN_R:
			return new CHowLing_State(m_pOwner);

		case CBerserkerState::FIELD_STATE_ID::STATE_HOWLING:
			return new CTurnR_State(m_pOwner);

		default:
			break;
		}

	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_WALK_F);
}

void CWalkState::Exit()
{

}