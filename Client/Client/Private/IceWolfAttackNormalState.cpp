#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"

using namespace IceWolf;

CAttackNormalState::CAttackNormalState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CAttackNormalState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CHawkState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bAnimFinish = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();
	m_fTarget_Distance = Find_BattleTarget();

	

	return nullptr;
}

CHawkState * CAttackNormalState::LateTick(_float fTimeDelta)
{

	m_iRand = rand() % 2;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	
	if(10 < m_fTarget_Distance)
	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);



		if (m_fIdleAttackTimer > 3.f && m_iRand == 0 && true == m_bAnimFinish)
			return new CAttackNormalState(m_pOwner);

		else if (m_fIdleAttackTimer > 3.f && m_iRand == 1 && true == m_bAnimFinish)
			return new CBattle_IdleState(m_pOwner);

		else m_fIdleAttackTimer += fTimeDelta;




	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);
}


void CAttackNormalState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	m_bAnimFinish = false;
}


