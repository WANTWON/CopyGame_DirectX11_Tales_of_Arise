#include "stdafx.h"

#include "BerserkerBattle_WalkState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_HowLingState.h"

using namespace Berserker;

CBattle_WalkState::CBattle_WalkState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_WalkState::AI_Behaviour(_float fTimeDelta)
{



	return nullptr;
}

CBerserkerState * CBattle_WalkState::Tick(_float fTimeDelta)
{
	
	//m_fTarget_Distance = Find_BattleTarget();

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

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

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

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

CBerserkerState * CBattle_WalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}


	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;
	//if (m_pTarget == nullptr)
	//	return nullptr;

	/*if (m_pTarget == nullptr)
		return new CBattle_WalkState(m_pOwner);*/

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	if (6.5f < m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.2f, m_pOwner->Get_Navigation());
	}

	else
		return new CBattle_Double_ClawState(m_pOwner);
	

	



	return nullptr;
}

void CBattle_WalkState::Enter()
{

	m_eStateId = STATE_ID::STATE_CALM;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_WALK_F);

	////


}

void CBattle_WalkState::Exit()
{

}