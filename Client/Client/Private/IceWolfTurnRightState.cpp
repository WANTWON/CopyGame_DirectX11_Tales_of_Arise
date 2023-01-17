#include "stdafx.h"
#include "..\Public\IceWolfTurnRightState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"



using namespace IceWolf;

CTurnRightState::CTurnRightState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CTurnRightState::AI_Behaviour()
{
	Find_Target();
	return nullptr;
}

CIceWolfState * CTurnRightState::Tick(_float fTimeDelta)
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
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fRadian * fTimeDelta);
	}

	
	return nullptr;
}

CIceWolfState * CTurnRightState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CTurnRightState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_RIGHT);
}

void CTurnRightState::Exit()
{
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);
}


