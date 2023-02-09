#include "stdafx.h"

#include "SlimeWalkState.h"
#include "GameInstance.h"
#include "SlimeChaseState.h"
#include "SlimeTurnR_State.h"
#include "SlimeIdleState.h"
using namespace Slime;

CWalkState::CWalkState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
}

CSlimeState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CSlimeState * CWalkState::Tick(_float fTimeDelta)
{


	Find_Target();
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	return nullptr;
}

CSlimeState * CWalkState::LateTick(_float fTimeDelta)
{


	if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_fWalkMoveTimer > 1.5f)
		return new CTurn_State(m_pOwner);



	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_WALK_F);

	
}

void CWalkState::Exit()
{

}