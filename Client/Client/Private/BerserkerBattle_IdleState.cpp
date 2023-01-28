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
	AI_Behaviour(fTimeDelta);
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	srand((_uint)time(NULL));
	m_iRand = rand() % 1;

	m_fRedayAttackTimer += fTimeDelta;

	if (m_fRedayAttackTimer >= 2.5f)
	{
		return new CBattle_WalkState(m_pOwner);
			
	}

	

	return nullptr;
}

CBerserkerState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (false == m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	//	m_bTargetSetting = true;
	//}
	


	/*m_iRand = rand() % 3;


	if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
		return new CBattle_Double_CrowState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
		return new CBattle_Double_ClawState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
		return new CBattle_Shock_WaveState(m_pOwner);

	else m_fIdleAttackTimer += fTimeDelta;*/

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::SYMBOL_IDLE);
}

void CBattle_IdleState::Exit()
{

}