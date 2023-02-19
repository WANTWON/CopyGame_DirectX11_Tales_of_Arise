#include "stdafx.h"
#include "PlayerHitState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

using namespace Player;

CHitState::CHitState(CPlayer * pPlayer, _float fTime)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_fTime = fTime;
}

CPlayerState * CHitState::HandleInput()
{
	return nullptr;
}

CPlayerState * CHitState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	if(!m_bIsBack)
	{
		m_pOwner->Get_Transform()->Sliding_Backward(fTimeDelta, m_pOwner->Get_Navigation());
		m_bIsBack = true;
	}

	return nullptr;
}

CPlayerState * CHitState::LateTick(_float fTimeDelta)
{
	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner->Get_SPHERECollider()))
		return new CHitState(m_pOwner, m_fTime);

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}
			
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
	case CPlayer::RINWELL:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DAMAGE_AIR_LARGE_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DAMAGE_LARGE_B);
		break;
	case CPlayer::LAW:
		if (m_bIsFly)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DAMAGE_AIR_SMALL_B);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DAMAGE_SMALL_B);
		break;
	}

	m_pOwner->Set_Manarecover(false);
}

void CHitState::Exit()
{
	__super::Exit();

	m_bIsBack = false;
}
