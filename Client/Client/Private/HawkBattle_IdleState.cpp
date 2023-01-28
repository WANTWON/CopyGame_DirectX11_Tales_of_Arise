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

using namespace Hawk;

CBattle_IdleState::CBattle_IdleState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	
}

CHawkState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	
	
	
	return nullptr;
}

CHawkState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	



			
	

	return nullptr;
}

CHawkState * CBattle_IdleState::LateTick(_float fTimeDelta)
{	
	srand((_uint)time(NULL));
	m_iRand = rand() % 2;


	m_fRedayAttackTimer += fTimeDelta;

	if (m_fRedayAttackTimer >= 2.5f)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
			break;
		case 1:
			return new CBattle_ChargeState(m_pOwner);
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

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::SYMBOL_IDLE);

	
}

void CBattle_IdleState::Exit()
{

	
}