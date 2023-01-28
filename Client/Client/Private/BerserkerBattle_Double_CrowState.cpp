#include "stdafx.h"

#include "BerserkerBattle_Double_CrowState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_WalkState.h"
#include "Monster.h"
using namespace Berserker;

CBattle_Double_CrowState::CBattle_Double_CrowState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_Double_CrowState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	

	return nullptr;
}

CBerserkerState * CBattle_Double_CrowState::Tick(_float fTimeDelta)
{
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	/*if (m_pTarget == nullptr)
		return nullptr;*/
	
	return nullptr;
}

CBerserkerState * CBattle_Double_CrowState::LateTick(_float fTimeDelta)
{
	
	
	if (m_bIsAnimationFinished)
	{
		
		return new CBattle_WalkState(m_pOwner);
	}

	else
	{
		/*_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());*/

		m_pOwner->Check_Navigation();
		
	}

	return nullptr;
}

void CBattle_Double_CrowState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_DOUBLE_CROW);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();


}

void CBattle_Double_CrowState::Exit()
{

}