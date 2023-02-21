#include "stdafx.h"
#include "..\Public\AI_JumpState.h"

#include "GameInstance.h"

#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"

#include "CloseChaseState.h"
#include "AlphenAttackState.h"
#include "AlphenSkillState.h"
#include "Player_RinwellNormalAttack_State.h"

#include "LawAttackState.h"
#include "LawSkillState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"

#include "AICheckState.h"
#include "AI_Sion_SkillState.h"
#include "AI_Alphen_SkillAttackState.h"
#include "AI_Rinwell_SkillState.h"

#include "AI_LAW_AIRSKILLR.h"
#include "AI_LAW_AIRSKILLF.h"
using namespace AIPlayer;

CAI_JumpState::CAI_JumpState(CPlayer* pPlayer, STATETYPE eType, _bool useskill , _float fTime )
{
	m_pOwner = pPlayer;
	m_eStateType = eType;
	//m_eJumpType = eJumpType;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	m_fTime = fTime;

	m_bUseskill = useskill;
}



CAIState * CAI_JumpState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

	
		/*m_bIsJump = true;

	if (m_bIsJump)*/
	Move(fTimeDelta);

	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	if (STATETYPE_END == m_eStateType)
		m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CAIState * CAI_JumpState::LateTick(_float fTimeDelta)
{
	/*if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner, m_fTime);*/
	if (m_fTime >= 7.f / 4.6f && m_bUseskill)
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			switch (rand() % 3)
			{
			case 0:
				return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN, m_fTime);
				break;

			case 1:
				return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA, m_fTime);
				break;

			case 2:
				return new CAI_Alphen_SkillAttackState(m_pOwner, STATE_ATTACK, m_pTarget, CAlphen::ANIM::ANIM_ATTACK_ENGETU, m_fTime);
				break;

			}
			break;
		case CPlayer::LAW:
			switch (rand() % 2)
			{
			case 0:
				return new CAI_LAW_AIRSKILLR(m_pOwner, STATE_SKILL_ATTACK_R);
				break;

			case 1:
				return new CAI_LAW_AIRSKILLF(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			}

			break;
		case CPlayer::SION:
			return new CAI_Sion_SkillState(m_pOwner, STATE_ATTACK, m_pTarget, CSion::ANIM::BTL_ATTACK_TRESVENTOS, m_fTime);
			break;

		case CPlayer::RINWELL:
			return new CAI_Rinwell_SkillState(m_pOwner, STATE_BANGJEON, m_pTarget , m_fTime);
			break;
		}
	}

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		if (m_bIsDrop)
		{
			
				if (CPlayer::ALPHEN == m_eCurrentPlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
						m_eStateType = STATETYPE_END;
					}

				}
				else if (CPlayer::SION == m_eCurrentPlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}

				}
				else if (CPlayer::RINWELL == m_eCurrentPlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}

				}
				else if (CPlayer::LAW == m_eCurrentPlayerID)
				{
					if (Check_JumpEnd(0.f))
					{
						m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_LAND);
						m_eStateType = STATETYPE_END;
					}
				}

				
			}
		break;
	
	case Client::STATETYPE_END:
		if (m_bIsAnimationFinished)
			return new CAICheckState(m_pOwner, m_eStateId);
		break;
	}

	return nullptr;
}

CAIState * CAI_JumpState::EventInput(void)
{
	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	//
	//	if (GetKeyState(VK_LBUTTON) < 0)
	//	{
	//		switch (m_eCurrentPlayerID)
	//		{
	//		case CPlayer::ALPHEN:
	//		case CPlayer::LAW:
	////			return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
	//			break;
	//		case CPlayer::SION:
	//	//		return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
	//			//for Sion State//
	//			break;

	//		case CPlayer::RINWELL:
	//	//		return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
	//			break;
	//		}
	//	}

	//	/* Skill */
	//	if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
	//	{
	//		
	//	}

		

	return nullptr;
}

void CAI_JumpState::Enter()
{
	//__super::Enter();

	m_eStateId = STATE_ID::STATE_JUMP;

	m_pOwner->On_IsFly();

		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
			break;
		case CPlayer::SION:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
			break;
		case CPlayer::RINWELL:
			if (m_eStateType == STATETYPE_START)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_JUMP);
			else if (m_eStateType == STATETYPE_END)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_LAND);
			break;
		case CPlayer::LAW:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_JUMP);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_LAND);
			break;
		}
	

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((7.f * 7.f) * (5.f / 23.f)) * -0.5f);
		m_fTime = 7.f / 4.6f;
	}
	else
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(true);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_JumpStart.wav"), SOUND_FOOT, 0.4f);
}

void CAI_JumpState::Exit()
{
	if (STATETYPE_END == m_eStateType && Check_JumpEnd(1.f))
	{
		m_pOwner->Off_IsFly();
		m_fTime = 0.f;
	}

	m_bIsDrop = false;

	CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
}

_bool CAI_JumpState::Check_JumpEnd(_float fOffset)
{
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float EndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (EndHeight + fOffset > XMVectorGetY(vPosition))
		return true;

	return false;
}

void CAI_JumpState::Move(_float fTimeDelta)
{
	_vector vPrePos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	m_fTime += fTimeDelta * 3.5f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 7.0f, 4.6f, m_fStartHeight);

	_vector vCurPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_float4 fChangeHeight;
	XMStoreFloat4(&fChangeHeight, (vCurPos - vPrePos));

	if (0 > fChangeHeight.y)
		m_bIsDrop = true;
	else
		m_bIsDrop = false;

	
	/*else if ((JUMP_BATTLE == m_eJumpType) && (m_eDirection != DIR_END) && (STATETYPE_END != m_eStateType))
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 3.f);*/
}