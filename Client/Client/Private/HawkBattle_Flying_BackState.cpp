#include "stdafx.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_IdleState.h"
#include "HawkBattle_RunState.h"

using namespace Hawk;

CBattle_Flying_BackState::CBattle_Flying_BackState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

CHawkState * CBattle_Flying_BackState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	return nullptr;


}

CHawkState * CBattle_Flying_BackState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_iRand = rand() % 2;

	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
			break;
			
		case 1:
			return new CBattle_ChargeState(m_pOwner);
			break;
		}
		
	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CHawkState * CBattle_Flying_BackState::LateTick(_float fTimeDelta)
{

	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	
	return nullptr;
}

void CBattle_Flying_BackState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::FLYING_BACK);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Flying_BackState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}