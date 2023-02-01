#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CAttackNormalState::CAttackNormalState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CAttackNormalState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackNormalState::Tick(_float fTimeDelta)
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

CIceWolfState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;

	if (m_bIsAnimationFinished)
	{
		if (m_iRand = 0)
			return new CBattle_BackStepState(m_pOwner);
		else
			return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_NORMAL_ATK);
	}
	
	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);
}


void CAttackNormalState::Exit()
{
	
	
}


