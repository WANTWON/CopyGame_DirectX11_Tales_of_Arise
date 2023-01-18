#include "stdafx.h"
#include "HawkBattle_ChargeState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_ChargeState::CBattle_ChargeState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_ChargeState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CHawkState * CBattle_ChargeState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	m_fTarget_Distance = Find_BattleTarget();


	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CHawkState * CBattle_ChargeState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (7 < m_fTarget_Distance)
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	/*if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
		return new CBattle_IdleState(m_pOwner);

	else
		m_fIdleAttackTimer += fTimeDelta;*/

	/*if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
		return new CBattle_ChargeState(m_pOwner);*/

	return nullptr;
}

void CBattle_ChargeState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_CHARGE);
}

void CBattle_ChargeState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}