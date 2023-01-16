#include "stdafx.h"
#include "..\Public\IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnRightState.h"

using namespace IceWolf;

CTurnLeftState::CTurnLeftState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CTurnLeftState::AI_Behaviour()
{
	Find_Target();
	return nullptr;
}

CIceWolfState * CTurnLeftState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_iRadian = rand() % 10;
	m_fRadian = 0.05 * m_iRadian;

	if (m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())))
	{
		m_iRand = rand() % 2;
		if (m_iRand == 0)
		{
			return new CWalkFrontState(m_pOwner);
		}

		else if (m_iRand == 1)
		{
			return new CIdleState(m_pOwner);
		}
	}

	else
	{
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fRadian *-1.f);
	}


	return nullptr;
}

CIceWolfState * CTurnLeftState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CTurnLeftState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_LEFT);
}

void CTurnLeftState::Exit()
{
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);
}



