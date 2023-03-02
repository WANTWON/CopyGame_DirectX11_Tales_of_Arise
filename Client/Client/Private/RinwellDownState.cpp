#include "stdafx.h"
#include "..\Public\RinwellDownState.h"

using namespace AiRinwell;

CRinwellDownState::CRinwellDownState(CAiRinwell * pRinwell)
{
	m_pOwner = pRinwell;
}

CRinwellState * CRinwellDownState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, true, "TransN");

	return nullptr;
}

CRinwellState * CRinwellDownState::LateTick(_float fTimeDelta)
{

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
}
