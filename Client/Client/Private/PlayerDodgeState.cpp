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
#include "Monster.h"
#include "PlayerOverlimit.h"


#include "Player_RinwellNormalAttack_State.h"
#include "Player_RinwellSkillAttack_State.h"

#include "LawAttackState.h"
#include "LawSkillState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"
#include "Animation.h"

using namespace Player;

CDodgeState::CDodgeState(CPlayer * pPlayer, DIRID eDir, _float fTime)
	: m_eDirection(eDir)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_fTime = fTime;
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
					return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::SION:
					return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::RINWELL:
					return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				case CPlayer::LAW:
					return new CLawAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
					break;
				}
			}
			else
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
				case CPlayer::LAW:
					return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
					break;
				case CPlayer::SION:
					return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				case CPlayer::RINWELL:
					return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				}
			}
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
		{
			if (m_bIsFly)
			{
				switch (m_ePlayerID)
				{
				case CPlayer::ALPHEN:
					if (pGameInstance->Key_Down(DIK_E))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
					if (pGameInstance->Key_Down(DIK_R))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
					if (pGameInstance->Key_Down(DIK_F))
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
					break;
				case CPlayer::SION:
					if (pGameInstance->Key_Down(DIK_E))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
					else if (pGameInstance->Key_Down(DIK_R))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
					else if (pGameInstance->Key_Down(DIK_F))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
					break;
				case CPlayer::RINWELL:
					if (pGameInstance->Key_Down(DIK_E))
						return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
					break;
				case CPlayer::LAW:
					if (pGameInstance->Key_Down(DIK_E))
						return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
					else if (pGameInstance->Key_Down(DIK_R))
						return new CLawAirRSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
					else if (pGameInstance->Key_Down(DIK_F))
						return new CLawAirFSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
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
				case CPlayer::RINWELL:
					if (pGameInstance->Key_Down(DIK_E))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
					else if (pGameInstance->Key_Down(DIK_R))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
					else if (pGameInstance->Key_Down(DIK_F))
						return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);
					break;
				case CPlayer::LAW:
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

		if (!m_bIsFly)
		{
			if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT);
			else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT);
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_BACKWARD_LEFT);
			else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT);
			else if (pGameInstance->Key_Pressing(DIK_A))
				return new CRunState(m_pOwner, DIR_LEFT);
			else if (pGameInstance->Key_Pressing(DIK_D))
				return new CRunState(m_pOwner, DIR_RIGHT);
			else if (pGameInstance->Key_Pressing(DIK_S))
				return new CRunState(m_pOwner, DIR_BACKWARD);
			else if (pGameInstance->Key_Pressing(DIK_W))
				return new CRunState(m_pOwner, DIR_STRAIGHT);
		}
	}

	return nullptr;
}

CPlayerState * CDodgeState::Tick(_float fTimeDelta)
{
	if (m_bDodgeEffect)
		DodgeEffect();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

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

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			{
				if (nullptr == m_pDodgeCollider)
				{
					m_fEffectEventEndTime = pEvent.fEndTime;
					m_fEffectEventCurTime = m_pOwner->Get_Model()->Get_Animations()[m_pOwner->Get_Model()->Get_CurrentAnimIndex()]->Get_CurrentTime();

					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
					ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
					ColliderDesc.vPosition = _float3(0.f, 2.5f, 0.f);

					m_pDodgeCollider = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
		}
		else
		{
			if ((ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType) && (m_pOwner->Get_IsJustDodge() == true))
			{
				m_pOwner->Off_JustDodge();

				CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 0.3f);

				if (nullptr != m_pDodgeCollider)
				{
					CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pDodgeCollider);
					m_pDodgeCollider = nullptr;
				}
			}
		}
	}

	if (nullptr != m_pDodgeCollider)
		m_pDodgeCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CPlayerState * CDodgeState::LateTick(_float ftimeDelta)
{
	if (nullptr != m_pDodgeCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pDodgeCollider, &pCollisionTarget) ||
			CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pDodgeCollider, &pCollisionTarget))
		{
			m_bDodgeEffect = true;

			m_pOwner->On_JustDodge();

			CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 0.3f);

			if (m_bJustEffectOnce)
			{
				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_JustDodgeEffect"), LEVEL_STATIC, TEXT("ddd"), &m_pOwner)))
					return OBJ_NOEVENT;
				m_bJustEffectOnce = false;
			}

			CCollision_Manager::Get_Instance()->Collect_Collider(CCollider::TYPE_SPHERE, m_pDodgeCollider);
			m_pDodgeCollider = nullptr;

			if (!m_bIncreaseOverLimit)
			{
				m_pOwner->Plus_Overcount();
				m_bIncreaseOverLimit = true;
			}
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pDodgeCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
		if (m_pOwner->Get_Info().idodgecount >= 2)
		{
		    CBaseObj* pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
			(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			
			m_pOwner->Set_Overcount(0);
			return new CPlayerOverlimit(m_pOwner);
		}

		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CDodgeState::EventInput(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

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
	if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
		case CPlayer::LAW:
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

	return nullptr;
}

void CDodgeState::Enter(void)
{
	__super::Enter();
	m_bJustEffectOnce = true;
	m_eStateId = STATE_ID::STATE_DODGE;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_AIR);
		else
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DodgeSound.wav"), SOUND_EFFECT, 1.0f);
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
			case Client::DIR_BACKWARD:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_FRONT);
				break;
			case Client::DIR_END:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_BACK);
				break;
			}
		}
		break;
	case CPlayer::SION:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_AIR);
		else
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DodgeSound.wav"), SOUND_EFFECT, 1.0f);
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
			case Client::DIR_BACKWARD:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND);
				break;
			case Client::DIR_END:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND_BACK);
				break;
			}
		}
		break;
	case CPlayer::RINWELL:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_AIR);

		else
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DodgeSound.wav"), SOUND_EFFECT, 1.0f);
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
			case Client::DIR_BACKWARD:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND);
				break;
			case Client::DIR_END:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND_BACK);
				break;
			}
		}
		break;
	case CPlayer::LAW:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_AIR);
		else
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DodgeSound.wav"), SOUND_EFFECT, 1.0f);
			switch (m_eDirection)
			{
			case Client::DIR_STRAIGHT:
			case Client::DIR_LEFT:
			case Client::DIR_RIGHT:
			case Client::DIR_STRAIGHT_LEFT:
			case Client::DIR_STRAIGHT_RIGHT:
			case Client::DIR_BACKWARD:
			case Client::DIR_BACKWARD_LEFT:
			case Client::DIR_BACKWARD_RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND);
				break;
			case Client::DIR_END:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND_BACK);
				break;
			}
		}
		break;
	}

	m_pOwner->Set_Manarecover(true);
}

void CDodgeState::Exit(void)
{
	__super::Exit();

	if (m_bDodgeEffect)
		m_pOwner->Set_DodgeEffect(true);

	m_bIncreaseOverLimit = false;

	Safe_Release(m_pDodgeCollider);

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CDodgeState::DodgeEffect()
{
#pragma region Dodge Effect
	_float fDuration = m_fEffectEventEndTime - m_fEffectEventCurTime;
	_float fCurrentTime = m_pOwner->Get_Model()->Get_Animations()[m_pOwner->Get_Model()->Get_CurrentAnimIndex()]->Get_CurrentTime();
	
	_float fInterpTimer = fDuration * 0.15f;

	/* Saturation Lerp */
	_float fSaturationInterpFactor = fCurrentTime / fInterpTimer;
	if (fSaturationInterpFactor > 1.f)
		fSaturationInterpFactor = 1.f;

	_float fSaturationStart = 1.5f;
	_float fSaturationEnd = 0.5f;
	_float fSaturationLerp = fSaturationStart + fSaturationInterpFactor * (fSaturationEnd - fSaturationStart);
	m_pOwner->Get_Renderer()->Set_Saturation(true, fSaturationLerp);

	/* Zoom Blur Lerp */
	_float fFocusPower = 5.f;

	_float fBlurInterpFactor = fCurrentTime / fInterpTimer;
	if (fBlurInterpFactor > 1.f)
		fBlurInterpFactor = 1.f;

	_int iDetailStart = 1;
	_int iDetailEnd = 10;
	_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
	m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
#pragma endregion Dodge Effect
}
