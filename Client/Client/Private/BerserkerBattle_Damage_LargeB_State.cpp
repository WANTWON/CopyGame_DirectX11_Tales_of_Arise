#include "stdafx.h"

#include "BerserkerBattle_Damage_LargeB_State.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_WalkState.h"
using namespace Berserker;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(CBerserker* pBerserker, _bool bOnAngry)
{
	m_pOwner = pBerserker;
	m_bAngry = bOnAngry;
	if (m_bAngry)
	{
		m_eEmotion = EMO_ANGRY;
	}
}

CBerserkerState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	
	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	


	return nullptr;
}

CBerserkerState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);


	m_iRand = rand() % 3;


	if(m_bIsAnimationFinished)
	{
		switch (m_eEmotion)
		{
		case EMO_CALM:
			return new CBattle_WalkState(m_pOwner);
			break;

		case EMO_ANGRY:
			return new CBattle_BackStepState(m_pOwner);
			break;

		default:
			break;
		}
	}

	else
	{
//		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

//		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	CMonster::STATS pBerserker_Stats;

	memcpy(&pBerserker_Stats, &m_pOwner->Get_Stats(), sizeof(CMonster::STATS));

	if (pBerserker_Stats.m_fCurrentHp <= 3 && false == m_bAngry)
	{
			
			return new CBattle_HowLingState(m_pOwner);
	}
	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::DAMAGE_LARGE_B);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Damage_LargeB_State::Exit()
{
	
}