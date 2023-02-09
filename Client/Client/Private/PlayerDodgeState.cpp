#include "stdafx.h"
#include "..\Public\PlayerDodgeState.h"

#include "GameInstance.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"

#include "CloseChaseState.h"
#include "AlphenAttackState.h"
#include "AlphenSkillState.h"

#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"

using namespace Player;

CDodgeState::CDodgeState(CPlayer * pPlayer, DIRID eDir, _float fStartHeight, _float fTime)
	: m_eDirection(eDir)
	, m_fStartHeight(fStartHeight)
	, m_fTime(fTime)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CDodgeState::HandleInput(void)
{
	if (m_bIsAnimationFinished)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		{
			if (m_bIsFly)
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);
					break;
				case CPlayer::SION:
					return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);
					break;
				case CPlayer::RINWELL:
					break;
				}
			}
			else
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
					break;
				case CPlayer::SION:
					return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				case CPlayer::RINWELL:
					break;
				}
			}
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (m_bIsFly)
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					if (pGameInstance->Key_Down(DIK_E))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fStartHeight, m_fTime);
					if (pGameInstance->Key_Down(DIK_R))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fStartHeight, m_fTime);
					if (pGameInstance->Key_Down(DIK_F))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fStartHeight, m_fTime);
					break;
				case CPlayer::SION:
					if (pGameInstance->Key_Down(DIK_E))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E, m_fStartHeight, m_fTime);
					else if (pGameInstance->Key_Down(DIK_R))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R, m_fStartHeight, m_fTime);
					else if (pGameInstance->Key_Down(DIK_F))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F, m_fStartHeight, m_fTime);
					break;
				}
			}
			else
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					if (pGameInstance->Key_Down(DIK_E))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
					if (pGameInstance->Key_Down(DIK_R))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
					if (pGameInstance->Key_Down(DIK_F))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
					break;
				case CPlayer::SION:
					if (pGameInstance->Key_Down(DIK_E))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
					else if (pGameInstance->Key_Down(DIK_R))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
					else if (pGameInstance->Key_Down(DIK_F))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);
					break;
				}
			}
		}

		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f, CJumpState::JUMPTYPE::JUMP_BATTLE);

		if (!m_bIsFly)
		{
			if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_BACKWARD_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_S))
				return new CRunState(m_pOwner, DIR_BACKWARD, pGameInstance->Key_Pressing(DIK_LSHIFT));
			else if (pGameInstance->Key_Pressing(DIK_W))
				return new CRunState(m_pOwner, DIR_STRAIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		}	
	}

	return nullptr;
}

CPlayerState * CDodgeState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.025f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
		else
			m_pOwner->Check_Navigation_Jump();
	}

	if (m_bIsFly)
		m_fTime += fTimeDelta;

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			{
				m_pOwner->On_JustDodge();
				break;
			}
		}
		else
			m_pOwner->Off_JustDodge();
	}

	return nullptr;
}

CPlayerState * CDodgeState::LateTick(_float ftimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner);
	}

	return nullptr;
}

void CDodgeState::Enter(void)
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_DODGE;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_AIR);

		else
		{
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_FRONT);
				break;
			case Client::DIR_BACKWARD:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_BACK);
				break;
			}
		}
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE);
		break;
	case CPlayer::RINWELL:
		break;
	default:
		break;
	}
}

void CDodgeState::Exit(void)
{
	__super::Exit();
}

void CDodgeState::Move(_float fTimeDelta)
{

}
