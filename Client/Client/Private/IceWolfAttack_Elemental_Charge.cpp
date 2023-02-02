#include "stdafx.h"
#include "..\Public\IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_RunState.h"
using namespace IceWolf;

CAttack_Elemental_Charge::CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType, CBaseObj* pCurTarget)
{
	m_eStateId_Charge = eStateType;
	m_pOwner = pIceWolf;
	m_pCurTarget = pCurTarget;
	m_fTimeDletaAcc = 0.f;
	m_fRandTime = ((rand() % 3000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
	
}

CIceWolfState * CAttack_Elemental_Charge::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	//m_fTarget_Distance = Find_BattleTarget();

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

		else if (m_pCurTarget)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
	}

	else if (pDamageCauser != nullptr)
	{
		m_pCurTarget = pDamageCauser;

		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

		////회전 코드 
		//CTransform* pMonSterTransform = m_pOwner->Get_Transform();


	if (m_eStateId_Charge == STATE_ID::STATE_CHARGE_START)
	{
		_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	}

		//_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
		//_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

		//vLook = XMVectorSetY(vLook, 0.f);
		//vTargetDir = XMVectorSetY(vTargetDir, 0.f);

		//_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

		//if (fDot < 0.8f)
		//	pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.2f);


	


	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::LateTick(_float fTimeDelta)
{
	m_fTimeDletaAcc += fTimeDelta;

	if (m_bTargetSetting = false)
	{
		m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		m_bTargetSetting = true;
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId_Charge)
		{
		case STATE_CHARGE_START:
		{	if (m_pCurTarget != nullptr)
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_CHARGE_START, m_pCurTarget);
			else 
				return new CBattle_RunState(m_pOwner, STATE_ID::STATE_CHARGE_START);
		}

		
		m_fTimeDletaAcc += fTimeDelta;
			
			
		case STATE_CHARGE_END:
			if (m_fTimeDletaAcc > m_fRandTime)
			return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_CHARGE_END);

		default:
			break;
		}
	}

		

	return nullptr;
}

void CAttack_Elemental_Charge::Enter()
{
	switch (m_eStateId_Charge)
	{
	case STATE_CHARGE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_START);
		break;

	case STATE_CHARGE_LOOP:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);
		break;

	case STATE_CHARGE_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_END);
		break;
	}

}

void CAttack_Elemental_Charge::Exit()
{
}



