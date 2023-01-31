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

CAttackBiteState::CAttackBiteState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CAttackBiteState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackBiteState::Tick(_float fTimeDelta)
{

	

	m_fDegreeToTarget = RadianToTarget();
	

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CIceWolfState * CAttackBiteState::LateTick(_float fTimeDelta)
{
	

	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_BITE);
		

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
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


