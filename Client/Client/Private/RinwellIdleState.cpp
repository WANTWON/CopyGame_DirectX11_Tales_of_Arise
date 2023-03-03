#include "stdafx.h"
#include "..\Public\RinwellIdleState.h"

#include "RinwellMoveState.h"
#include "RinwellAttackState.h"
#include "RinwellSkillState.h"
#include "RinwellPoseState.h"
#include "RinwellTurnState.h"

using namespace AiRinwell;

CRinwellIdleState::CRinwellIdleState(CAiRinwell * pRinwell, _float fTime)
{
	m_pOwner = pRinwell;
	m_fWaitingTime = fTime;
}

CRinwellState * CRinwellIdleState::Tick(_float fTimeDelta)
{
	if (m_bAirMove)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 1.5f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;
		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	m_fTime += fTimeDelta;

	return nullptr;
}

CRinwellState * CRinwellIdleState::LateTick(_float fTimeDelta)
{
	_float fCurrentHP = m_pOwner->Get_Stats().m_fCurrentHp;
	_float fMaxHP = m_pOwner->Get_Stats().m_fMaxHp;

	if ((fCurrentHP < fMaxHP * 0.5f) && !m_pOwner->Get_AirMode())
		return new CPoseState(m_pOwner, STATE_HP50DOWN);

	if (m_fWaitingTime < m_fTime)
	{
		_vector vToTargetDir = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		_float fDistance = XMVectorGetX(XMVector3Length(vToTargetDir));
		if (fDistance < 8.f)
		{
			_float fCos = XMVectorGetX(XMVector3Dot(XMVector4Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)), XMVector4Normalize(vToTargetDir)));

			if (fCos > 0.4f)
			{
				m_pOwner->Set_SkillIndex(CRinwellState::GALEFORCE);
				return new CRinwellTurnState(m_pOwner, STATE_SKILL);
			}
			else
				return new CSkillState(m_pOwner, CRinwellState::THUNDERFIELD);
		}
		else
		{
			if (fCurrentHP < fMaxHP * 0.75f)
			{
				_int iSkill = rand() % 2;

				if (1 == iSkill)
					return new CSkillState(m_pOwner, CRinwellState::HOLY);
			}

			return new CRinwellTurnState(m_pOwner, STATE_ATTACK, STATETYPE_START);
		}
	}

	return nullptr;
}

void CRinwellIdleState::Enter(void)
{
	m_eStateId = STATE_ID::STATE_IDLE;

	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		m_bAirMove = true;
	else
		m_bAirMove = false;

	if (m_bAirMove)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_FALL);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MOVE_IDLE);

	Find_ActiveTarget();
	m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CRinwellIdleState::Exit(void)
{
	m_fTime = 0.f;
}
