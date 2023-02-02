#include "stdafx.h"
#include "..\Public\PlayerSkillState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerJumpState.h"
#include "UI_Skillmessage.h"

using namespace Player;

CSkillState::CSkillState(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
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
	}
	else
		m_pOwner->Check_Navigation();

	if (m_bIsFly)
		m_fTime += 0.1f;

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
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
				case Client::CPlayerState::STATE_SKILL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
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
		return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if ((0 != m_iSkillEvent) && (floor(m_pOwner->Get_Info().fCurrentMp) > 0))
	{
		switch (m_iSkillEvent)
		{
		case 1:
			return new CSkillState(m_pOwner, STATE_ID::STATE_SKILL_ATTACK1);
			break;
		case 2:
			return new CSkillState(m_pOwner, STATE_ID::STATE_SKILL_ATTACK2);
			break;
		case 3:
			return new CSkillState(m_pOwner, STATE_ID::STATE_SKILL_ATTACK3);
			break;
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime);
		else
			return new CIdleState(m_pOwner);
	}
		
	return nullptr;
}

void CSkillState::Enter(void)
{
	__super::Enter();

	if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
	{
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(1);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HITENSYUOUKU_START);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(2);
				break;
			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HIENZIN);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(3);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_AKIZAME);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(4);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(5);
				break;
			}
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
