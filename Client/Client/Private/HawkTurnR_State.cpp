#include "stdafx.h"

#include "HawkTurnR_State.h"
#include "GameInstance.h"
#include "HawkChaseState.h"

using namespace Hawk;

CTurnR_State::CTurnR_State(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CTurnR_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	m_pOwner->Check_Navigation();

	return nullptr;
}

CHawkState * CTurnR_State::LateTick(_float fTimeDelta)
{

	if (m_pTarget)
		return new CChaseState(m_pOwner);

	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::TURN_R);
}

void CTurnR_State::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}