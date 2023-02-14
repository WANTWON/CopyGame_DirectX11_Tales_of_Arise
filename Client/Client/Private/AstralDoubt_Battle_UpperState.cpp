#include "stdafx.h"

#include "AstralDoubt_Battle_UpperState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"

using namespace Astral_Doubt;

CBattle_UpperState::CBattle_UpperState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_UpperState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_UpperState::Tick(_float fTimeDelta)
{
	Find_Target();
	m_fTarget_Distance = m_fOutPutTarget_Distance;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	/*m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

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
	}*/

	return nullptr;
}

CAstralDoubt_State * CBattle_UpperState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_bIsAnimationFinished)
	{
		if (m_fTarget_Distance <= 5.5f)
		{
			return new CBattle_720Spin_FirstState(m_pOwner);
		}

		else
		return new CBattleWalkState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_UPPER);
	}

	return nullptr;
}

void CBattle_UpperState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_IN_OUT_UPPER);
}

void CBattle_UpperState::Exit()
{

}