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

CBattle_RunState::CBattle_RunState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
	
}

CIceWolfState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();

	//m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	srand((_uint)time(NULL));
	m_iRand = rand() % 2;
	
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);


	//if (false == m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	m_bTargetSetting = true;
	//}

	
	if (2.5f < m_fTarget_Distance)
	{
		
		m_pOwner->Set_Speed(5.f);
		
		//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.1f);
		
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

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);

}

void CBattle_RunState::Exit()
{



}


