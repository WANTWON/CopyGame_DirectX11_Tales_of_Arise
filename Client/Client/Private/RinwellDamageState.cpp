#include "stdafx.h"
#include "RinwellDamageState.h"
#include "RinwellMoveState.h"

using namespace AiRinwell;

CDamageState::CDamageState(CAiRinwell* pRinwell, _uint eDir)
{
	m_pOwner = pRinwell;
	m_eDmgDir = eDir;
}


CRinwellState * CDamageState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	return nullptr;
}

CRinwellState * CDamageState::LateTick(_float fTimeDelta)
{
	
	if (m_bIsAnimationFinished)
	{
		return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
	}

	return nullptr;
}

void CDamageState::Enter()
{
	m_eStateId = STATE_ID::STATE_DAMAGE;

	switch (m_eDmgDir)
	{
	case FRONT:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_F);
		break;
	case BACK:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_B);
		break;
	case LEFT:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_L);
		break;
	case RIGHT:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_DAMAGE_LARGE_R);
		break;
	}
}

void CDamageState::Exit()
{
	
}

