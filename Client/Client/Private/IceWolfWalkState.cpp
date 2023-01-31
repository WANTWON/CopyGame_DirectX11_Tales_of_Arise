#include "stdafx.h"

#include "IceWolfWalkState.h"
#include "GameInstance.h"
#include "IceWolfIdleState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfChaseState.h"
#include "IceWolfHowLingState.h"

using namespace IceWolf;

CWalkState::CWalkState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
}

CIceWolfState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CIceWolfState * CWalkState::Tick(_float fTimeDelta)
{

	Find_Target();
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	m_pOwner->Check_Navigation();
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.6f);



	return nullptr;
}

CIceWolfState * CWalkState::LateTick(_float fTimeDelta)
{
	if (m_pTarget)
		return new CChaseState(m_pOwner);
	

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 1.5f)
	{
		switch (m_ePreState_Id)
		{
		case CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE:
			return new CTurnLeftState(m_pOwner);

		case CIceWolfState::FIELD_STATE_ID::STATE_TURN:
			return new CHowLingState(m_pOwner);

		default:
			break;
		}
	}

	
	
	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);
}

void CWalkState::Exit()
{
	
}

