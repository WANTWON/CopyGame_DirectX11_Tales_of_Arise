#include "stdafx.h"

#include "SlimeBattle_RunState.h"
#include "GameInstance.h"
#include "SlimeBattle_Atk_TackleState.h"
#include "SlimeIdleState.h"
#include "SlimeBattle_Poision_RainState.h"
#include "SlimeBattle_Poision_ShotState.h"

using namespace Slime;

CBattle_RunState::CBattle_RunState(CSlime* pSlime, STATE_ID ePreState)
{
	m_pOwner = pSlime;
	m_ePreState_Id = ePreState;
}

CSlimeState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CSlimeState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();


	if (m_pTarget == nullptr)
		return nullptr;


	AI_Behaviour(fTimeDelta);
	return nullptr;
}

CSlimeState * CBattle_RunState::LateTick(_float fTimeDelta)
{


	if (m_pTarget == nullptr)
		return nullptr;

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (m_fTarget_Distance > 2.f)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.89f);


	}

	else
	{
		if (STATE_TACKLE == m_ePreState_Id)
			return new CBattle_Poision_ShotState(m_pOwner, STATE_ID::STATE_MOVE);

		else if (STATE_POSION_SHOT == m_ePreState_Id)
			return new CAtk_TackleState(m_pOwner);

		else
			return new CBattle_Poision_RainState(m_pOwner);

	}
	
	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::MOVE_RUN);

	
}

void CBattle_RunState::Exit()
{

}