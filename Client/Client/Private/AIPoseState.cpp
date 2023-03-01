#include "stdafx.h"
#include "..\Public\AIPoseState.h"

#include "AICheckState.h"
#include "Alphen.h"
#include "Rinwell.h"
#include "Law.h"

using namespace AIPlayer;

CAIPoseState::CAIPoseState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAIPoseState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAIPoseState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (CBattleManager::Get_Instance()->Get_IsOneonOneMode() == true)
			return nullptr;
		else
			return new CAICheckState(m_pOwner, m_eStateId);
	}
	
	return nullptr;
}

void CAIPoseState::Enter(void)
{
	m_eStateId = STATE_ID::STATE_POSE;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ADVENT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ADVENT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ADVENT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;

	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_ADVENT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		break;
	}

	m_pOwner->Set_Manarecover(false);
}

void CAIPoseState::Exit(void)
{
	__super::Exit();
}
