#include "stdafx.h"

#include "BerserkerHowLing_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerWalkState.h"
using namespace Berserker;

CHowLing_State::CHowLing_State(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;

	m_fTimeDeltaAcc = 0.f;
	m_fHowlingTime = ((rand() % 4000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CHowLing_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CHowLing_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	return nullptr;
}

CBerserkerState * CHowLing_State::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;


	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);


	_bool bIs_TargetInFront = false;

	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);


	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
		{
			if (m_fTimeDeltaAcc > m_fRandTime)
			{
				switch (rand() % 2)
				{
				case 0:
					return new CWalkState(m_pOwner, FIELD_STATE_END);
				case 1:
					return new CTurnR_State(m_pOwner);


				default:
					break;
				}
			}
		}

		else
		{
			if (bIs_TargetInFront)
				return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_HOWLING, true);

			else
				return new CTurnR_State(m_pOwner);

		}
	}





	////이전 코드 
	/*if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_HOWLING);
*/
	return nullptr;
}

void CHowLing_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_HOWLING);
}

void CHowLing_State::Exit()
{
	
}