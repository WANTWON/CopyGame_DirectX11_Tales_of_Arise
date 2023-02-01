#include "stdafx.h"
#include "..\Public\IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CBattle_IdleState::CBattle_IdleState(class CIce_Wolf* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState = ePreState;
	m_fTimeDletaAcc = 0;
	m_fIdleTime = ((rand() % 6000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	Find_BattleTarget();

	return nullptr;
}

CIceWolfState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (false == m_bTargetSetting)
	//{
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	/*	m_bTargetSetting = true;
	}*/

	m_fTimeDletaAcc += fTimeDelta;


	if (CIceWolfState::STATE_ID::START_BATTLE == m_ePreState)
	{
		if (m_fTimeDletaAcc > m_fIdleTime)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);
	}



	//else if(CIceWolfState::STATE_ID::STATE_BATTLEING == m_ePreState)
	//{
	//	if (m_fRedayAttackTimer >= 0.1f)
	//		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_IDLE);
	//}

		


	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	
	
}



