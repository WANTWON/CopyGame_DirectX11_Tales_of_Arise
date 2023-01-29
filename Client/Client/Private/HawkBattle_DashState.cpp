#include "stdafx.h"
#include "HawkBattle_DashState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_TornadeState.h"

using namespace Hawk;

CBattle_DashState::CBattle_DashState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_DashState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CBattle_DashState::Tick(_float fTimeDelta)
{

	
	Find_BattleTarget();


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

CHawkState * CBattle_DashState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;


	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		/*case 0:
			return new CBattle_BombingState(m_pOwner);
			break;*/

		case 0:
			return new CBattle_RunState(m_pOwner, CHawkState::STATE_ID::STATE_DASH);
			break;

		case 1:
			return new CBattle_RunState(m_pOwner, CHawkState::STATE_ID::STATE_DASH);
			break;

		default:
			break;
		}
	}




	return nullptr;
}

void CBattle_DashState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_DASH);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_DashState::Exit()
{
	

}