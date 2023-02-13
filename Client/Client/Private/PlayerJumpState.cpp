#include "stdafx.h"

#include "GameInstance.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"

#include "CloseChaseState.h"
#include "AlphenAttackState.h"
#include "AlphenSkillState.h"

using namespace Player;

CJumpState::CJumpState(CPlayer* pPlayer, _float fStartHeight, STATETYPE eType, _float fTime, JUMPTYPE eJumpType)
{
	m_pOwner = pPlayer;
	m_fStartHeight = fStartHeight;
	m_eStateType = eType;
	m_fTime = fTime;
	m_eJumpType = eJumpType;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CJumpState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);

			case CPlayer::SION:
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);
			}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (pGameInstance->Key_Down(DIK_E))
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fStartHeight, m_fTime);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fStartHeight, m_fTime);
				else if (pGameInstance->Key_Down(DIK_F))
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

		if (pGameInstance->Key_Pressing(DIK_LSHIFT))
		{
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
			case Client::DIR_BACKWARD:
				return new CDodgeState(m_pOwner, DIR_STRAIGHT);
				break;
			case Client::DIR_END:
				return new CDodgeState(m_pOwner, DIR_BACKWARD);
				break;
			}
		}
	}

	if (JUMP_RUN == m_eJumpType)
	{
		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_STRAIGHT_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_STRAIGHT_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_BACKWARD_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_BACKWARD_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_A))
			m_eDirection = DIR_LEFT;
		else if (pGameInstance->Key_Pressing(DIK_D))
			m_eDirection = DIR_RIGHT;
		else if (pGameInstance->Key_Pressing(DIK_S))
			m_eDirection = DIR_BACKWARD;
		else if (pGameInstance->Key_Pressing(DIK_W))
			m_eDirection = DIR_STRAIGHT;
		else
			m_eDirection = DIR_END;
	}

	return nullptr;
}

CPlayerState * CJumpState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	Move(fTimeDelta);

	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (STATETYPE_END == m_eStateType)
		m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CPlayerState * CJumpState::LateTick(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner, m_fStartHeight, m_fTime);

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		if (m_bIsDrop)
		{
			switch (m_eJumpType)
			{
			case Client::Player::CJumpState::JUMP_IDLE:
				if (CPlayer::ALPHEN == m_ePlayerID) 
				{
					if (Check_JumpEnd(1.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
				}
				else if (CPlayer::SION == m_ePlayerID)
				{

				}
				break;
			case Client::Player::CJumpState::JUMP_RUN:
				if (CPlayer::ALPHEN == m_ePlayerID)
				{
					if (Check_JumpEnd(0.5f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
						m_eStateType = STATETYPE_END;
					}
					else if (m_bIsAnimationFinished)
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
				}
				else if (CPlayer::SION == m_ePlayerID)
				{

				}
				break;
			case Client::Player::CJumpState::JUMP_BATTLE:
				if (CPlayer::ALPHEN == m_ePlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
						m_eStateType = STATETYPE_END;
					}	
				}
				else if (CPlayer::SION == m_ePlayerID)
				{

				}
				break;
			}
		}
		break;
	case Client::STATETYPE_MAIN:
		if (Check_JumpEnd(1.f))
		{
			m_eStateType = STATETYPE_END;
			switch (m_eJumpType)
			{
			case Client::Player::CJumpState::JUMP_IDLE:
				if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
				else if (CPlayer::SION == m_pOwner->Get_PlayerID())
				{
				}
				break;
			case Client::Player::CJumpState::JUMP_RUN:
				if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
				else if (CPlayer::SION == m_pOwner->Get_PlayerID())
				{
				}
				break;
			}
		}
		break;
	case Client::STATETYPE_END:
		if (m_bIsAnimationFinished)
		{
			CPlayerState* pEvent = EventInput();
			if (nullptr != pEvent)
				return pEvent;
			else
				return new CIdleState(m_pOwner, CIdleState::IDLE_SIDE);
		}
		break;
	}

	return nullptr;
}

CPlayerState * CJumpState::EventInput(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
				break;
			case CPlayer::SION:
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
				//for Sion State//
				break;
			}
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (pGameInstance->Key_Down(DIK_E))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::SION:
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);

				break;
			}

		}
	}

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

	return nullptr;
}

void CJumpState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_JUMP;

	m_pOwner->On_IsFly();

	if (JUMP_IDLE == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LANDING);
			break;
		}
	}
	else if (JUMP_RUN == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_END);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_START);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_RUN_END);
			break;
		}
	}
	else if (JUMP_BATTLE == m_eJumpType)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
			break;
		case CPlayer::RINWELL:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_MAIN)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::JUMP_LOOP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
			break;
		}
	}

	m_pOwner->Set_Manarecover(true);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_JumpStart.wav"), SOUND_FOOT, 0.4f);
}

void CJumpState::Exit()
{
	if (STATETYPE_END == m_eStateType && Check_JumpEnd(0.1f))
	{
 		m_pOwner->Off_IsFly();
		m_fTime = 0.f;
	}

	m_bIsDrop = false;

	CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
}

_bool CJumpState::Check_JumpEnd(_float fOffset)
{
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fEndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (m_fEndHeight + fOffset > XMVectorGetY(vPosition))
		return true;

	return false;
}

void CJumpState::Move(_float fTimeDelta)
{
	_vector vPrePos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	m_fTime += fTimeDelta * 3.5f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 7.f, 4.6f, m_fStartHeight, m_fEndHeight);

	_vector vCurPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float4 fChangeHeight;
	XMStoreFloat4(&fChangeHeight, (vCurPos - vPrePos));

	if (0 > fChangeHeight.y)
		m_bIsDrop = true;
	else
		m_bIsDrop = false;

	if ((JUMP_IDLE != m_eJumpType) && (m_eDirection != DIR_END))
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 3.f);
}