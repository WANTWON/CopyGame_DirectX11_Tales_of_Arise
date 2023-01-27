#include "stdafx.h"
#include "..\Public\IceWolfBattle_HowLingState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"

using namespace IceWolf;

CBattle_HowLingState::CBattle_HowLingState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_HowLingState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_HowLingState::Tick(_float fTimeDelta)
{
	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));


	Find_BattleTarget();
	m_pOwner->Check_Navigation();

	return nullptr;
}

CIceWolfState * CBattle_HowLingState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	
		if (m_fIdleAttackTimer > 3.f && true == m_bAnimFinish)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

		else
			m_fIdleAttackTimer += fTimeDelta;
	

	return nullptr;
}

void CBattle_HowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_DETECT_IDLE);
}

void CBattle_HowLingState::Exit()
{
	m_bAnimFinish = false;
}


