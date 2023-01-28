#include "stdafx.h"

#include "BerserkerBattle_Shock_WaveState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"

using namespace Berserker;

CBattle_Shock_WaveState::CBattle_Shock_WaveState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Shock_WaveState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();

	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	if (false == m_bIsAnimationFinished)
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::LateTick(_float fTimeDelta)
{
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	

	if (m_bIsAnimationFinished)
		return new CBattle_IdleState(m_pOwner);

	else
	{
		/*_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());*/

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

void CBattle_Shock_WaveState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_SHOCK_WAVE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Shock_WaveState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}