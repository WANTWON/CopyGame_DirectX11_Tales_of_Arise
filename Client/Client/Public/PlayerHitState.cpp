#include "stdafx.h"
#include "PlayerHitState.h"

using namespace Player;

CHitState::CHitState(CPlayer * pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CHitState::HandleInput()
{
	return nullptr;
}

CPlayerState * CHitState::Tick(_float fTimeDelta)
{
	return nullptr;
}

CPlayerState * CHitState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CHitState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_HIT;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		break;
	case CPlayer::SION:
		break;
	}
}

void CHitState::Exit()
{
	__super::Exit();
}
