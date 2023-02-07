#include "stdafx.h"

#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerSkillState.h"
#include "PlayerRunState.h"
#include "PlayerHitState.h"

using namespace Player;

CJumpState::CJumpState(CPlayer* pPlayer, _float fStartHeight, STATETYPE eType, _float fTime, JUMPTYPE eJumpType)
{
	m_pOwner = pPlayer;
	m_fStartHeight = fStartHeight;
	m_eStateType = eType;
	m_fTime = fTime;
	m_eJumpType = eJumpType;
}

CPlayerState * CJumpState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (pGameInstance->Key_Down(DIK_E))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1, m_fStartHeight, m_fTime);
			else if (pGameInstance->Key_Down(DIK_R))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2, m_fStartHeight, m_fTime);
			else if (pGameInstance->Key_Down(DIK_F))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3, m_fStartHeight, m_fTime);
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

	if (JUMP_IDLE == m_eJumpType)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_pOwner->Get_Model()->Get_CurrentAnimIndex())
				{
				case CAlphen::ANIM::ANIM_JUMP_START:
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsJump = true;
					break;
				case CAlphen::ANIM::ANIM_JUMP_LAND:
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						CPlayerState* pEventInput = EventInput();
						if (nullptr != pEventInput)
							return pEventInput;
					}
					break;
				}
			}
		}
	}
	else if (JUMP_RUN == m_eJumpType || JUMP_BATTLE == m_eJumpType)
		m_bIsJump = true;

	if (m_bIsJump)
		Move(fTimeDelta);

	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	if (STATETYPE_END == m_eStateType)
		m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CPlayerState * CJumpState::LateTick(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner);

	switch (m_eStateType)
	{
	case STATETYPE_START:
	{
		if (m_bIsDrop || m_bIsAnimationFinished)
		{
			if (Check_JumpEnd())
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Jumpland.wav"), SOUND_FOOT, 0.4f);
				m_eStateType = STATETYPE_END;
				if (JUMP_IDLE == m_eJumpType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
				else if (JUMP_RUN == m_eJumpType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
				else if (JUMP_BATTLE == m_eJumpType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
			}
			else
			{
				m_eStateType = STATETYPE_MAIN; 
				if (JUMP_RUN == m_eJumpType)
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
				else
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
			}
		}
		break;
	}
	case STATETYPE_MAIN:
		if (Check_JumpEnd())
		{CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Jumpland.wav"), SOUND_FOOT, 0.4f);
			m_eStateType = STATETYPE_END;
			if (JUMP_IDLE == m_eJumpType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
			else if (JUMP_RUN == m_eJumpType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LAND);
			else if (JUMP_BATTLE == m_eJumpType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
		}
		break;
	case STATETYPE_END:
		if (m_bIsAnimationFinished)
		{
			m_bIsJump = false;
			return new CIdleState(m_pOwner);
		}
	}

	return nullptr;
}

CPlayerState * CJumpState::EventInput(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1);

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (pGameInstance->Key_Down(DIK_E))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
			else if (pGameInstance->Key_Down(DIK_R))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
			else if (pGameInstance->Key_Down(DIK_F))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
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

	CPlayer::PLAYERID ePlayerID = m_pOwner->Get_PlayerID();

	if (JUMP_IDLE == m_eJumpType)
	{
		switch (m_eStateType)
		{
		case STATETYPE_START:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (LEVEL_BATTLE == m_pOwner->Get_Level())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
				else
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_START);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_MAIN:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_END:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				if (LEVEL_BATTLE == m_pOwner->Get_Level())
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
				else
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
		}
	}
	else if (JUMP_RUN == m_eJumpType)	
	{
		switch (m_eStateType)
		{
		case STATETYPE_START:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_START);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_MAIN:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_LOOP);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_END:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_RUN_END);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		}
	}
	else if (JUMP_BATTLE == m_eJumpType)
	{
		switch (m_eStateType)
		{
		case STATETYPE_START:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_MAIN:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		case STATETYPE_END:
			switch (ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
				break;
			case CPlayer::SION:
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
				break;
			}
			break;
		}
	}

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_JumpStart.wav"), SOUND_FOOT, 0.4f);
}

void CJumpState::Exit()
{
	m_fTime = 0.f;

	m_pOwner->Off_IsFly();

	m_bIsDrop = false;

	CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
}

_bool CJumpState::Check_JumpEnd()
{
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(vPosition);
	m_fEndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (m_fEndHeight + 4.f > XMVectorGetY(vPosition))
		return true;

	return false;
}

void CJumpState::Move(_float fTimeDelta)
{
	_vector vPrePos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	m_fTime += 0.1f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 5.f, 2.0f, m_fStartHeight, m_fEndHeight);

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