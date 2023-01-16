#include "stdafx.h"

#include "IceWolfIdleState.h"
#include "GameInstance.h"
#include "IceWolfWalkState.h"

using namespace IceWolf;

CIdleState::CIdleState(CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CIdleState::AI_Behaviour()
{
	return nullptr;
}

CIceWolfState * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	if (fTimer > 5.f)
		return new CRunState(m_pOwner, DIR_STRAIGHT);
	else
		fTimer += fTimeDelta;

	return nullptr;
}

CIceWolfState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CIdleState::Exit()
{

}