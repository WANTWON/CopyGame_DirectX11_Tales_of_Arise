#include "stdafx.h"
#include "HawkBattle_IdleState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_PeckState.h"
#include "HawkBattle_ChargeState.h"
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
	//Find_BattleTarget();

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
	m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);


	/*if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_bTargetSetting = true;
	}*/

	
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
	{
		switch (m_ePreBattleState)
		{
		case CHawkState::STATE_ID::START_BATTLEMODE:
			return new CBattle_DashState(m_pOwner);
			
		default:
			break;
		}
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::SYMBOL_IDLE);

	
}

void CBattle_IdleState::Exit()
{

	
}