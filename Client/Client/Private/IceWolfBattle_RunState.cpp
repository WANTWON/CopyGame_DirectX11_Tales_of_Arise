#include "stdafx.h"
#include "..\Public\IceWolfBattle_RunState.h"
#include "GameInstance.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfTurnLeftState.h"

using namespace IceWolf;

CBattle_RunState::CBattle_RunState(class CIce_Wolf* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
}

CIceWolfState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	//m_fTarget_Distance = Find_BattleTarget();

	//m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	//if (m_pTarget == nullptr)
	//	return nullptr;

	m_iRand = rand() % 2;
	
	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (4.5f < m_fTarget_Distance)
	//{
	//	//m_pOwner->Set_Speed(5.f);
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.1f);
	//	
	//}
	//else
	//{
		switch (m_ePreState_Id)
		{
		case CIceWolfState::STATE_ID::START_BATTLE:
			return new CAttackNormalState(m_pOwner);
			break;
		case CIceWolfState::STATE_ID::STATE_BACKSTEP:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
			break;
		case CIceWolfState::STATE_ID::STATE_ELEMENTAL_CHARGE:
			if (m_iRand == 0)
				return new CAttackBiteState(m_pOwner);
			else
				return new CAttackNormalState(m_pOwner);
			break;
		case CIceWolfState::STATE_ID::STATE_BITE:
			return new CAttackNormalState(m_pOwner);
		case CIceWolfState::STATE_ID::STATE_NORMAL_ATK:
			if(m_iRand == 0)
				return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
			else 
				return new CBattle_BackStepState(m_pOwner);
		default:
			break;
		}
	//}
		
	return nullptr;
}

void CBattle_RunState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);

}

void CBattle_RunState::Exit()
{
	
	m_pOwner->Get_Model()->Reset();

}


