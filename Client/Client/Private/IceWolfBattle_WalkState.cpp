#include "stdafx.h"
#include "..\Public\IceWolfBattle_WalkState.h"
#include "GameInstance.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfAttackBiteState.h"
using namespace IceWolf;

CBattle_WalkState::CBattle_WalkState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
	
	

}

CIceWolfState * CBattle_WalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_WalkState::Tick(_float fTimeDelta)
{
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();

	return nullptr;
}

CIceWolfState * CBattle_WalkState::LateTick(_float fTimeDelta)
{
	
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (3.0f < m_fTarget_Distance)
	{
		m_pOwner->Set_Speed(5.f);
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	}
	else
	{
		switch (m_iRand)
		{
		case 0:
			return new CAttackNormalState(m_pOwner);
			break;
		case 1:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
			break;
		default:
			break;
		}
	}


	return nullptr;
}

void CBattle_WalkState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);

}

void CBattle_WalkState::Exit()
{
	
	
}


