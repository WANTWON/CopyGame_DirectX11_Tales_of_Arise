#include "stdafx.h"

#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerBattle_HowLingState.h"

using namespace Berserker;

CIdleState::CIdleState(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CBerserkerState * CIdleState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CBerserkerState * CIdleState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation(); // ÀÚÀ¯
	Find_Target();


	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	


	return nullptr;
}

CBerserkerState * CIdleState::LateTick(_float fTimeDelta)
{
	

	/*if (m_pTarget)
	{
		_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
		return new CBattle_HowLingState(m_pOwner);
	}

	else
	{
		m_iRand = rand() % 3;
		if (m_fIdleMoveTimer > 3.f && m_iRand == 0)
			return new CTurnLeftState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 1)
			return new CTurnRightState(m_pOwner);

		else if (m_fIdleMoveTimer > 3.f && m_iRand == 2)
			return new CWalkFrontState(m_pOwner);

		else
			m_fIdleMoveTimer += fTimeDelta;

	}*/

	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CBerserker::ANIM::SYMBOL_IDLE);
}

void CIdleState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}