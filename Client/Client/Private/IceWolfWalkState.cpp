#include "stdafx.h"

#include "IceWolfWalkState.h"
#include "GameInstance.h"

using namespace IceWolf;

CRunState::CRunState(CIce_Wolf* pIceWolf, DIRID eDir)
	: m_eDirection(eDir)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CRunState::AI_Behaviour()
{
	return nullptr;
}

CIceWolfState * CRunState::Tick(_float fTimeDelta)
{
	Move(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	return nullptr;
}

CIceWolfState * CRunState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CRunState::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);
}

void CRunState::Exit()
{

}

void CRunState::Move(_float fTimeDelta)
{
	/*switch (m_eDirection)
	{
	case DIR_STRAIGHT_LEFT:
	pPlayer->Get_Transform()->Set_RotationY(305.f);
	break;
	case DIR_STRAIGHT_RIGHT:
	pPlayer->Get_Transform()->Set_RotationY(45.f);
	break;
	case DIR_BACKWARD_LEFT:
	pPlayer->Get_Transform()->Set_RotationY(225.f);
	break;
	case DIR_BACKWARD_RIGHT:
	pPlayer->Get_Transform()->Set_RotationY(135.f);
	break;
	case DIR_STRAIGHT:
	pPlayer->Get_Transform()->Set_RotationY(0.f);
	break;
	case DIR_BACKWARD:
	pPlayer->Get_Transform()->Set_RotationY(180.f);
	break;
	case DIR_LEFT:
	pPlayer->Get_Transform()->Set_RotationY(270.f);
	break;
	case DIR_RIGHT:
	pPlayer->Get_Transform()->Set_RotationY(90.f);
	break;
	}

	pPlayer->Get_Transform()->Move_Straight(fTimeDelta, pPlayer->Get_Navigation(), pPlayer->Get_Radius());*/
}