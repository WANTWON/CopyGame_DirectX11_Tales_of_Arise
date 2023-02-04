#include "stdafx.h"
#include "..\Public\IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_RunState.h"
using namespace IceWolf;

CAttack_Elemental_Charge::CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType, CBaseObj* pCurTarget)
{
	m_eStateId_Charge = eStateType;
	m_pOwner = pIceWolf;
	m_pCurTarget = pCurTarget;
	m_fTimeDeltaAcc = 0.f;
	m_fRandTime = ((rand() % 3000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
	
}

CIceWolfState * CAttack_Elemental_Charge::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	//m_fTarget_Distance = Find_BattleTarget();

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

		else if (m_pCurTarget)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
	}

	else if (pDamageCauser != nullptr)
	{
		m_pCurTarget = pDamageCauser;

		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}



	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::LateTick(_float fTimeDelta)
{
	/*if (m_pTarget == nullptr)
		return nullptr;*/
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_bTargetSetting = false)
	{
		m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId_Charge)
		{
		case STATE_CHARGE_START:
		{	if (m_pCurTarget != nullptr)
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_CHARGE_START, m_pCurTarget);
			else 
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_CHARGE_START);
		}

		
		m_fTimeDeltaAcc += fTimeDelta;
			
			
		case STATE_CHARGE_END:
			if (m_fTimeDeltaAcc > m_fRandTime)
			return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_CHARGE_END);

		default:
			break;
		}
	}

		

	return nullptr;
}

void CAttack_Elemental_Charge::Enter()
{
	switch (m_eStateId_Charge)
	{
	case STATE_CHARGE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_START);
		break;

	case STATE_CHARGE_LOOP:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);
		break;

	case STATE_CHARGE_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_END);
		break;
	}

}

void CAttack_Elemental_Charge::Exit()
{
}



