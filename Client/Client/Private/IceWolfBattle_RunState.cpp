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
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_HowLingState.h"

using namespace IceWolf;

CBattle_RunState::CBattle_RunState(class CIce_Wolf* pIceWolf, STATE_ID ePreState, CBaseObj* pCurTarget)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;
	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 200) *0.001f)*((rand() % 100) * 0.01f);
	m_pCurTarget = pCurTarget;

}

CIceWolfState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj* pOrigin_DamageCause = nullptr;
	pOrigin_DamageCause = m_pOwner->Get_OrginDamageCauser();


	if (m_pCurTarget == nullptr)
	{
		if (pOrigin_DamageCause == nullptr)
		{
			CBaseObj* pDamageCauser = nullptr;
			pDamageCauser = m_pOwner->Get_DamageCauser();

			if (pDamageCauser == nullptr)
			{
				m_pCurTarget = m_pOwner->Find_MinDistance_Target();

				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
			}

			else if (pDamageCauser != nullptr)
			{
				CBaseObj* pDamageCauser = nullptr;
				pDamageCauser = m_pOwner->Get_DamageCauser();
				m_pOwner->Set_OrginDamageCauser(pDamageCauser);

				m_pCurTarget = pDamageCauser;

				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
			}
		}

		else if (pOrigin_DamageCause != nullptr)
		{
			if (pOrigin_DamageCause->Get_Info().fCurrentHp <= 0)
			{
				CBaseObj* pDamageCauser = nullptr;
				pDamageCauser = m_pOwner->Get_DamageCauser();

				if (pDamageCauser == nullptr)
				{
					CBaseObj* pCorpseNearby = nullptr;
					pCorpseNearby = m_pOwner->Find_MinDistance_Target();

					if (pCorpseNearby->Get_Info().fCurrentHp > 0)
					{
						m_pCurTarget = pCorpseNearby;
						m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
					}

				}
				//
				else if (pDamageCauser != nullptr)
				{
					if (pDamageCauser->Get_Info().fCurrentHp > 0)
					{
						pDamageCauser = m_pOwner->Get_DamageCauser();
						m_pOwner->Set_OrginDamageCauser(pDamageCauser);

						m_pCurTarget = pDamageCauser;

						m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
					}

					return new CBattle_HowLingState(m_pOwner);
				}
			}

			else if (pOrigin_DamageCause->Get_Info().fCurrentHp > 0)
			{ 
				m_pCurTarget = pOrigin_DamageCause;

				m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
				m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
			}
		}
	}

	else
	{
		if (m_pCurTarget->Get_Info().fCurrentHp <= 0)
		{
			CBaseObj* pDamageCauser = nullptr;
			pDamageCauser = m_pOwner->Get_DamageCauser();

			if (pDamageCauser == nullptr)
			{
				CBaseObj* pCorpseNearby = nullptr;
				pCorpseNearby = m_pOwner->Find_MinDistance_Target();

				if (pCorpseNearby->Get_Info().fCurrentHp > 0)
				{
					m_pCurTarget = pCorpseNearby;
					m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
				}

				else
					return new CBattle_HowLingState(m_pOwner);
			}

			else if (pDamageCauser != nullptr)
			{
				int a = 0;
				if (pDamageCauser->Get_Info().fCurrentHp > 0)
				{
					pDamageCauser = m_pOwner->Get_DamageCauser();
					m_pOwner->Set_OrginDamageCauser(pDamageCauser);

					m_pCurTarget = pDamageCauser;

					m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
					m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
				}

				else
					return new CBattle_HowLingState(m_pOwner);	
			}
		}

		else if (m_pCurTarget->Get_Info().fCurrentHp > 0)
		{	
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}

	/////////02-22-1523//
	/*CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		else
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	
	else
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}*/



	//if (m_pCurTarget == nullptr)
	//{
	//	m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	//	if (m_pCurTarget == nullptr)
	//		return nullptr;

	//	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//}
	//else
	//{
	//	m_pCurTarget = pDamageCauser;
	//	m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	//}



	/*CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (m_bStartTarget == false)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		m_bStartTarget = true;
	}
	
	
	if (pDamageCauser && m_bTargetSetting == false)
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
		m_bTargetSetting = true;
	}*/

	return nullptr;
}

CIceWolfState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	//if (m_pTarget == nullptr)
	//	return nullptr;
	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}

	m_pOwner->Check_Navigation();
	
	m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 35);

	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_fTimeDeltaAcc >= m_fRandTime)
		m_iRand = rand() % 2;
	


	

	if (m_fTarget_Distance <= 3.5f && m_b_IsTargetInsight == false)
		return new CBattle_SomerSaultState(m_pOwner);
	
	if (m_fTarget_Distance >= 12.5f && m_ePreState_Id != STATE_ID::STATE_CHARGE_START)
		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START, false);

	else
	{		
		if (m_b_IsTargetInsight == true && m_bOnGoing_Rotation == false)
		{
			_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAt(vPosition);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f);
			//m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());

			if (m_fTarget_Distance <= 7.5f)
			{
				if (m_b_IsTargetInsight == true )
				{
					switch (m_iRand)
					{
					case 0:
						return new CAttackNormalState(m_pOwner, m_ePreState_Id);

					case 1:
						return new CAttackBiteState(m_pOwner);

					default:
						break;
					}

				}
				else if (m_b_IsTargetInsight = false)
					return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END, false);
			}
		}
		
		else
		{
			m_bOnGoing_Rotation = true;
			////회전 코드 
			CTransform* pMonSterTransform = m_pOwner->Get_Transform();

			_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
			_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

			vLook = XMVectorSetY(vLook, 0.f);
			vTargetDir = XMVectorSetY(vTargetDir, 0.f);

			_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

			if (fDot < 0.95f)
				pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07f);

			//m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());

			if (m_fTarget_Distance <= 7.5f)
			{
				if (m_b_IsTargetInsight == true)
				{
					switch (m_iRand)
					{
					case 0:
						return new CAttackNormalState(m_pOwner, m_ePreState_Id);

					case 1:
						return new CAttackBiteState(m_pOwner);

					default:
						break;
					}

				}
				else if (m_b_IsTargetInsight = false)
					return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END, false);
			}
		}
	}




	///원본 코드 
	//m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 30);

	//if (m_fTarget_Distance <= 3.5f && m_b_IsTargetInsight)
	//	return new CBattle_SomerSaultState(m_pOwner);

	//if (m_ePreState_Id == STATE_ID::STATE_CHARGE_START)
	//{
	//	if (m_fTarget_Distance <= 5.5f)
	//		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);

	//	else
	//	{

	//		if (m_fTarget_Distance > 5.5f)
	//		{
	//			////회전 코드 
	//			CTransform* pMonSterTransform = m_pOwner->Get_Transform();

	//			_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
	//			_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

	//			vLook = XMVectorSetY(vLook, 0.f);
	//			vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	//			_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	//			if (fDot < 0.75f)
	//				pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

	//			//m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
	//		}

	//		//else
	//		//{
	//		//	return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
	//		//	//////회전 코드 
	//		//	//CTransform* pMonSterTransform = m_pOwner->Get_Transform();

	//		//	//_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
	//		//	//_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

	//		//	//vLook = XMVectorSetY(vLook, 0.f);
	//		//	//vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	//		//	//_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	//		//	//if (fDot < 0.5f)
	//		//	//	pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

	//		//	////m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//		//	//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
	//		//}
	//	}
	//}
	//else if(m_ePreState_Id == STATE_ID::STATE_BE_DAMAGED)
	//	return new CAttackBiteState(m_pOwner);

	//else
	//{
	//	if (m_fTarget_Distance > 10.5f && false == m_bAttacknormal)
	//	{
	//		m_fTimeDeltaAcc += fTimeDelta;
	//		if (m_fTimeDeltaAcc > m_fRandTime)
	//			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);
	//	}

	//	else
	//	{
	//		m_bAttacknormal = true;
	//		////회전 코드 
	//		CTransform* pMonTransform = m_pOwner->Get_Transform();
	//		_vector		vMonPos = pMonTransform->Get_State(CTransform::STATE_TRANSLATION);
	//		_vector		vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//		_vector vTargetDir = XMVector3Normalize(vCurTargetPos - vMonPos);
	//		_vector vMonLook = XMVector3Normalize(pMonTransform->Get_State(CTransform::STATE_LOOK));

	//		vMonLook = XMVectorSetY(vMonLook, 0.f);
	//		vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	//		_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vMonLook));

	//		if (fDot < 0.95f)
	//			pMonTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

	//		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
	//		
	//		_bool bIsTargetInsight = false;
	//		bIsTargetInsight = Is_TargetInSight(vCurTargetPos, 30.f);

	//		if (m_fTarget_Distance <= 7.f && bIsTargetInsight == true)
	//			return new CAttackNormalState(m_pOwner, m_ePreState_Id);

	//	}
	//}



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


