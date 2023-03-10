#include "stdafx.h"
#include "..\Public\Monster_Lawhit.h"
#include "RinwellMoveState.h"
#include "RinwellPoseState.h"
#include "RinwellDownState.h"
#include "RinwellIdleState.h"

#include "Monster_LawPoseState.h"
#include "Monster_LawIdleState.h"

using namespace MonsterLaw;

CMonster_Lawhit::CMonster_Lawhit(CMonsterLaw* pRinwell, _uint eDir, STATE_ID eStateID)
{
	m_pOwner = pRinwell;
	m_eDmgDir = eDir;
	m_eStateId = eStateID;
}

CMonsterLawState * CMonster_Lawhit::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false, "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;
		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	Move();

	m_pOwner->Check_Navigation();

	return nullptr;
}

CMonsterLawState * CMonster_Lawhit::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_eStateId == STATE_DAMAGE)
		{
			if ((m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f))
				return new CPoseState(m_pOwner, CMonsterLawState::STATE_IDLE);
			else
				return new CMonster_LawIdleState(m_pOwner);
		}

		if (m_eStateId == STATE_DEAD)
		{
			m_pOwner->Set_GlowUp();
			return  nullptr;
		}
	}

	m_pOwner->Check_Navigation();
	return nullptr;
}

void CMonster_Lawhit::Enter()
{
	if (m_eStateId == STATE_DAMAGE)
	{
		
			switch (m_eDmgDir)
			{
			case FRONT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_DAMAGE_LARGE_B);
				break;
			case BACK:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_DAMAGE_LARGE_F);
				break;
			case LEFT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_DAMAGE_LARGE_R);
				break;
			case RIGHT:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_DAMAGE_LARGE_L);
				break;
			}
		
	}
	else if (m_eStateId == STATE_DEAD)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_DEAD);
}

void CMonster_Lawhit::Exit()
{

}

void CMonster_Lawhit::Move(void)
{
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

	m_pOwner->Get_Transform()->Go_PosDir(0.0025f, vDir, m_pOwner->Get_Navigation());

	m_pOwner->Check_Navigation();
}

