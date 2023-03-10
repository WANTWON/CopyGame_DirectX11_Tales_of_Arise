#include "stdafx.h"
#include "..\Public\PlayerPoseState.h"

#include "PlayerIdleState.h"

using namespace Player;

CPlayerPoseState::CPlayerPoseState(CPlayer * pPlayer, STATE_ID eStateID)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_eStateId = eStateID;
}

CPlayerState * CPlayerPoseState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CPlayerPoseState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (CPlayer::ALPHEN == m_ePlayerID)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Set_Rotation(true);
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Set_Rotation(false);
					//dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Reset_RotateTime();
				}
			}
		}
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CPlayerPoseState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);

	return nullptr;
}

void CPlayerPoseState::Enter(void)
{
	__super::Enter();

	switch (m_ePlayerID)
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ADVENT);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ADVENT);
		break;
	case CPlayer::RINWELL:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ADVENT);
		break;
	case CPlayer::LAW:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ADVENT);
		break;
	}
}

void CPlayerPoseState::Exit(void)
{
	__super::Exit();
}
