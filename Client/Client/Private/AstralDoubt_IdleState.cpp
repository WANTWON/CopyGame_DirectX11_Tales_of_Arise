#include "stdafx.h"

#include "AstralDoubt_IdleState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_TurnState.h"

using namespace Astral_Doubt;

CIdleState::CIdleState(CAstralDoubt* pAstralDoubt, FIELD_STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 8000 + 4000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CIdleState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	return nullptr;
}

CAstralDoubt_State * CIdleState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
	{
		if (m_pTarget)
		{
			return new CChaseState(m_pOwner);
		}

		else
		{
			if (m_fTimeDeltaAcc > m_fIdleTime)
			{
				switch (rand() % 2)
				{
				case 0:
					return new CWalkState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE, false);
				case 1:
					return new CDetectStop_State(m_pOwner);

					break;
				}
			}
		}
	}

	else
	{
		return new CTurnState(m_pOwner);
	}


	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_IDLE);
}

void CIdleState::Exit()
{

}