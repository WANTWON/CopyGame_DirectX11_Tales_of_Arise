#include "stdafx.h"

#include "SlimeMagicEmitState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeChaseState.h"

using namespace Slime;

CMagicEmitState::CMagicEmitState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CMagicEmitState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;
}

CSlimeState * CMagicEmitState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	if (!m_bIsAnimationFinished)
	{
		
		
		m_pOwner->Check_Navigation();
	}
	
	return nullptr;
}

CSlimeState * CMagicEmitState::LateTick(_float fTimeDelta)
{
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CSlimeState::FIELD_STATE_ID::STATE_MAGICEMIT);
	}


	return nullptr;
}

void CMagicEmitState::Enter()
{
	//m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MAGIC_EMIT);


}

void CMagicEmitState::Exit()
{

}