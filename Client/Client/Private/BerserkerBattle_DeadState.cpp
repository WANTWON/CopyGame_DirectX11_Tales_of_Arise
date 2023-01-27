#include "stdafx.h"

#include "BerserkerBattle_DeadState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"

using namespace Berserker;

CBattle_DeadState::CBattle_DeadState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	return nullptr;
}

CBerserkerState * CBattle_DeadState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	Find_BattleTarget();

	
	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CBerserkerState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	

	/*_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);*/


	m_iRand = rand() % 3;


	//if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
	//	return new CBattle_Double_CrowState(m_pOwner);

	//else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
	//	return new CBattle_Double_ClawState(m_pOwner);

	//else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
	//	return new CBattle_Shock_WaveState(m_pOwner);

	//else m_fIdleAttackTimer += fTimeDelta;

	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::SLOW_DEAD);
}

void CBattle_DeadState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}