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

CBattle_IdleState::CBattle_IdleState(CHawk* pHawk, STATE_ID ePreBattleState)
{
	m_pOwner = pHawk;
	m_ePreBattleState = ePreBattleState;
}

CHawkState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	
	
	
	return nullptr;
}

CHawkState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	


	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CHawkState * CBattle_IdleState::LateTick(_float fTimeDelta)
{	
	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_bTargetSetting = true;
	}

	
	m_fNextMotionTimer += fTimeDelta;

	

	if (m_fNextMotionTimer >= 0.5f)
	{
		switch (m_ePreBattleState)
		{
		case CHawkState::STATE_ID::START_BATTLEMODE:
			return new CBattle_DashState(m_pOwner);
			break;

		case CHawkState::STATE_ID::STATE_PECK:
			return new CBattle_Flying_BackState(m_pOwner);
			break;

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