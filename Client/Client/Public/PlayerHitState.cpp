#include "stdafx.h"
#include "PlayerHitState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

using namespace Player;

CHitState::CHitState(CPlayer* pPlayer, _vector vCauserPos, _float fMoveLength, HITTYPE eHitType, STATETYPE eStateType)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_eHitType = eHitType;
	m_vCauserPos = vCauserPos;
	m_eStateType = eStateType;
	m_fMoveLength = fMoveLength;
}

CPlayerState * CHitState::HandleInput()
{
	return nullptr;
}

CPlayerState * CHitState::Tick(_float fTimeDelta)
{
	switch (m_eHitType)
	{
	case HIT_NORMAL:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);
		Move(fTimeDelta);
		break;
	case HIT_AIR:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);
		if (!m_bIsMove)
			Move(fTimeDelta);
		break;
	case HIT_DOWN:
		if (!m_bIsMove)
		{
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
			Move(fTimeDelta);
		}
		else
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);
		break;
	}

	m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CPlayerState * CHitState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_bIsMove && (STATETYPE_START == m_eStateType))
		{
			m_eStateType = STATETYPE_END;
			Enter();
		}
		else
		{
			if (m_bIsFly)
				return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, 1.f);
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

	switch (m_eHitType)
	{
	case HIT_NORMAL:
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
		break;
	case HIT_AIR:
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DAMAGE_AIR_SMALL_B);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DAMAGE_AIR_LARGE_B);
			break;
		case CPlayer::RINWELL:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DAMAGE_AIR_LARGE_B);
			break;
		case CPlayer::LAW:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DAMAGE_AIR_SMALL_B);
			break;
		}
		break;
	case HIT_DOWN:
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DOWN_B);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ARISE_B);
			break;
		case CPlayer::SION:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DOWN_B);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ARISE_B);
			break;
		case CPlayer::RINWELL:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DOWN_B);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ARISE_B);
			break;
		case CPlayer::LAW:
			if (STATETYPE_START == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DOWN_B);
			else if (STATETYPE_END == m_eStateType)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ARISE_B);
			break;
		}
		break;
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

	m_bIsMove = false;

	if (m_bIsFly)
		m_pOwner->Off_IsFly();

	m_pOwner->Set_TakeDamage_DelayFinish();
}

_bool CHitState::Move(_float fTimeDelta)
{
	m_pOwner->Get_Transform()->LookAtExceptY(m_vCauserPos);

	_vector vOwnerPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vDir = XMVector4Normalize(vOwnerPos - XMVectorSetY(m_vCauserPos, XMVectorGetY(vOwnerPos)));

	if (HIT_NORMAL != m_eHitType)
	{
		if (!m_pOwner->Get_IsFly())
			m_pOwner->On_IsFly();
		
		if (HIT_DOWN == m_eHitType)
			m_fTime += fTimeDelta * 3.5f;
		else
			m_fTime += fTimeDelta * 3.f;
		
		m_pOwner->Get_Transform()->Jump(m_fTime, 2.3f, 2.9f, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));

		_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_float EndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

		if (EndHeight > XMVectorGetY(vPosition))
		{
			if (m_pOwner->Get_IsFly())
				m_pOwner->Off_IsFly();

			m_bIsMove = true;
		}
	}
	
	m_pOwner->Get_Transform()->Go_PosDir(fTimeDelta * m_fMoveLength, vDir, m_pOwner->Get_Navigation());

	return m_bIsMove;
}
