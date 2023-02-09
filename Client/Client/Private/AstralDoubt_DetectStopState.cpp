#include "stdafx.h"

#include "AstralDoubt_DetectStopState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_IdleState.h"

using namespace Astral_Doubt;

CDetectStop_State::CDetectStop_State(CAstralDoubt* pAstralDoubt)
{
	m_pOwner = pAstralDoubt;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 8000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CDetectStop_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CDetectStop_State::Tick(_float fTimeDelta)
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


	if (m_pCurTarget == nullptr)
		return nullptr;

	return nullptr;
}

CAstralDoubt_State * CDetectStop_State::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		if (m_bIsAnimationFinished)
			return new CWalkState(m_pOwner, FIELD_STATE_END, false);
	}

	//수정 코드 
	else
	{
		m_fTimeDeltaAcc += fTimeDelta;

		if (m_pCurTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDeltaAcc > m_fRandTime)
		{
			switch (rand() % 3)
			{
			case 0:
				return new CWalkState(m_pOwner, FIELD_STATE_END);
			case 1:
				return new CDetectStop_State(m_pOwner);
			case 2:
				return new CIdleState(m_pOwner);


			default:
				break;
			}
		}
	}
		
	return nullptr;
}

void CDetectStop_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_DETECT_STOP);
}

void CDetectStop_State::Exit()
{

}