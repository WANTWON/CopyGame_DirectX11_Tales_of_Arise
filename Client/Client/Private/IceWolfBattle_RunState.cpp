#include "stdafx.h"
#include "..\Public\IceWolfBattle_RunState.h"
#include "GameInstance.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfBattle_IdleState.h"

using namespace IceWolf;

CBattle_RunState::CBattle_RunState(class CIce_Wolf* pIceWolf, STATE_ID ePreState, CBaseObj* pCurTarget)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_fTimeDeltaAcc = 0;
	m_fRunTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_pCurTarget = pCurTarget;
}

CIceWolfState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		
		else if(m_pCurTarget)
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


	

	//m_fDegreeToTarget = RadianToTarget();
	return nullptr;
}

CIceWolfState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	//if (m_pTarget == nullptr)
	//	return nullptr;
	m_pOwner->Check_Navigation();
	

	if (m_ePreState_Id == STATE_ID::STATE_CHARGE_START)
	{
		if (m_fTarget_Distance <= 2.5f)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);

		else
		{

			if (m_fTarget_Distance > 8.5f)
			{
				////회전 코드 
				CTransform* pMonSterTransform = m_pOwner->Get_Transform();

				_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
				_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

				vLook = XMVectorSetY(vLook, 0.f);
				vTargetDir = XMVectorSetY(vTargetDir, 0.f);

				_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

				if (fDot < 0.8f)
					pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

				//m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
				m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
			}

			else
			{
				////회전 코드 
				CTransform* pMonSterTransform = m_pOwner->Get_Transform();

				_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
				_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

				vLook = XMVectorSetY(vLook, 0.f);
				vTargetDir = XMVectorSetY(vTargetDir, 0.f);

				_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

				if (fDot < 0.8f)
					pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

				//m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
				m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
			}
		}
	}
	else if(m_ePreState_Id == STATE_ID::STATE_BE_DAMAGED)
		return new CAttackBiteState(m_pOwner);

	else
	{
		if (m_fTarget_Distance > 10.5f && false == m_bAttacknormal)
		{
			m_fTimeDeltaAcc += fTimeDelta;
			if (m_fTimeDeltaAcc > m_fRandTime)
				return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);
		}

		else
		{
			m_bAttacknormal = true;
			////회전 코드 
			CTransform* pMonSterTransform = m_pOwner->Get_Transform();

			_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
			_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

			vLook = XMVectorSetY(vLook, 0.f);
			vTargetDir = XMVectorSetY(vTargetDir, 0.f);

			_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

			if (fDot < 0.8f)
				pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());

			if (m_fTarget_Distance <= 3.f)
				return new CAttackNormalState(m_pOwner);

		}
	}

	return nullptr;
}

void CBattle_RunState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_RUN);

}

void CBattle_RunState::Exit()
{
	
}


