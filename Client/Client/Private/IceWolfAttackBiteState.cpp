#include "stdafx.h"
#include "..\Public\IceWolfAttackBiteState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_HowLingState.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CAttackBiteState::CAttackBiteState(class CIce_Wolf* pIceWolf, CBaseObj* pCurTarget)
{
	m_pOwner = pIceWolf;
	m_pCurTarget = pCurTarget;
}

CIceWolfState * CAttackBiteState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackBiteState::Tick(_float fTimeDelta)
{
	//m_fDegreeToTarget = RadianToTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	}
	else 
		m_pCurTarget = pDamageCauser;

	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.04f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CIceWolfState * CAttackBiteState::LateTick(_float fTimeDelta)
{
	if (m_bTargetSetting = false)
	{
		m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		//m_pOwner->Set_Done_HitAnimState();
		m_pOwner->Set_FinishBite();
		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_BITE);
	}



	else 
		m_pOwner->Set_OnGoingBite();
	return nullptr;
}

void CAttackBiteState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_BITE);

	
}


void CAttackBiteState::Exit()
{
	
}


