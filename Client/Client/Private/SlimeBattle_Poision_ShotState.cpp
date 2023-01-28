#include "stdafx.h"

#include "SlimeBattle_Poision_ShotState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeBattle_Poision_RainState.h"

using namespace Slime;

CBattle_Poision_ShotState::CBattle_Poision_ShotState(CSlime* pSlime, STATE_ID ePreState)
{
	m_pOwner = pSlime;
	
	m_ePreState_Id = ePreState;
	
}

CSlimeState * CBattle_Poision_ShotState::AI_Behaviour(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	

	return nullptr;
}

CSlimeState * CBattle_Poision_ShotState::Tick(_float fTimeDelta)
{


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();


	if (m_pTarget == nullptr)
		return nullptr;

	return nullptr;
}

CSlimeState * CBattle_Poision_ShotState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if(STATE_ID::STATE_POSION_RAIN == m_ePreState_Id)
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_POSION_SHOT);

		else
			return new CBattle_Poision_RainState(m_pOwner);
	}


	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	return nullptr;
}

void CBattle_Poision_ShotState::Enter()
{

	m_pOwner->Get_Model()->Set_NextAnimIndex(CSlime::ANIM::ATTACK_POISON_SHOT);
	
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Poision_ShotState::Exit()
{

}