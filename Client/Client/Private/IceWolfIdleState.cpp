#include "stdafx.h"

#include "IceWolfIdleState.h"
#include "GameInstance.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CIdleState::CIdleState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fIdleTimeAcc = 0;
	m_fIdleTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CIdleState::Tick(_float fTimeDelta)
{
//	Find_Target();

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

CIceWolfState * CIdleState::LateTick(_float fTimeDelta)
{


	m_fIdleTimeAcc += fTimeDelta;

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	


	else
	{
		if (m_fIdleTimeAcc > m_fIdleTime)
		{
			
			switch (rand()%4)
			{
			case 0:
				return new CWalkState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
				break;
			case 1:
				return new CIdleState(m_pOwner, FIELD_STATE_ID::STATE_TURN);
				break;
			case 2:
				return new CWalkState(m_pOwner, STATE_TURN);
				break;
			case 3:
				return new CIdleState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE);
				break;
			default:
				break;
			}
		}
	}










	//원본코드
	//else
	//{
	//	if (m_fIdleMoveTimer > 3.5f)
	//	{
	//		
	//		switch (m_ePreState_Id)
	//		{
	//		case Client::CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE:
	//			return new CWalkState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	//			break;
	//		case Client::CIceWolfState::FIELD_STATE_ID::STATE_TURN_L:
	//			return new CIdleState(m_pOwner, FIELD_STATE_ID::STATE_TURN);
	//			break;
	//		case Client::CIceWolfState::STATE_TURN:
	//			return new CWalkState(m_pOwner, STATE_TURN);
	//			break;
	//		case Client::CIceWolfState::FIELD_STATE_ID::STATE_HOWLING:
	//			return new CIdleState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE);
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}


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