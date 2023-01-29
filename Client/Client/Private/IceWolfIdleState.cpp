#include "stdafx.h"

#include "IceWolfIdleState.h"
#include "GameInstance.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CIdleState::CIdleState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState, FIELD_STATE_ID ePreTurn)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_ePreTurn_Id = ePreTurn;
}

CIceWolfState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	m_pOwner->Check_Navigation(); // ÀÚÀ¯

	return nullptr;
}

CIceWolfState * CIdleState::LateTick(_float fTimeDelta)
{
	m_fIdleMoveTimer += fTimeDelta;

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	
	else
	{
		if (m_fIdleMoveTimer > 3.f)
		{
			switch (m_ePreState_Id)
			{
			case Client::CIceWolfState::STATE_IDLE:
				return new CWalkState(m_pOwner, m_ePreTurn_Id);
				break;
			case Client::CIceWolfState::STATE_TURN_L:
				return new CIdleState(m_pOwner, FIELD_STATE_IDLE, STATE_TURN_L);
				break;
			case Client::CIceWolfState::STATE_TURN_R:
				return new CIdleState(m_pOwner, FIELD_STATE_IDLE, STATE_TURN_R);
				break;
			case Client::CIceWolfState::STATE_HOWLING:
				break;
			default:
				break;
			}
		}
	}

		
		/*if (m_fIdleMoveTimer > 3.f && m_iRand == 0)
			return new CTurnLeftState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 1)
			return new CTurnRightState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 2)
			

		else
			m_fIdleMoveTimer += fTimeDelta;*/

	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CIdleState::Exit()
{

}