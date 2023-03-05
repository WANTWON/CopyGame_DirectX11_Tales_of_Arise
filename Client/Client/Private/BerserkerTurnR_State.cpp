#include "stdafx.h"

#include "BerserkerTurnR_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerHowLing_State.h"
#include "BerserkerWalkState.h"

using namespace Berserker;

CTurnR_State::CTurnR_State(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;

	m_fTimeDeltaAcc = 0;
	m_fTurnR_Time = ((rand() % 3000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CTurnR_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CTurnR_State::Tick(_float fTimeDelta)
{
	Find_Target_InField();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.3f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	/*if (m_bIsAnimationFinished)
		return new CWalkState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_TURN_R);*/

	return nullptr;
}

CBerserkerState * CTurnR_State::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	//나의 트리거 박스랑 충돌안했을떄
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);

	if (m_bIsAnimationFinished)
	{
		//나의 트리거 박스랑 충돌해 있을때(트리거 박스 안에 있을 때)
		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
		{
			if (m_pTarget)
			{
				return new CChaseState(m_pOwner);
			}

			else
			{
				if (m_fTimeDeltaAcc > m_fRandTime)
				{
					switch (rand() % 2)
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

		}
		else
		{
//			if (bIs_TargetInFront)
				return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_TURN_R, true);

			//else
			//	return new CTurnR_State(m_pOwner);
		}
	}

	//else if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}


	return nullptr;
}

void CTurnR_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::SYMBOL_TURN_LEFT);
}

void CTurnR_State::Exit()
{
	//m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
}