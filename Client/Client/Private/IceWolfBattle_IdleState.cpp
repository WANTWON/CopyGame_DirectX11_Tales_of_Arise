#include "stdafx.h"
#include "..\Public\IceWolfBattle_IdleState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CBattle_IdleState::CBattle_IdleState(class CIce_Wolf* pIceWolf, STATE_ID ePreState, CBaseObj* pCurTarget)
{
	m_pOwner = pIceWolf;
	m_ePreState = ePreState;
	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_pCurTarget = pCurTarget;

}

CIceWolfState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	CBaseObj*	m_pCurTarget = m_pOwner->Get_DamageCauser();

	if (m_pCurTarget == nullptr)
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);

	if (m_ePreState == STATE_ID::START_BATTLE)
	{
		m_pOwner->Get_Transform()->LookDir(XMVectorSet(0.f,0.f,-1.f,0.f));
	}

	return nullptr;
}

CIceWolfState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	
	m_pOwner->Check_Navigation();
	
	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_ePreState == STATE_ID::STATE_ARISE)
	{
		if (m_bIsAnimationFinished)
		{
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START, true, m_pCurTarget);
		}
		else
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			m_pOwner->Check_Navigation();
		}
	}
	else if (m_ePreState == STATE_ID::START_BATTLE)
	{
		if (m_fTimeDeltaAcc > m_fIdleTime && m_bIsAnimationFinished)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START, false, m_pCurTarget);
	}
	else
	{
		if (m_fTimeDeltaAcc > m_fIdleTime)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START, false, m_pCurTarget);
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	if(m_ePreState == STATE_ID::STATE_ARISE)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ARISE_F);
	if(m_ePreState == STATE_ID::START_BATTLE)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ADVENT);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	m_pOwner->Set_IsActionMode(false);
}



