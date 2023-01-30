#include "stdafx.h"

#include "IceWolfWalkState.h"
#include "GameInstance.h"
#include "IceWolfIdleState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CWalkState::CWalkState(CIce_Wolf* pIceWolf, FIELD_STATE_ID ePreTurn)
{
	m_pOwner = pIceWolf;
	m_ePreTurn_Id = ePreTurn;
}

CIceWolfState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CIceWolfState * CWalkState::Tick(_float fTimeDelta)
{

	Find_Target();
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
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
		if(STATE_TURN_L == m_ePreTurn_Id)
			return new CTurnRightState(m_pOwner);

		else if(STATE_TURN_R == m_ePreTurn_Id)
			return new CTurnLeftState(m_pOwner);
	}
	//else
	//{
	//	return new CTurnLeftState(m_pOwner);
	//		
	//		/*if (m_fWalkMoveTimer > 3.f && m_iRand == 0)
	//			return new CIdleState(m_pOwner);

	//		else if (m_fWalkMoveTimer > 3.f && m_iRand == 1)
	//			return new CTurnLeftState(m_pOwner);

	//		else if (m_fWalkMoveTimer > 3.f && m_iRand == 2)
	//			return new CTurnRightState(m_pOwner);
	//		else
	//			m_fWalkMoveTimer += fTimeDelta;*/
	//}
	//

	//if (5.5f < m_fTarget_Distance)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
	//}

	//else
	//{
	//	switch (m_ePreState_Id)
	//	{
	//	case STATE_DASH_SCRATCHES:
	//		return new CBattle_Quadruple_ClawState(m_pOwner);
	//		break;

	//	case STATE_QUADRUPLE:
	//		return new CBattle_DashStartState(m_pOwner);
	//		break;

	//	default:
	//		break;
	//	}
	//}
	
	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);
}

void CWalkState::Exit()
{
	m_pOwner->Get_Model()->Reset();
}

