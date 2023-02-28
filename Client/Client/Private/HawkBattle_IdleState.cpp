#include "stdafx.h"
#include "HawkBattle_IdleState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_PeckState.h"
#include "HawkBattle_DashState.h"

using namespace Hawk;

CBattle_IdleState::CBattle_IdleState(CHawk* pHawk, STATE_ID ePreBattleState, CBaseObj* pCurTarget)
{
	m_pOwner = pHawk;
	m_ePreBattleState = ePreBattleState;
	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_pCurTarget = pCurTarget;
}

CHawkState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CHawkState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

		else if (m_pCurTarget)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}

	else if (pDamageCauser != nullptr)
	{
		m_pCurTarget = pDamageCauser;

		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}

	return nullptr;
}

CHawkState * CBattle_IdleState::LateTick(_float fTimeDelta)
{	
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	if (m_ePreBattleState == STATE_ID::STATE_DOWN)
	{
		if (m_bIsAnimationFinished)
			return new CBattle_DashState(m_pOwner);
		else
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			m_pOwner->Check_Navigation();
		}
	}

	else if (m_ePreBattleState == STATE_ID:: STATE_BRAVE)
	{
		if (m_bIsAnimationFinished)
		{
			CBaseObj* p2thCorpseNearby = nullptr;
			p2thCorpseNearby = m_pOwner->Find_MinDistance_Target();

			if (p2thCorpseNearby->Get_Info().fCurrentHp > 0)
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_END);
			else
				return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
		}
	}

	else
	{
		if (m_fTimeDeltaAcc > m_fRandTime)
			return new CBattle_DashState(m_pOwner);
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	if (m_ePreBattleState == STATE_ID::STATE_DOWN)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ARISE_F);

	else if (m_ePreBattleState == STATE_ID::STATE_BRAVE)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_BRAVE);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::SYMBOL_IDLE);
	
}

void CBattle_IdleState::Exit()
{
	if (m_ePreBattleState == STATE_ID::STATE_DOWN)
	{
		m_pOwner->Set_FinishGoingDown();
		m_pOwner->Set_FinishDownState();
	}

	m_pOwner->Set_IsActionMode(false);
}