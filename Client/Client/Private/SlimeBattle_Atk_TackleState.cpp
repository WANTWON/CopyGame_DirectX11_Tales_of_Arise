#include "stdafx.h"

#include "SlimeBattle_Atk_TackleState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"


using namespace Slime;

CAtk_TackleState::CAtk_TackleState(CSlime* pSlime)
{
	m_pOwner = pSlime;

}

CSlimeState * CAtk_TackleState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	
	

	return nullptr;
}

CSlimeState * CAtk_TackleState::Tick(_float fTimeDelta)
{

	
	m_fTarget_Distance = Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	//if (m_bIsAnimationFinished)
	//{
	//	if (m_fTarget_Distance < 13.f)
	//		return new CBattle_Poision_Rain(m_pOwner);

	//	else
	//		return new CBattle_RunState(m_pOwner, STATE_TACKLE_DOWN);
	//}
	//	
	//	(m_fTarget_Distance < 13.f)
	//	

	//else
		
	
	return nullptr;
}

CSlimeState * CAtk_TackleState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{

		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_TACKLE);
	}


	else
	{
//		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

void CAtk_TackleState::Enter()
{

	//switch (m_eStateId)
	//{
	//case STATE_TACKLE_START:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ATTACK_TACKLE);
	//	break;

	//case STATE_TACKLE_DOWN:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::DOWN_F);
	//	break;

	//case STATE_TACKLE_ARISE:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ARISE_F);
	//	break;


	//}
	
	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ATTACK_TACKLE);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAtk_TackleState::Exit()
{

}