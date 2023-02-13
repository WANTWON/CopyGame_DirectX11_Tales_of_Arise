#include "stdafx.h"
#include "..\Public\PlayerDeadState.h"

using namespace Player;

CPlayerDeadState::CPlayerDeadState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CPlayerDeadState::HandleInput()
{
	return nullptr;
}

CPlayerState * CPlayerDeadState::Tick(_float fTimeDelta)
{
	if (!m_bIsStop)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					m_bIsStop = true;
			}
		}
	}

	return nullptr;
}

CPlayerState * CPlayerDeadState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		m_bIsStop = true;

	return nullptr;
}

CPlayerState * CPlayerDeadState::EventInput(void)
{
	return nullptr;
}

void CPlayerDeadState::Enter()
{
	__super::Enter();

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DEAD);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DEAD);
		break;
	case CPlayer::RINWELL:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DEAD);
		break;
	}
}

void CPlayerDeadState::Exit()
{
	__super::Exit();
}
