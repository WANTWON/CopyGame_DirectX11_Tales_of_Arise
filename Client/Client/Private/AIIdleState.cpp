#include "stdafx.h"

#include "AIIdleState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"

using namespace AIPlayer;

CIdleState::CIdleState(CPlayer* pPlayer, STATE_ID eStateType)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CIdleState::Tick(_float fTimeDelta)
{

	

	m_fTimer += fTimeDelta;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	

	return nullptr;
}

CAIState * CIdleState::LateTick(_float fTimeDelta)
{

	if (m_fTimer > 3.f)
		return new CAIAttackNormalState(m_pOwner, STATE_ID::STATE_ATTACK , m_pTarget);


	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;
	m_iCurrentAnimIndex = CSion::ANIM::IDLE;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
}

void CIdleState::Exit()
{
	__super::Exit();
}