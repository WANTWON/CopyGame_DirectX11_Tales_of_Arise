#include "stdafx.h"

#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"

using namespace Hawk;

CIdleState::CIdleState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_pOwner->Check_Navigation(); // ÀÚÀ¯


	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CHawkState * CIdleState::LateTick(_float fTimeDelta)
{
	if (m_pTarget)
		return new CChaseState(m_pOwner);



	

	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_IDLE);
}

void CIdleState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}