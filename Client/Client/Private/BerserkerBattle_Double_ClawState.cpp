#include "stdafx.h"

#include "BerserkerBattle_Double_ClawState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"

using namespace Berserker;

CBattle_Double_ClawState::CBattle_Double_ClawState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Double_ClawState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	Find_BattleTarget();

	
	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);


	if (m_fIdleAttackTimer > 3.f && true == m_bAnimFinish)
		return new CBattle_IdleState(m_pOwner);

	else
		m_fIdleAttackTimer += fTimeDelta;

	return nullptr;
}

void CBattle_Double_ClawState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_DOUBLE_CLAW);
}

void CBattle_Double_ClawState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}