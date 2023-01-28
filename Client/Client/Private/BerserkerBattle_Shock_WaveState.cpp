#include "stdafx.h"

#include "BerserkerBattle_Shock_WaveState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
using namespace Berserker;

CBattle_Shock_WaveState::CBattle_Shock_WaveState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Shock_WaveState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);



	m_iRand = rand() % 1;

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_DASH_SCRATCHES);
	}
	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CBerserkerState * CBattle_Shock_WaveState::LateTick(_float fTimeDelta)
{
	
	

	return nullptr;
}

void CBattle_Shock_WaveState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_SHOCK_WAVE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Shock_WaveState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}