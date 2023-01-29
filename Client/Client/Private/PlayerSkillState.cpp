#include "stdafx.h"
#include "..\Public\PlayerSkillState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"

using namespace Player;

CSkillState::CSkillState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CSkillState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());

		//m_pOwner->Check_Navigation();
	}

	vector<EVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK1:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (EVENT_STATE == pEvent.iEventType)
					{
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						RELEASE_INSTANCE(CGameInstance);
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK2:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (EVENT_STATE == pEvent.iEventType)
					{
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						RELEASE_INSTANCE(CGameInstance);
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK3:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (EVENT_STATE == pEvent.iEventType)
					{
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						RELEASE_INSTANCE(CGameInstance);
					}
					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{

			}
		}
		else
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{

			}
		}
	}

	return nullptr;
}

CPlayerState * CSkillState::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		/*switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK4);
			break;
		}*/
	}

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner);

	return nullptr;
}

void CSkillState::Enter(void)
{
	if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_AKIZAME);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HITENSYUOUKU_START);
			break;
		}
	}
	else if (CPlayer::SION == m_pOwner->Get_PlayerID())
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		}
	}
}

void CSkillState::Exit(void)
{
	__super::Exit();
}
