#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"

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
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	
	m_fTarget_Distance = Find_BattleTarget();

	

	return nullptr;
}

CIceWolfState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	srand((_uint)time(NULL));
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

		default:
			break;
		}


	}
		else
		{
			/*_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

			m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());*/

			m_pOwner->Check_Navigation();
			

			
		}


	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);
}


void CAttackNormalState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	
}


