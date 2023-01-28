#include "stdafx.h"

#include "SlimeBattle_Poision_RainState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_RunState.h"
#include "SlimeBattle_Poision_ShotState.h"


using namespace Slime;

CBattle_Poision_RainState::CBattle_Poision_RainState(CSlime* pSlime)
{
	m_pOwner = pSlime;
	
	
	
}

CSlimeState * CBattle_Poision_RainState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CSlimeState * CBattle_Poision_RainState::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();


	if (m_pTarget == nullptr)
		return nullptr;

	return nullptr;
}

CSlimeState * CBattle_Poision_RainState::LateTick(_float fTimeDelta)
{
	
	if (m_bIsAnimationFinished)
	{
		if (m_fTarget_Distance > 6.f)
		{
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_TACKLE);
		}
		
		else
		{
			return new CBattle_Poision_ShotState(m_pOwner, STATE_ID::STATE_POSION_RAIN);
		}
	}
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		

	//else
	//{
	//	_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");
	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());
	//	m_pOwner->Check_Navigation();
	//}

	return nullptr;
}

void CBattle_Poision_RainState::Enter()
{

	m_pOwner->Get_Model()->Set_NextAnimIndex(CSlime::ANIM::ATTACK_POISON_RAIN);
	
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_Poision_RainState::Exit()
{

}