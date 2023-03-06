#include "stdafx.h"
#include "..\Public\RinwellDownState.h"
#include "RinwellPoseState.h"
#include "RinwellIdleState.h"

using namespace AiRinwell;

CRinwellDownState::CRinwellDownState(CAiRinwell * pRinwell, _float fTime)
{
	m_pOwner = pRinwell;
	m_fLimitTime = fTime;
}

CRinwellState * CRinwellDownState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, true, "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;
		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_fTime += fTimeDelta;

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CRinwellState * CRinwellDownState::LateTick(_float fTimeDelta)
{
	if (m_fLimitTime < m_fTime)
	{
		if (m_pOwner->Get_Stats().m_fCurrentHp < (m_pOwner->Get_Stats().m_fMaxHp * 0.5f))
		{
			if (!m_pOwner->Get_AirMode())
			{
				m_pOwner->Set_AirMode(true);
				return new CPoseState(m_pOwner, CRinwellState::STATE_HP50DOWN);
			}
			else
				return new CRinwellIdleState(m_pOwner, 0.f);
		}
		else
			return new CRinwellIdleState(m_pOwner, 0.f);
	}

	return nullptr;
}

void CRinwellDownState::Enter(void)
{
	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_DAMAGE_AIR_LOOP);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_DAMAGE_LOOP);
}

void CRinwellDownState::Exit(void)
{
	m_fTime = 0.f;
} 