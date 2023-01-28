#include "stdafx.h"

#include "SlimeBattle_TakeDamage_State.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeBattle_Poision_RainState.h"


using namespace Slime;

CBattle_TakeDamage_State::CBattle_TakeDamage_State(CSlime* pSlime)
{
	m_pOwner = pSlime;

}

CSlimeState * CBattle_TakeDamage_State::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CSlimeState * CBattle_TakeDamage_State::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CSlimeState * CBattle_TakeDamage_State::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;


	if (m_bIsAnimationFinished)
	{
		return new CBattle_Poision_RainState(m_pOwner);
	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

void CBattle_TakeDamage_State::Enter()
{

	m_pOwner->Get_Model()->Set_NextAnimIndex(CSlime::ANIM::DAMAGE_LARGE_B);
	
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

}

void CBattle_TakeDamage_State::Exit()
{

}