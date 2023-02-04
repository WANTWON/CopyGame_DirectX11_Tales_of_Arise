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
	
	return nullptr;
}

CBerserkerState * CBattle_DeadState::Tick(_float fTimeDelta)
{


	if(false == m_bDeadAnimFinish)
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta*2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	


	return nullptr;
}

CBerserkerState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	
	m_pOwner->Check_Navigation();

	if (m_bIsAnimationFinished && false == m_bDeadAnimFinish)
	{


		
		m_pOwner->Get_Model()->Play_Animation(4.5f, false);
		
		//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DEAD);
		
		m_StartPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);

		m_bDeadAnimFinish = true;


		//m_pOwner->Get_Model()->Play_Animation(2.5f, false);
	}

	else
	{
		/*_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());*/
	}

	if (m_bDeadAnimFinish)
	{
		m_pOwner->Set_State(CTransform::STATE_TRANSLATION, m_StartPos);
		m_pOwner->Set_GlowUp();
	}
	return nullptr;

}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DEAD);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	
}

void CBattle_DeadState::Exit()
{

}