#include "stdafx.h"

#include "BerserkerBattle_BackStepState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_BackStepState::CBattle_BackStepState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_BackStepState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_BackStepState::Tick(_float fTimeDelta)
{
	Find_BattleTarget();

	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);

		m_bTargetSetting = true;
	}

	srand((_uint)time(NULL));
	m_iRand = rand() % 1;
	

	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner, STATE_QUADRUPLE);
			break;
		
		default:
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

CBerserkerState * CBattle_BackStepState::LateTick(_float fTimeDelta)
{
	
	
	
	
	

	

	return nullptr;
}

void CBattle_BackStepState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::ATTACK_STEP_BACK);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_BackStepState::Exit()
{

}