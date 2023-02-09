#include "stdafx.h"

#include "AstralDoubt_IdleState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"

using namespace Astral_Doubt;

CIdleState::CIdleState(CAstralDoubt* pAstralDoubt, FIELD_STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		else
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	else
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}

	return nullptr;
}

CAstralDoubt_State * CIdleState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	if (m_pCurTarget)
	{
		return new CChaseState(m_pOwner);
	}
	
	else
	{
		if (m_fTimeDeltaAcc > m_fIdleTime)
		{
			switch (rand()%2)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_IDLE, false);
			case 1:
				return new CDetectStop_State(m_pOwner);

				break;
			}
		}
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