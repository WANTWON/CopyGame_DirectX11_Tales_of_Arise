#include "stdafx.h"
#include "..\Public\IceWolfHowLingState.h"
#include "IceWolfChaseState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
using namespace IceWolf;

CHowLingState::CHowLingState(CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;

	m_fHowLingTimeAcc = 0.f;
	m_fHowlingTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CHowLingState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CIceWolfState * CHowLingState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CIceWolfState * CHowLingState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fHowLingTimeAcc += fTimeDelta;

	if (m_fHowLingTimeAcc > m_fHowlingTime)
		m_iRand = rand() % 4;


	if (m_pTarget)
		return new CChaseState(m_pOwner);

	else if (m_bIsAnimationFinished)
		switch (m_iRand)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 1:
				return new CIdleState(m_pOwner);
			case 2:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 3:
				return new CIdleState(m_pOwner);
			default:
				break;
			}
	

	return nullptr;
}

void CHowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_HOWLING);
}

void CHowLingState::Exit()
{
}



