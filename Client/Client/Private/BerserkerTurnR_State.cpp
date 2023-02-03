#include "stdafx.h"

#include "BerserkerTurnR_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerHowLing_State.h"
#include "BerserkerWalkState.h"

using namespace Berserker;

CTurnR_State::CTurnR_State(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;

	m_fTimeDletaAcc = 0;
	m_fTurnR_Time = ((rand() % 8000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CTurnR_State::Tick(_float fTimeDelta)
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

CBerserkerState * CTurnR_State::LateTick(_float fTimeDelta)
{


	m_fTimeDletaAcc += fTimeDelta;

	if (m_fTimeDletaAcc > m_fTurnR_Time)
		m_iRand = rand() % 2;
	
	if (m_pTarget)
	{
		return new CChaseState(m_pOwner);
	}

	else if (m_bIsAnimationFinished)
	{
		//나의 트리거 박스랑 충돌안했을떄
		CBaseObj* pTrigger = m_pOwner->Get_Trigger();

		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
			return new CWalkState(m_pOwner, FIELD_STATE_END, true);
		else
		{
			switch (m_iRand)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END, false);
			case 1:
				return new CHowLing_State(m_pOwner);
			default:
				break;
			}
		}

	}
		






	//이전코드 ///

	/*
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 1.f)
		return new CIdleState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_TURN_R);

	else
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}*/
	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);
}

void CTurnR_State::Exit()
{

}