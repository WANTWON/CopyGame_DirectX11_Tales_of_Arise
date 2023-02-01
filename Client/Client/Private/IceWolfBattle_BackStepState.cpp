#include "stdafx.h"
#include "..\Public\IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttack_Elemental_Charge.h"
using namespace IceWolf;

CBattle_BackStepState::CBattle_BackStepState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_BackStepState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_BackStepState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	//m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_BackStepState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_BACKSTEP);

	return nullptr;
}

void CBattle_BackStepState::Enter()
{
	//m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_STEP_BACK);
}

void CBattle_BackStepState::Exit()
{

	
}



