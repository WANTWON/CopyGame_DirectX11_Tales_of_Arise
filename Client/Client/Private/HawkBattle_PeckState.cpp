#include "stdafx.h"
#include "HawkBattle_PeckState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_PeckState::CBattle_PeckState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	
}

CHawkState * CBattle_PeckState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;


}

CHawkState * CBattle_PeckState::Tick(_float fTimeDelta)
{


	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		
		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CHawkState * CBattle_PeckState::LateTick(_float fTimeDelta)
{
	srand((_uint)time(NULL));
	m_iRand = rand() % 1;




	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner, CHawkState::STATE_ID::STATE_PECK);
			break;
	

		default:
			break;
		}


	}


	return nullptr;
}

void CBattle_PeckState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_PECK);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_PeckState::Exit()
{

	
}