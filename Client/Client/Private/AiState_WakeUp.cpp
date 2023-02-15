#include "stdafx.h"
#include "..\Public\AiState_WakeUp.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "AICheckState.h"

using namespace AIPlayer;

CAiState_WakeUp::CAiState_WakeUp(CPlayer* pPlayer)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAiState_WakeUp::Tick(_float fTimeDelta)
{



	//m_fTimer += fTimeDelta;

	if (!m_bIsAnimationFinished)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();



	return nullptr;
}

CAIState * CAiState_WakeUp::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CAICheckState(m_pOwner, m_eStateId);

	else
		return nullptr;
}

void CAiState_WakeUp::Enter()
{
	//m_eStateId = STATE_ID::STATE_DEAD;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ARISE_B;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ARISE_B;
		break;
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ARISE_B;
		break;
	default:
		break;
	}

	m_pOwner->Set_Manarecover(true);

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
}

void CAiState_WakeUp::Exit()
{
	__super::Exit();
}