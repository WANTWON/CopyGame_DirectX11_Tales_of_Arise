#include "stdafx.h"

#include "BerserkerBattle_HowLingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_DashStartState.h"

using namespace Berserker;

CBattle_HowLingState::CBattle_HowLingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_HowLingState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	return nullptr;
}

CBerserkerState * CBattle_HowLingState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	Find_BattleTarget();

	
	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CBerserkerState * CBattle_HowLingState::LateTick(_float fTimeDelta)
{
	

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);


	if (m_fIdleAttackTimer > 3.f && true == m_bAnimFinish)
		return new CBattle_DashStartState(m_pOwner, STATE_ID::STATE_DASH_START);

	else
		m_fIdleAttackTimer += fTimeDelta;

	return nullptr;
}

void CBattle_HowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::HOWING);
}

void CBattle_HowLingState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}