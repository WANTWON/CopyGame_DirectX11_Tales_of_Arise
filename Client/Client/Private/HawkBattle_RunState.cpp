#include "stdafx.h"
#include "HawkBattle_RunState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_PeckState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_TornadeState.h"
#include "HawkBattle_DashState.h"

using namespace Hawk;

CBattle_RunState::CBattle_RunState(CHawk* pHawk, STATE_ID ePreBattleState)
{
	m_pOwner          = pHawk;
	m_ePreBattleState = ePreBattleState;

	m_fRandTime = ((rand() % 4000) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CHawkState * CBattle_RunState::Tick(_float fTimeDelta)
{
	//m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


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

	return nullptr;
}

CHawkState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (m_fTarget_Distance > 4.5f)
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

		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());

		m_fTimeDletaAcc += fTimeDelta;
		if (m_fTimeDletaAcc > m_fRandTime)
			m_iRand = rand() % 3;

	}
	else if (m_fTarget_Distance <= 4.5f)
	{
		switch (m_iRand)
		{
		case /*CHawkState::STATE_ID::STATE_DASH*/0:
			return new CBattle_PeckState(m_pOwner);

		case /*CHawkState::STATE_ID::STATE_PECK*/1:
			return new CBattle_TornadeState(m_pOwner);
					
		case 2:
			return new CBattle_DashState(m_pOwner);

				//case CHawkState::STATE_ID::STATE_TORNADE:
				//	return new CBattle_DashState(m_pOwner);

		default:
			break;
		}
	}
	
	


	return nullptr;




	///////////0202 14:29 원본코드
	//if (m_fTarget_Distance > 10.5f)
	//{

	//	////회전 코드 
	//	CTransform* pMonSterTransform = m_pOwner->Get_Transform();

	//	_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
	//	_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

	//	vLook = XMVectorSetY(vLook, 0.f);
	//	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	//	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	//	if (fDot < 0.8f)
	//		pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

	//	//m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.1f, m_pOwner->Get_Navigation());



	//	/*m_pOwner->Get_Transform()->LookAt(vTargetPosition);

	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);*/
	//}
	//else
	//{
	//	if (m_fTimeDletaAcc > m_fRandTime)
	//	{
	//		switch (m_ePreBattleState)
	//		{

	//		case CHawkState::STATE_ID::STATE_DASH:
	//			return new CBattle_PeckState(m_pOwner);

	//		case CHawkState::STATE_ID::STATE_PECK:
	//			return new CBattle_TornadeState(m_pOwner);
	//			break;

	//			//case CHawkState::STATE_ID::STATE_TORNADE:
	//			//	return new CBattle_DashState(m_pOwner);

	//		default:
	//			break;
	//		}
	//	}
	//}

	//return nullptr;


	/*m_iRand = rand() % 1;

	CCollider* pCollider = m_pOwner->Get_Collider();
	pCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());
	

		if (10.f <= m_fIdleAttackTimer)
		{
			return new CBattle_ChargeState(m_pOwner);
		}

		else
		{
			if (6.f > m_fTarget_Distance)
				return new CBattle_GrabStartState(m_pOwner);
			

			m_fIdleAttackTimer += fTimeDelta;
		}
		*/



	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_RUN);

	
}

void CBattle_RunState::Exit()
{

	
}