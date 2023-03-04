#include "stdafx.h"

#include "BerserkerBattle_IdleState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_WalkState.h"

using namespace Berserker;

CBattle_IdleState::CBattle_IdleState(CBerserker* pBerserker, STATE_ID eStateId)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 5000 + 2000) *0.001f)*((rand() % 100) * 0.01f);
	m_fCosignTimeAcc = 0.f;
	m_eStateId = eStateId;
}

CBerserkerState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	
	if (m_eStateId == STATE_ID::START_BATTLE)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta*2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	CBaseObj*	m_pCurTarget = m_pOwner->Get_DamageCauser();

	if (m_pCurTarget == nullptr)
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	
	m_fTarget_Cosign = Find_ToTargetCosign(m_vCurTargetPos);
	m_bTarget_isRight = Find_ToTargetRightSide(m_vCurTargetPos);
	
	if (m_eStateId == STATE_ID::START_BATTLE)
	{
		m_pOwner->Get_Transform()->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
	}
	
	if (m_eStateId == STATE_ID::STATE_ARISE && !m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CBerserkerState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_eStateId == STATE_ID::STATE_ARISE || m_eStateId == STATE_ID::START_BATTLE)
	{
		if (m_bIsAnimationFinished)
			return new CBattle_WalkState(m_pOwner);
	}
	else
	{
		if (m_fTimeDeltaAcc > m_fRandTime)
			return new CBattle_WalkState(m_pOwner);
	}


	m_pOwner->Check_Navigation();
	return nullptr;
}

void CBattle_IdleState::Enter()
{
	if(m_eStateId == STATE_ID::STATE_ARISE)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ARISE_F);
	else if(m_eStateId == STATE_ID::START_BATTLE)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ADVENT);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	if (m_eStateId == STATE_ID::STATE_ARISE)
	{
		m_pOwner->Set_FinishGoingDown();
		m_pOwner->Set_FinishDownState();
	}

	m_pOwner->Set_IsActionMode(false);
}