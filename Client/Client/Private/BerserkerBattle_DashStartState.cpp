#include "stdafx.h"

#include "BerserkerBattle_DashStartState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"

using namespace Berserker;

CBattle_DashStartState::CBattle_DashStartState(CBerserker* pBerserker, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_DashStartState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CBerserkerState * CBattle_DashStartState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	return nullptr;
}

CBerserkerState * CBattle_DashStartState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (11 < m_fTarget_Distance)
	{
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	}

	///
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	///

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CBerserkerState::STATE_DASH_START:
			return new CBattle_DashStartState(m_pOwner, STATE_ID::STATE_DASH_LOOP);
		case Client::CBerserkerState::STATE_DASH_LOOP:
			break;
		case Client::CBerserkerState::STATE_DASH_SCRATCHES:
			return new CBattle_IdleState(m_pOwner);
		case Client::CBerserkerState::STATE_END:
			break;
		}
	}
	else
	{
		if (STATE_DASH_LOOP == m_eStateId)
		{
			if (15 < m_fTarget_Distance)
				m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

			if (15 >= m_fTarget_Distance)
			{
				return new CBattle_DashStartState(m_pOwner, STATE_ID::STATE_DASH_SCRATCHES);
				m_iCount = 0;
			}
		}


		//////
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
		/////
	}
	

	return nullptr;
}

void CBattle_DashStartState::Enter()
{

	switch (m_eStateId)
	{
	case STATE_DASH_START:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::DASH_START);
		break;
	case STATE_DASH_LOOP:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::DASH_LOOP);
		break;
	case STATE_DASH_SCRATCHES:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::FLYING_ATTACK2);
		break;
	}
	////
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	////
}

void CBattle_DashStartState::Exit()
{
	//m_fIdleMoveTimer = 0.f;
	//m_fIdleAttackTimer = 0.f;
}