#include "stdafx.h"
#include "..\Public\State.h"

void CState::Enter(void)
{
	m_bIsFly = m_pOwner->Get_IsFly();
}

CState * CState::ChangeState(CState * pCurrentState, CState * pNewState)
{
	if (pCurrentState)
	{
		pCurrentState->Exit();
		Safe_Delete(pCurrentState);
	}

	pCurrentState = pNewState;
	pCurrentState->Enter();

	return pCurrentState;
}