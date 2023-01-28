#include "stdafx.h"

#include "BerserkerBattle_RunState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_Quadruple_ClawState.h"
#include "BerserkerBattle_DashStartState.h"

using namespace Berserker;

CBattle_RunState::CBattle_RunState(CBerserker* pBerserker, STATE_ID ePreState)
{
	m_pOwner   = pBerserker;
	m_ePreState_Id = ePreState;
}

CBerserkerState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	
	m_iRand = rand() % 2;

	
		if (5.5f < m_fTarget_Distance)
		{
			m_pOwner->Get_Transform()->LookAt(vTargetPosition);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 2.f);
		}
			
		else
		{
			switch (m_ePreState_Id)
			{
			case STATE_DASH_SCRATCHES:
				return new CBattle_Quadruple_ClawState(m_pOwner);
				break;

			case STATE_QUADRUPLE:
				return new CBattle_DashStartState(m_pOwner);
				break;

			default:
				break;
			}
		}
	return nullptr;
}

CBerserkerState * CBattle_RunState::Tick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;
	


	AI_Behaviour(fTimeDelta);

	
	return nullptr;
}

CBerserkerState * CBattle_RunState::LateTick(_float fTimeDelta)
{

	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_RUN);


	////
	
}

void CBattle_RunState::Exit()
{

}