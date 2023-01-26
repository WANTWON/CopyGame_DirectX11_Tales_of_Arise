#include "stdafx.h"
#include "HawkBattle_DashState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_RunState.h"

using namespace Hawk;

CBattle_DashState::CBattle_DashState(CHawk* pHawk)
{
	m_pOwner = pHawk;
}

CHawkState * CBattle_DashState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CBattle_DashState::Tick(_float fTimeDelta)
{

	
	Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);


	return nullptr;
}

CHawkState * CBattle_DashState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	m_iRand = rand() % 1;


	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			m_pOwner->Get_Transform()->LookAt(vTargetPosition);
			m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
			return new CBattle_RunState(m_pOwner);
			break;

		default:
			break;
		}
	}

	else
	{
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
		



	return nullptr;
}

void CBattle_DashState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CHawk::ANIM::ATTACK_DASH);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_DashState::Exit()
{
	
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
}