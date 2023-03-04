#include "stdafx.h"
#include "PlayerHitState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

using namespace Player;

CHitState::CHitState(CPlayer * pPlayer, _vector vCauserPos, _bool isDown, _float fTime)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_fTime = fTime;
	m_bIsDown = isDown;
	m_vCauserPos = vCauserPos;
}

CPlayerState * CHitState::HandleInput()
{
	return nullptr;
}

CPlayerState * CHitState::Tick(_float fTimeDelta)
{
	if (m_bIsDown)
	{
		if (Move(fTimeDelta))
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);
		else
			m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
		
		m_pOwner->Check_Navigation_Jump();
	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);
		m_pOwner->Check_Navigation_Jump();
	}

	return nullptr;
}

CPlayerState * CHitState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_bIsDown)
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
		else
		{
			if (m_bIsFly)
				return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE, m_fTime);
			else
				return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
		}
	}

	return nullptr;
}

void CHitState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_HIT;

	if (m_bIsDown)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DOWN_B);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DOWN_B);
			break;
		case CPlayer::RINWELL:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DOWN_B);
			break;
		case CPlayer::LAW:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DOWN_B);
			break;
		}
	}
	else
	{
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
	}

	m_pOwner->Set_Manarecover(false);

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((3.6f * 3.6f) * (2.f / 9.f)) * -0.5f);
		m_fTime = 3.6f / 4.5f;
	}
	else
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CHitState::Exit()
{
	__super::Exit();

	m_bIsDown = false;
	m_bIsLook = false;

	if (m_bIsFly)
		m_pOwner->Off_IsFly();

	m_pOwner->Set_TakeDamage_DelayFinish();
}

_bool CHitState::Move(_float fTimeDelta)
{
	_vector vDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * -1.f;

	if (!m_bIsLook)
	{
		m_pOwner->Get_Transform()->LookAtExceptY(m_vCauserPos);
		m_bIsLook = true;

		vDir = XMVector4Normalize(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vCauserPos);
	}

	m_fTime += fTimeDelta * 3.8f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 2.6f, 3.f, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));

	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float EndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (EndHeight > XMVectorGetY(vPosition))
		return true;
	else
		m_pOwner->Get_Transform()->Go_PosDir(fTimeDelta * 4.f, vDir, m_pOwner->Get_Navigation());

	return false;
}
