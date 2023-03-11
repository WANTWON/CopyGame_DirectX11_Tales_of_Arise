#include "stdafx.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "PlayerPoseState.h"

#include "Effect.h"
#include "EffectTexture.h"
#include "PlayerJumpState.h"
#include "PlayerCollectState.h"

#include "CloseChaseState.h"
#include "AlphenAttackState.h"

#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"
#include "Player_RinwellNormalAttack_State.h"
#include "Player_RinwellSkillAttack_State.h"

#include "Level_Restaurant.h"

using namespace Player;

CIdleState::CIdleState(CPlayer* pPlayer, IDLETYPE eIdleType)
{
	m_pOwner = pPlayer;
	m_eIdleType = eIdleType;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CIdleState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		if ((CPlayer::ALPHEN == m_pOwner->Get_PlayerID()) || (CPlayer::LAW == m_pOwner->Get_PlayerID()))
		{
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
		}
		else if (CPlayer::SION == m_pOwner->Get_PlayerID())
		{
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
		}
		else if (CPlayer::RINWELL == m_pOwner->Get_PlayerID())
		{
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
		}
			
		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (pGameInstance->Key_Down(DIK_E))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
				if (pGameInstance->Key_Down(DIK_R))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
				if (pGameInstance->Key_Down(DIK_F))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::SION:
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::RINWELL: //For Rinwell State
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK5);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK6);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			case CPlayer::LAW:
				if (pGameInstance->Key_Down(DIK_E))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
				if (pGameInstance->Key_Down(DIK_R))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
				if (pGameInstance->Key_Down(DIK_F))
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
				break;
			}
		}

		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMPTYPE::JUMP_BATTLE);

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_LEFT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_RIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT) && !m_bIsFly)
			return new CDodgeState(m_pOwner, DIR_END, m_fTime);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT) && m_bIsFly)
			return new CDodgeState(m_pOwner, DIR_STRAIGHT, m_fTime);
	}
	else if ((LEVEL_RESTAURANT == pGameInstance->Get_CurrentLevelIndex()) && ((CLevel_Restaurant*)(pGameInstance->Get_CurrentLevel()))->Get_MiniGameStart())
	{
		if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
			return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
	}
	else
	{
		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMPTYPE::JUMP_IDLE);
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
	{
		return new CRunState(m_pOwner, DIR_STRAIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));

	}
		
	return nullptr;
}

CPlayerState * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	if(!g_bEnd)
		m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CIdleState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished && !CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		switch (m_eIdleType)
		{
		case Client::Player::CIdleState::IDLE_MAIN:
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE_TO_IDLE_SIDE);
				break;
			case CPlayer::SION:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE_TRANS_IDLE);
				break;
			case CPlayer::RINWELL:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::IDLE_TO_IDLE_SIDE);
				break;
			case CPlayer::LAW:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::IDLE_TRANS_IDLE_SIDE);
				break;
			}
			m_eIdleType = IDLE_MAIN_TO_SIDE;
			break;
		case Client::Player::CIdleState::IDLE_MAIN_TO_SIDE:
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE_SIDE);
				break;
			case CPlayer::SION:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE_CHARA);
				break;
			case CPlayer::RINWELL:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::IDLE_SIDE);
				break;
			case CPlayer::LAW:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::IDLE_SIDE);
				break;
			}
			m_eIdleType = IDLE_SIDE;
			break;
		}
	}

	return nullptr;
}

void CIdleState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_IDLE;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_IDLE);
		else if ((LEVEL_RESTAURANT == CGameInstance::Get_Instance()->Get_CurrentLevelIndex()) && ((CLevel_Restaurant*)(CGameInstance::Get_Instance()->Get_CurrentLevel()))->Get_MiniGameStart())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_IDLE);
		else
		{
			switch (m_eIdleType)
			{
			case Client::Player::CIdleState::IDLE_MAIN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE);
				break;
			case Client::Player::CIdleState::IDLE_SIDE:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE_SIDE);
				break;
			}
		}
		break;
	case CPlayer::SION:
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_IDLE);
		else
		{
			switch (m_eIdleType)
			{
			case Client::Player::CIdleState::IDLE_MAIN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE);
				break;
			case Client::Player::CIdleState::IDLE_SIDE:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE_CHARA);
				break;
			}
		}
		break;
	case CPlayer::RINWELL:
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_LOOP);
		else
		{
			switch (m_eIdleType)
			{
			case Client::Player::CIdleState::IDLE_MAIN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::IDLE);
				break;
			case Client::Player::CIdleState::IDLE_SIDE:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::IDLE_SIDE);
				break;
			}
		}
		break;
	case CPlayer::LAW:
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_IDLE);
		else
		{
			switch (m_eIdleType)
			{
			case Client::Player::CIdleState::IDLE_MAIN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::IDLE);
				break;
			case Client::Player::CIdleState::IDLE_SIDE:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::IDLE_SIDE);
				break;
			}
		}
		break;
	}
	
	m_pOwner->Set_Manarecover(true);
}

void CIdleState::Exit()
{
	__super::Exit();
}