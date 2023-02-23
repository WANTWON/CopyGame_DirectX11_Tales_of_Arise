#include "stdafx.h"
#include "..\Public\IceWolfBattle_HowLingState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttackBiteState.h"

using namespace IceWolf;

CBattle_HowLingState::CBattle_HowLingState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_HowLingState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_HowLingState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	//Find_BattleTarget();
	m_pOwner->Check_Navigation();




	return nullptr;
}

CIceWolfState * CBattle_HowLingState::LateTick(_float fTimeDelta)
{

	if (m_bIsAnimationFinished)
	{
		CBaseObj* p2thCorpseNearby = nullptr;
			p2thCorpseNearby = m_pOwner->Find_MinDistance_Target();

			if (p2thCorpseNearby->Get_Info().fCurrentHp > 0)
			{
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_END);
			}

			else
				return new CBattle_HowLingState(m_pOwner);
	}

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;

	/*_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	if (false == m_bTargetSetting)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		
		
		m_bTargetSetting = true;
	}*/

	//
	//if (m_bIsAnimationFinished)
	//{
	//	switch (m_iRand)
	//	{
	//	case 0:
	//		return new CBattle_RunState(m_pOwner);
	//		break;

	//	case 1:
	//		return new CAttackBiteState(m_pOwner);
	//		break;

	//	default:
	//		break;
	//	}

	//}
	

	//else
	//{
	////	_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

	////	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}


	return nullptr;
}

void CBattle_HowLingState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_HOWLING);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_HowLingState::Exit()
{
	m_bAnimFinish = false;
}


