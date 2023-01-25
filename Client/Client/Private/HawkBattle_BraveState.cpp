#include "stdafx.h"
#include "HawkBattle_BraveState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"

using namespace Hawk;

CBattle_BraveState::CBattle_BraveState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_BraveState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CHawkState * CBattle_BraveState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	


	return nullptr;
}

CHawkState * CBattle_BraveState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);



	if (true == m_bIsAnimationFinished)
		return new CBattle_BombingState(m_pOwner);

	else
	{
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
		



	return nullptr;
}

void CBattle_BraveState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_BRAVE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_BraveState::Exit()
{
	
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}