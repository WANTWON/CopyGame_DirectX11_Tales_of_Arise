#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfTurnLeftState.h"

using namespace IceWolf;

CAttackNormalState::CAttackNormalState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CAttackNormalState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CAttackNormalState::LateTick(_float fTimeDelta)
{

	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
			switch (m_iRand)
			{
			case 0:
				return new CBattle_SomerSaultState(m_pOwner);
				break;
			case 1:
				return new CBattle_BackStepState(m_pOwner);
				break;
			}

	}

	else
	{
		_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

		//if (m_fIdleAttackTimer > 3.f && m_iRand == 0 && true == m_bIsAnimationFinished)
		//	return new CAttackNormalState(m_pOwner);

		//else if (m_fIdleAttackTimer > 3.f && m_iRand == 1 && true == m_bIsAnimationFinished)
		//	return new CBattle_IdleState(m_pOwner);

		//else m_fIdleAttackTimer += fTimeDelta;

		/*if (true == m_bIsAnimationFinished)
			return new CBattle_IdleState(m_pOwner);

		else
		{
			_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

			m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

			m_pOwner->Check_Navigation();
		}*/

	
	if (m_fDegreeToTarget >= 30)
	{
		return new CTurnLeftState(m_pOwner);
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 0.5f);
	}

	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}


void CAttackNormalState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	
}


