#include "stdafx.h"
#include "..\Public\IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"


using namespace IceWolf;

CBattle_IdleState::CBattle_IdleState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bAnimFinish =	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	
	Find_BattleTarget();
	m_pOwner->Check_Navigation();


	return nullptr;
}

CIceWolfState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 4;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	/*if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
		return new CBattle_WalkState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
		return new CBattle_IdleState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
		return new CAttackNormalState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 3)
		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

	else m_fIdleAttackTimer += fTimeDelta;*/

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	m_iRand = false;
}



