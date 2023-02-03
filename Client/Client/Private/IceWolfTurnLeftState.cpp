#include "stdafx.h"
#include "..\Public\IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
#include "IceWolfChaseState.h"

using namespace IceWolf;

CTurnLeftState::CTurnLeftState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;


	m_fTimeDletaAcc = 0;
	m_fTurnLeftTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CTurnLeftState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CTurnLeftState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CIceWolfState * CTurnLeftState::LateTick(_float fTimeDelta)
{


	m_fTimeDletaAcc += fTimeDelta;

	if (m_fTimeDletaAcc > m_fTurnLeftTime)
		m_iRand = rand() % 4;
	
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
		}


	}
		



	return nullptr;
}

void CTurnLeftState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	if(rand()%2 == 0)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_TURN_L);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_TURN_R);


}

void CTurnLeftState::Exit()
{
	//m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
	//m_pOwner->Get_Model()->Reset();
}



