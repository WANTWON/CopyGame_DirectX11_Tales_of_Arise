#include "stdafx.h"
#include "..\Public\IceWolfBattle_DeadState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"

using namespace IceWolf;

CBattle_DeadState::CBattle_DeadState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	m_pOwner->Check_Navigation();
	return nullptr;
}

CIceWolfState * CBattle_DeadState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	return nullptr;
}

CIceWolfState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 4;

	/*if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
		return new CBattle_WalkState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
		return new CBattle_DeadState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
		return new CAttackNormalState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 3)
		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

	else m_fIdleAttackTimer += fTimeDelta;*/

	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_DEAD);
}

void CBattle_DeadState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	
}



