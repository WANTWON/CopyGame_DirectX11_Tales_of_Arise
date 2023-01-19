#include "stdafx.h"
#include "HawkBattle_IdleState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"

using namespace Hawk;

CBattle_IdleState::CBattle_IdleState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	//Find_Target();
	return nullptr;
}

CHawkState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	m_fTarget_Distance = Find_BattleTarget();


	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	return nullptr;
}

CHawkState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 1;
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	
		//m_pOwner->Get_Transform()->Go_Up(fTimeDelta * 10.f);
		//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);


	if (m_fIdleAttackTimer > 5.f && m_iRand == 0)
		return new CBattle_ChargeState(m_pOwner);
	
	else
		m_fIdleAttackTimer += fTimeDelta;

	//else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
	//	return new CBattle_IdleState(m_pOwner);






	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::SYMBOL_DETECT_STOP);
}

void CBattle_IdleState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}