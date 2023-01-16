#include "stdafx.h"

#include "IceWolfIdleState.h"
#include "GameInstance.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"

using namespace IceWolf;

CIdleState::CIdleState(CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CIdleState::AI_Behaviour()
{
	Find_Target();
	return nullptr;
}

CIceWolfState * CIdleState::Tick(_float fTimeDelta)
{
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation(); // ÀÚÀ¯

	if (m_pTarget)
	{
		_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);

		if (m_fIdleAttackTimer > 1.5f)
		{
			/*if (!m_bHasSpottedTarget)
			return new CAggroState();
			else
			return new CAttackState();*/
		}
		else
			m_fIdleAttackTimer += fTimeDelta;
	}
	else
	{
		
		m_iRand = rand()%3;
		if (m_fIdleMoveTimer > 3.f && m_iRand == 0)
			return new CWalkFrontState(m_pOwner);
		
		else if (m_fIdleMoveTimer > 3.f && m_iRand == 1)
			return new CTurnLeftState(m_pOwner);
		
		else if (m_fIdleMoveTimer > 3.f && m_iRand == 2)
			return new CTurnRightState(m_pOwner);

		else
			m_fIdleMoveTimer += fTimeDelta;
	}

	return nullptr;
}

CIceWolfState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CIdleState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
}