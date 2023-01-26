#include "stdafx.h"

#include "PlayerAttackNormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"

using namespace Player;

CAttackNormalState::CAttackNormalState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CAttackNormalState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	if (!m_bIsAnimationFinished)
	{
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN"));

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	
	return nullptr;
}

CPlayerState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner);
	}

	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CSion::ANIM::ANIM_ATTACK_NORMAL_0);
		break;
	default:
		break;
	}

	

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAttackNormalState::Exit()
{
}