#include "stdafx.h"
#include "HawkBattle_Damage_LargeB_State.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"

using namespace Hawk;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(CHawk* pHawk)
{
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CHawkState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
	return nullptr;


}

CHawkState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	m_iRand = rand() % 3;

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);



		/*if (m_fIdleAttackTimer > 3.f)
			return new CBattle_Flying_BackState(m_pOwner);

		else m_fIdleAttackTimer += fTimeDelta;*/

		//else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
		//	return new CAttackNormalState(m_pOwner);

		//else if (m_fIdleAttackTimer > 3.f && m_iRand == 3)
		//	return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

			
	

	return nullptr;
}

CHawkState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{

	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::DAMAGE_LARGE_F);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Damage_LargeB_State::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}