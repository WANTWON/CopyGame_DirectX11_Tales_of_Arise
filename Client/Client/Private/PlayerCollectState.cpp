#include "stdafx.h"
#include "..\Public\PlayerCollectState.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"

using namespace Player;

CCollectState::CCollectState(CPlayer * pPlayer, CInteractObject* pObject)
{
	m_pOwner = pPlayer;
	m_pObject = pObject;
}

CPlayerState * CCollectState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CCollectState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					if (nullptr != m_pObject)
						m_pObject->Set_Interact();
					else
						return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
				}
			}
		}
	}
	
	return nullptr;
}

CPlayerState * CCollectState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	
	return nullptr;
}

void CCollectState::Enter(void)
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_COLLECT;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		if (nullptr == m_pObject)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_OPEN_DOOR);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_OPEN_TREASURE);
		break;
	case CPlayer::SION:
		if (nullptr == m_pObject)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DOOR_OPEN_HAND_RIGHT);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::TREASURE_OPEN);
		break;
	case CPlayer::RINWELL:
		if (nullptr == m_pObject)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::SYS_DOOR_OPEN_HAND_RIGHT);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::TREASURE_OPEN);
		break;
	case CPlayer::LAW:
		if (nullptr == m_pObject)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DOOR_OPEN_RIGHT_HAND);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::TREASURE_OPEN);
		break;
	}
}

void CCollectState::Exit(void)
{
	__super::Exit();
}
