#include "stdafx.h"
#include "..\Public\IceWolfBattle_BackStepState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttack_Elemental_Charge.h"


using namespace IceWolf;

CBattle_BackStepState::CBattle_BackStepState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
	m_fTimeDeltaAcc = 0;
	m_iRandTime = ((rand() % 1500) *0.001f)*((rand() % 100) * 0.01f);
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

	return nullptr;
}

CIceWolfState * CBattle_BackStepState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_fTimeDeltaAcc >= m_iRandTime)
		m_iRand = rand() % 2;

	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CAttack_Elemental_Charge(m_pOwner, CIceWolfState::STATE_ID::STATE_CHARGE_START);

		case 1:
			return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_BACKSTEP);
			
		default:
			break;
		}
		
	}
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



