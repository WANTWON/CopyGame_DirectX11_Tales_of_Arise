#include "stdafx.h"

#include "Ice_WolfIdleState.h"
#include "GameInstance.h"

using namespace Ice_Wolf;

CIdleState::CIdleState(CIce_Wolf* pPlayer)
{
	m_pOwner = pPlayer;
}


CIce_WolfState * CIdleState::AI_Behavior()
{
	Find_Target();

	return nullptr;
}

CIce_WolfState * CIdleState::Tick(_float fTimeDelta)
{
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta/*, m_bIsAnimationFinished, pPlayer->Is_AnimationLoop(pPlayer->Get_Model()->Get_CurrentAnimIndex())*/);
	m_pOwner->Check_Navigation();

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
		/*if (m_fIdleMoveTimer > 3.f)
			return new CMoveState();
		else
			m_fIdleMoveTimer += fTimeDelta;*/
	}

	return nullptr;
}

CIce_WolfState * CIdleState::LateTick(_float fTimeDelta)
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