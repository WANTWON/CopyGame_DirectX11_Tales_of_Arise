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
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	Find_BattleTarget();


	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CHawkState * CBattle_BraveState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);



	if (m_fIdleAttackTimer > 3.f && true == m_bAnimFinish)
		return new CBattle_BombingState(m_pOwner);

	else
		m_fIdleAttackTimer += fTimeDelta;



	return nullptr;
}

void CBattle_BraveState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_BRAVE);
}

void CBattle_BraveState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}