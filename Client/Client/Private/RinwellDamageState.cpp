#include "stdafx.h"
#include "RinwellDamageState.h"
#include "RinwellMoveState.h"
#include "RinwellPoseState.h"
#include "RinwellDownState.h"
#include "RinwellIdleState.h"

using namespace AiRinwell;

CDamageState::CDamageState(CAiRinwell* pRinwell, _uint eDir, STATE_ID eStateID)
{
	m_pOwner = pRinwell;
	m_eDmgDir = eDir;
	m_eStateId = eStateID;
}

CRinwellState * CDamageState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false, "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;
		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	Move(fTimeDelta);
	
	m_pOwner->Check_Navigation();

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CRinwellState * CDamageState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_eStateId == STATE_DAMAGE)
		{
			if ((m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f) && !m_pOwner->Get_AirMode())
				return new CPoseState(m_pOwner, CRinwellState::STATE_HP50DOWN);
			else if (0.f >= m_pOwner->Get_Methor())
				return new CRinwellDownState(m_pOwner);
			else
				return new CRinwellIdleState(m_pOwner, 1.f);
		}
			
		if (m_eStateId == STATE_DEAD)
		{
			m_pOwner->Set_Dissolve();
			m_pOwner->Set_GlowUp();
			return  nullptr;
		}
	}

	return nullptr;
}

void CDamageState::Enter()
{
	if (m_eStateId == STATE_DAMAGE)
	{
		if (m_pOwner->Get_AirMode())
		{
			switch (m_eDmgDir)
			{
			case FRONT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_AIR_LARGE_B);
				break;
			case BACK:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_AIR_LARGE_F);
				break;
			case LEFT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_AIR_LARGE_L);
				break;
			case RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_AIR_LARGE_R);
				break;
			}
		}
		else
		{
			switch (m_eDmgDir)
			{
			case FRONT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_B);
				break;
			case BACK:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_F);
				break;
			case LEFT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_R);
				break;
			case RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_L);
				break;
			}
		}
	}
	else if(m_eStateId == STATE_DEAD)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DEAD);

	m_vStartPos = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 0.f);

	_vector vDir;

	switch (m_eDmgDir)
	{
	case FRONT:
		vDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * -1.f;
		break;
	case BACK:
		vDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
		break;
	case LEFT:
		vDir = m_pOwner->Get_TransformState(CTransform::STATE_RIGHT);
		break;
	case RIGHT:
		vDir = m_pOwner->Get_TransformState(CTransform::STATE_RIGHT) * -1.f;
		break;
	}

	m_vGoalPos = m_vStartPos + vDir;
}

void CDamageState::Exit()
{
	
}

void CDamageState::Move(_float fTimeDelta)
{
	_vector vOwnerPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float fOwnerPosY = XMVectorGetY(vOwnerPos);

	_float fDecrease = XMVectorGetX(XMVector4Length(XMVectorSetY(vOwnerPos, XMVectorGetY(m_vGoalPos)) - m_vStartPos)) / XMVectorGetX(XMVector4Length(m_vStartPos - m_vGoalPos));

	m_fRatio += (1.f - fDecrease) * 0.2f + fTimeDelta;

	_vector vPos = XMVectorSetY(XMVectorLerp(m_vStartPos, m_vGoalPos, m_fRatio), fOwnerPosY);
	if (m_pOwner->Get_Navigation()->isMove(vPos))
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);

	m_pOwner->Check_Navigation();
}

