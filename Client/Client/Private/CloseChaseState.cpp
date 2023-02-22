#include "stdafx.h"
#include "..\Public\CloseChaseState.h"

#include "BattleManager.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"
#include "PlayerHitState.h"

#include "AlphenAttackState.h"
#include "AlphenSkillState.h"

#include "LawAttackState.h"
#include "LawSkillState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"

using namespace Player;

CCloseChaseState::CCloseChaseState(CPlayer * pPlayer, STATE_ID eStateType, STATE_ID eNextStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	m_eNextState = eNextStateType;
}

CPlayerState * CCloseChaseState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
		return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMPTYPE::JUMP_BATTLE);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
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

	return nullptr;
}

CPlayerState * CCloseChaseState::Tick(_float fTimeDelta)
{
	if (nullptr != m_pTarget)
	{
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f, m_pOwner->Get_Navigation());
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "Trans");

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CCloseChaseState::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pTarget)
	{
		_vector vToTargetDir = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_float fRadiusSum = m_pTarget->Get_SPHERECollider()->Get_SphereRadius() + m_pOwner->Get_SPHERECollider()->Get_SphereRadius() + 1.5f;
		switch (m_pOwner->Get_PlayerID())
		{
		case CPlayer::ALPHEN:
			if (fRadiusSum > XMVectorGetX(XMVector4Length(vToTargetDir)))
			{
				switch (m_eNextState)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
					break;
				}
			}
			break;
		case CPlayer::LAW:
			if (fRadiusSum > XMVectorGetX(XMVector4Length(vToTargetDir)))
			{
				switch (m_eNextState)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					return new CLawAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
						return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
					break;
				}
			}
			break;
		}
	}

	return nullptr;
}

CPlayerState * CCloseChaseState::EventInput(void)
{
	return nullptr;
}

void CCloseChaseState::Enter()
{
	__super::Enter();

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_RUN);
		break;
	case CPlayer::LAW:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_RUN);
		break;
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	m_pTarget = pBattleMgr->Get_LackonMonster();
}

void CCloseChaseState::Exit()
{
}
