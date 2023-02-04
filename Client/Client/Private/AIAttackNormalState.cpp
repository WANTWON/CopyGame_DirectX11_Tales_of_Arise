#include "stdafx.h"
#include "AIAttackNormalState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIIdleState.h"

using namespace AIPlayer;

CAIAttackNormalState::CAIAttackNormalState(CPlayer* pPlayer , STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	
}

CAIState * CAIAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAIAttackNormalState::LateTick(_float fTimeDelta)
{

	if(m_bIsAnimationFinished)
	return new CIdleState(m_pOwner, STATE_ID::STATE_IDLE);

	return nullptr;
}

void CAIAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(2);
}

void CAIAttackNormalState::Exit()
{
}