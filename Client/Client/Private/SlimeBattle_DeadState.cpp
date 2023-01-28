#include "stdafx.h"

#include "SlimeBattle_DeadState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeBattle_Poision_RainState.h"

using namespace Slime;

CBattle_DeadState::CBattle_DeadState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CSlimeState * CBattle_DeadState::Tick(_float fTimeDelta)
{

	if (false == m_bDeadAnimFinish)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	}
	return nullptr;


}

CSlimeState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished && false == m_bDeadAnimFinish)
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::DEAD);
		m_pOwner->Get_Model()->Play_Animation(6.2f, false);
		m_pOwner->Set_Dissolve();

		m_bDeadAnimFinish = true;

	}
	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CSlime::ANIM::DEAD);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_DeadState::Exit()
{

}