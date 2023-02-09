#include "stdafx.h"
#include "PlayerHitState.h"
#include "PlayerIdleState.h"

using namespace Player;

CHitState::CHitState(CPlayer * pPlayer)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CHitState::HandleInput()
{
	return nullptr;
}

CPlayerState * CHitState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CHitState::LateTick(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner);

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner);
	
	return nullptr;
}

void CHitState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_HIT;

	switch (m_ePlayerID)
	{
	case CPlayer::ALPHEN:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DAMAGE_AIR_SMALL_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DAMAGE_SMALL_B);
		break;
	case CPlayer::SION:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DAMAGE_AIR_LARGE_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DAMAGE_LARGE_B);
		break;
	}
}

void CHitState::Exit()
{
	__super::Exit();
}
