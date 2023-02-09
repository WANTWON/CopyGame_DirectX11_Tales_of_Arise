#include "stdafx.h"
#include "..\Public\CloseChaseState.h"

#include "BattleManager.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"
#include "PlayerHitState.h"
#include "AlphenAttackState.h"
#include "AlphenSkillState.h"

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
		return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f, CJumpState::JUMPTYPE::JUMP_BATTLE);
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

		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2.f, m_pOwner->Get_Navigation());
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CCloseChaseState::LateTick(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner);

	if (nullptr != m_pTarget)
	{
		_vector vToTargetDir = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

		switch (m_pOwner->Get_PlayerID())
		{
		case CPlayer::ALPHEN:
			if (4.5f > XMVectorGetX(XMVector4Length(vToTargetDir)))
			{
				switch (m_eNextState)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK1:
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK1);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK2:
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK2);
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK3:
					return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK3);
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
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_RUN);
		break;
	}

	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	m_pTarget = pBattleMgr->Get_LackonMonster();

	RELEASE_INSTANCE(CBattleManager);
}

void CCloseChaseState::Exit()
{
}
