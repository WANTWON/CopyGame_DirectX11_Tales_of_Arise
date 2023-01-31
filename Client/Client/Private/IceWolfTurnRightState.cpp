#include "stdafx.h"
#include "..\Public\IceWolfTurnRightState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CTurnRightState::CTurnRightState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CTurnRightState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CTurnRightState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	
	return nullptr;
}

CIceWolfState * CTurnRightState::LateTick(_float fTimeDelta)
{
	

	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	//if (m_bIsAnimationFinished)
	//{
	//	
	//	return new CIdleState(m_pOwner, STATE_TURN_R);
	//}
	
	

	return nullptr;
}

void CTurnRightState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_RIGHT);
}

void CTurnRightState::Exit()
{
	//m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	//m_pOwner->Get_Model()->Reset();
}


