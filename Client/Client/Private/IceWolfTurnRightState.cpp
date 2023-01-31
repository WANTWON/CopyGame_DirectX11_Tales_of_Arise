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

	m_fTimeDletaAcc = 0.f;
	m_fTurnRightTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
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

		//현재 이 코드쓰면 터져서 잠시 주석 검
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	
	return nullptr;
}

CIceWolfState * CTurnRightState::LateTick(_float fTimeDelta)
{
	

	/*if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}*/


	m_fTimeDletaAcc += fTimeDelta;

	if (m_fTimeDletaAcc > m_fTurnRightTime)
		m_iRand = rand() % 4;

	if (m_bIsAnimationFinished)
		switch (m_iRand)
		{
		case 0:
			return new CWalkState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
			break;
		case 1:
			return new CIdleState(m_pOwner, FIELD_STATE_ID::STATE_TURN);
			break;
		case 2:
			return new CWalkState(m_pOwner, STATE_TURN);
			break;
		case 3:
			return new CIdleState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE);
			break;
		default:
			break;
		}
	
	

	return nullptr;
}

void CTurnRightState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_RIGHT);
}

void CTurnRightState::Exit()
{
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	//m_pOwner->Get_Model()->Reset();
}


