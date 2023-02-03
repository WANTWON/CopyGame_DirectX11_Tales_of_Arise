#include "stdafx.h"
#include "..\Public\IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfTurnLeftState.h"

using namespace IceWolf;

CBattle_SomerSaultState::CBattle_SomerSaultState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_SomerSaultState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_SomerSaultState::Tick(_float fTimeDelta)
{
	

	m_pOwner->Check_Navigation();


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

CIceWolfState * CBattle_SomerSaultState::LateTick(_float fTimeDelta)
{
	
	if (m_bIsAnimationFinished)
	{	
		m_pOwner->Set_Done_HitAnimState();
		return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_SOMESAULT);
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

void CBattle_SomerSaultState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_SOMERSAULT_END);


}

void CBattle_SomerSaultState::Exit()
{

	
}



