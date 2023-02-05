#include "stdafx.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "PlayerRunState.h"
#include "PlayerAttackNormalState.h"

#include "Effect.h"
#include "EffectTexture.h"
#include "PlayerJumpState.h"
#include "PlayerSkillState.h"
#include "PlayerCollectState.h"

using namespace Player;

CIdleState::CIdleState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CIdleState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
			return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1);

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (pGameInstance->Key_Down(DIK_E))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
			if (pGameInstance->Key_Down(DIK_R))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
			else if (pGameInstance->Key_Down(DIK_F))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
		}
	}
	
	if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
		return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f, CJumpState::JUMPTYPE::JUMP_IDLE);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
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

CPlayerState * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_IDLE;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		if (LEVEL_BATTLE == m_pOwner->Get_Level())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_IDLE);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE);
		break;
	case CPlayer::SION:
		if (LEVEL_BATTLE == m_pOwner->Get_Level())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_IDLE_LONG);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::IDLE);
		break;
	default:
		break;
	}
}

void CIdleState::Exit()
{
	__super::Exit();
}