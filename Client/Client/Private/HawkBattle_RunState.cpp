#include "stdafx.h"
#include "HawkBattle_RunState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"

using namespace Hawk;

CBattle_RunState::CBattle_RunState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
	return nullptr;


}

CHawkState * CBattle_RunState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	m_bIsAnimationFinished =  m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	return nullptr;
}

CHawkState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 1;

	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	

		if (10.f <= m_fIdleAttackTimer)
		{
			return new CBattle_ChargeState(m_pOwner);
		}

		else
		{
			if (6.f > m_fTarget_Distance)
				return new CBattle_GrabStartState(m_pOwner);


			m_fIdleAttackTimer += fTimeDelta;
		}
	
	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::FAST_FLAP_OF_WINGS);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_RunState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}