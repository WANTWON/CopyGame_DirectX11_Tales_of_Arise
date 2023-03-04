#include "stdafx.h"
#include "RinwellDamageState.h"
#include "RinwellMoveState.h"
#include "RinwellPoseState.h"
#include "RinwellDownState.h"

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
	
	return nullptr;
}

CRinwellState * CDamageState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (m_eStateId == STATE_DAMAGE)
		{
			if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
				return new CPoseState(m_pOwner, CRinwellState::STATE_HP50DOWN);
			else
				return new CRinwellDownState(m_pOwner);
		}
			

		if (m_eStateId == STATE_DEAD)
		{
			m_pOwner->Set_Dissolve();
			m_pOwner->Set_GlowUp();
			return  nullptr;
		}
	}

	m_pOwner->Check_Navigation();
	return nullptr;
}

void CDamageState::Enter()
{
	if (m_eStateId == STATE_DAMAGE)
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
	else if(m_eStateId == STATE_DEAD)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DEAD);
}

void CDamageState::Exit()
{
	
}

