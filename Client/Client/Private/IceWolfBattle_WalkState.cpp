#include "stdafx.h"
#include "..\Public\IceWolfBattle_WalkState.h"
#include "GameInstance.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"

using namespace IceWolf;

CBattle_WalkState::CBattle_WalkState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
	
	

}

CHawkState * CBattle_WalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CHawkState * CBattle_WalkState::Tick(_float fTimeDelta)
{
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	m_fTarget_Distance = Find_BattleTarget();

	return nullptr;
}

CHawkState * CBattle_WalkState::LateTick(_float fTimeDelta)
{
	
	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	if (10 < m_fTarget_Distance)
	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);

	//if (m_bBitePossible)
	//{
	//	return new CAttackNormalState(m_pOwner);
	//}


		if (m_fIdleAttackTimer > 3.f && 0 == m_iRand)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);
		else if (m_fIdleAttackTimer > 3.f && 1 == m_iRand)
			return new CAttackNormalState(m_pOwner);

		else m_fIdleAttackTimer += fTimeDelta;


	return nullptr;
}

void CBattle_WalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);

}

void CBattle_WalkState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	m_iRand = false;
}


