#include "stdafx.h"
#include "HawkBattle_TornadeState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_DashState.h"

using namespace Hawk;

CBattle_TornadeState::CBattle_TornadeState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	
}

CHawkState * CBattle_TornadeState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;


}

CHawkState * CBattle_TornadeState::Tick(_float fTimeDelta)
{

	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
		m_pOwner->Check_Navigation();
	

	return nullptr;
}

CHawkState * CBattle_TornadeState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		


	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner, CHawkState::STATE_ID::STATE_TORNADE);
		



	return nullptr;
}

void CBattle_TornadeState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_TORNADE);

	
}

void CBattle_TornadeState::Exit()
{
	
}