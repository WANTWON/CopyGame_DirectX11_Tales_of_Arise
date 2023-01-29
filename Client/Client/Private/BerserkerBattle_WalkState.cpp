#include "stdafx.h"

#include "BerserkerBattle_WalkState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_HowLingState.h"

using namespace Berserker;

CBattle_WalkState::CBattle_WalkState(CBerserker* pBerserker)
{
	m_pOwner   = pBerserker;
}

CBerserkerState * CBattle_WalkState::AI_Behaviour(_float fTimeDelta)
{



	return nullptr;
}

CBerserkerState * CBattle_WalkState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);
	m_fTarget_Distance = Find_BattleTarget();

	m_pOwner->Check_Navigation();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	m_fRedayAngryTimer += fTimeDelta;

	/*if (m_fRedayAngryTimer >= 5.5f)
	{
		return new CBattle_HowLingState(m_pOwner);

	}*/

	return nullptr;
}

CBerserkerState * CBattle_WalkState::LateTick(_float fTimeDelta)
{

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	if (m_pTarget == nullptr)
		return nullptr;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	if (3.5f < m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *0.85);


		
		m_pOwner->Check_Navigation();
	}

	else
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_Double_ClawState(m_pOwner);
			break;
		case 1:
			return new CBattle_Double_CrowState(m_pOwner);
			break;


		default:
			break;
		}
		
	}

	

	return nullptr;
}

void CBattle_WalkState::Enter()
{

	m_eStateId = STATE_ID::STATE_CALM;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_WALK_F);

	////


}

void CBattle_WalkState::Exit()
{

}