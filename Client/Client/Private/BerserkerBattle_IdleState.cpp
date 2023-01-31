#include "stdafx.h"

#include "BerserkerBattle_IdleState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_WalkState.h"

using namespace Berserker;

CBattle_IdleState::CBattle_IdleState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{

	
	return nullptr;
}

CBerserkerState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	srand((_uint)time(NULL));
	m_iRand = rand() % 1;

	m_fRedayAttackTimer += fTimeDelta;

	if (m_fRedayAttackTimer >= 3.f)
		return new CBattle_WalkState(m_pOwner);
			

	return nullptr;
}

CBerserkerState * CBattle_IdleState::LateTick(_float fTimeDelta)
{

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{

}