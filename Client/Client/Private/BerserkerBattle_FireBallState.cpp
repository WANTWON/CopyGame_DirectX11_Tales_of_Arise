#include "stdafx.h"

#include "BerserkerBattle_FireBallState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_FireBallState::CBattle_FireBallState(CBerserker* pBerserker)
{
	
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_FireBallState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_FireBallState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);

	srand((_uint)time(NULL));
	m_iRand = rand() % 1;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_bTargetSetting = true;
	}



	if (m_bIsAnimationFinished)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);

		if (m_fTarget_Distance > 7)
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_DASH_SCRATCHES);

		else
			return new CBattle_Shock_WaveState(m_pOwner);

	}
	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CBerserkerState * CBattle_FireBallState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CBattle_FireBallState::Enter()
{

	m_eStateId = STATE_ID::STATE_ANGRY;

		m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_FIRE_BALL);
		
	
	////
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	////
}

void CBattle_FireBallState::Exit()
{

}