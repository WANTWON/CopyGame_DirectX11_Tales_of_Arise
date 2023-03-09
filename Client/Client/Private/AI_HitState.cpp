#include "stdafx.h"
#include "..\Public\AI_HitState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Rinwell.h"
#include "Sion.h"
#include "Law.h"
#include "AICheckState.h"

using namespace AIPlayer;


CAI_HitState::CAI_HitState(CPlayer* pPlayer, _vector vCauserPos, _bool isDown , _float fTime , STATE_ID eStateID )
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	m_bIsDown = isDown;
	m_vCauserPos = vCauserPos;
	m_eStateId = eStateID;
}

CAIState * CAI_HitState::Tick(_float fTimeDelta)
{
	if (m_eStateId == STATE_SMASHHIT)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, false);
	else
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
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
			m_pOwner->Check_Navigation_Jump();
			//m_pOwner->Check_Navigation();
		}
	}
	return nullptr;
}

CAIState * CAI_HitState::LateTick(_float fTimeDelta)
{

	if (m_bIsAnimationFinished && m_eStateId != STATE_SMASHHIT)
		return new CAICheckState(m_pOwner, m_eStateId);
	if (m_bIsAnimationFinished && m_eStateId == STATE_SMASHHIT)
	{
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void CAI_HitState::Enter()
{
	if (m_eStateId == STATE_SMASHHIT)
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_B;
			break;
		case CPlayer::SION:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_B;
			break;
		case CPlayer::RINWELL:
			m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LARGE_B;
			break;
		case CPlayer::LAW:
			m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_LARGE_B;
			break;
		}
		m_pOwner->Get_Model()->Reset();
		return;
	}


	m_eStateId = STATE_ID::STATE_HIT;

	if (m_bIsDown)
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DOWN_B;
			break;
		case CPlayer::SION:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DOWN_B;
			break;
		case CPlayer::RINWELL:
			m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DOWN_B;
			break;
		case CPlayer::LAW:
			m_iCurrentAnimIndex = CLaw::ANIM::BTL_DOWN_B;
			break;
		}
	}

	else
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:

			switch (rand() % 8)
			{
			case 0:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_B;
				break;
			case 1:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_F;
				break;
			case 2:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_L;
				break;
			case 3:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_R;
				break;
			case 4:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_B;
				break;
			case 5:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_F;
				break;
			case 6:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_L;
				break;
			case 7:
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_R;
				break;
			}
			break;
		case CPlayer::SION:
			switch (rand() % 8)
			{
			case 0:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_B;
				break;
			case 1:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_F;
				break;
			case 2:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_L;
				break;
			case 3:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_R;
				break;
			case 4:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_B;
				break;
			case 5:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_F;
				break;
			case 6:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_L;
				break;
			case 7:
				m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_R;
				break;
			}
			break;
		case CPlayer::RINWELL:
			switch (rand() % 8)
			{
			case 0:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LARGE_B;
				break;
			case 1:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LARGE_F;
				break;
			case 2:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LARGE_L;
				break;
			case 3:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_LARGE_R;
				break;
			case 4:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_SMALL_B;
				break;
			case 5:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_SMALL_F;
				break;
			case 6:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_SMALL_L;
				break;
			case 7:
				m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DAMAGE_SMALL_R;
				break;
			}

		case CPlayer::LAW:
			switch (rand() % 8)
			{
			case 0:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_LARGE_B;
				break;
			case 1:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_LARGE_F;
				break;
			case 2:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_LARGE_L;
				break;
			case 3:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_LARGE_R;
				break;
			case 4:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_SMALL_B;
				break;
			case 5:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_SMALL_F;
				break;
			case 6:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_SMALL_L;
				break;
			case 7:
				m_iCurrentAnimIndex = CLaw::ANIM::BTL_DAMAGE_SMALL_R;
				break;
			}
			break;

		default:
			break;
		}
	}
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

	m_pOwner->Set_Manarecover(false);

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((3.6f * 3.6f) * (2.f / 9.f)) * -0.5f);
		m_fTime = 3.6f / 4.5f;
	}
	else
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));


}

void CAI_HitState::Exit()
{
	__super::Exit();

	m_bIsDown = false;
	m_bIsLook = false;

	if (m_bIsFly)
		m_pOwner->Off_IsFly();

	m_pOwner->Set_TakeDamage_DelayFinish();
}

_bool CAI_HitState::Move(_float fTimeDelta)
{
	_vector vDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * -1.f;

	if (!m_bIsLook)
	{
		m_pOwner->Get_Transform()->LookAtExceptY(m_vCauserPos);
		m_bIsLook = true;

		vDir = XMVector4Normalize(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vCauserPos);
	}
	
	m_fTime += fTimeDelta * 4.8f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 2.6f, 3.f, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));

	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float EndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (EndHeight > XMVectorGetY(vPosition))
		return true;
	else
		m_pOwner->Get_Transform()->Go_PosDir(fTimeDelta * 4.f, vDir, m_pOwner->Get_Navigation());

	return false;
}
