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
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation(); // ÀÚÀ¯

	m_iRand = rand() % 1;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	m_fRedayAttackTimer += fTimeDelta;

	if (21.5f <= m_fRedayAttackTimer)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
			break;
		/*case 1:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_LOOP);
			break;*/
			/*case 2:
			return new CBattle_HowLingState(m_pOwner);
			break;*/
		default:
			break;
		}
	}


	return nullptr;


	//m_iRand = rand() % 3;

	////_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	////m_pOwner->Get_Transform()->LookAt(vTargetPosition);



	//	if (m_fRedayAttackTimer > 3.f)
	//		return new CBattle_Flying_BackState(m_pOwner);

	//	else m_fIdleAttackTimer += fTimeDelta;

	//	//else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
	//	//	return new CAttackNormalState(m_pOwner);

	//	//else if (m_fIdleAttackTimer > 3.f && m_iRand == 3)
	//	//	return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

			
	

	return nullptr;
}

CHawkState * CBattle_IdleState::LateTick(_float fTimeDelta)
{

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::SYMBOL_IDLE);

	
}

void CBattle_IdleState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fRedayAttackTimer = 0.f;
	
}