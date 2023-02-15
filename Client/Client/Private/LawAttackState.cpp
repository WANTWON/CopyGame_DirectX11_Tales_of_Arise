#include "stdafx.h"
#include "..\Public\LawAttackState.h"

#include "GameInstance.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

#include "Effect.h"

using namespace Player;

CLawAttackState::CLawAttackState(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CLawAttackState::HandleInput()
{
	return nullptr;
}

CPlayerState * CLawAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
		else
			m_pOwner->Check_Navigation_Jump();
	}

	return nullptr;
}

CPlayerState * CLawAttackState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_START, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CLawAttackState::EventInput(void)
{
	return nullptr;
}

void CLawAttackState::Enter()
{
	__super::Enter();

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_1);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_2);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_3);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_4);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_1);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_2);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_3);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_4);
			break;
		}
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	//m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);
}

void CLawAttackState::Exit()
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}
