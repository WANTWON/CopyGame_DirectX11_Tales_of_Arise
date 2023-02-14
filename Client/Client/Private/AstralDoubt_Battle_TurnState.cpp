#include "stdafx.h"

#include "AstralDoubt_Battle_TurnState.h"
#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CBattle_TurnState::CBattle_TurnState(CAstralDoubt* pAstralDoubt, _bool bIsToTarget_Right, _float bGapDot)
{
	m_pOwner = pAstralDoubt;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 8000) *0.001f)*((rand() % 100) * 0.01f);
	m_bIsToTarget_Right = bIsToTarget_Right;
	m_fGapDot = bGapDot;
}

CAstralDoubt_State * CBattle_TurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CAstralDoubt_State * CBattle_TurnState::Tick(_float fTimeDelta)
{
	
	//m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	//if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	//m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}
	//m_pOwner->Check_Navigation();

	//CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	//if (pDamageCauser == nullptr)
	//{
	//	if (m_pCurTarget == nullptr)
	//	{
	//		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	//		if (m_pCurTarget == nullptr)
	//			return nullptr;

	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}
	//	else
	//	{
	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}

	//}
	//else
	//{
	//	m_pCurTarget = pDamageCauser;
	//	m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	//}


	if (m_pCurTarget == nullptr)
		return nullptr;

	return nullptr;
}

CAstralDoubt_State * CBattle_TurnState::LateTick(_float fTimeDelta)
{


	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fTurnR_Time)
		m_iRand = rand() % 2;
	
	_vector		vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_float fToTargetRadian = Find_ToTargetDot(vCurTargetPos);
	_bool  Is_ToTargetRightSide = Is_TargetRightSide(vCurTargetPos);


	//if (m_bIsAnimationFinished)
	//{
	//	return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::T);
	//}

	return nullptr;
}

void CBattle_TurnState::Enter()
{
	//m_eStateId = STATE_ID::STATE_IDLE;

	if(m_bIsToTarget_Right)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::TURN_R);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::TURN_L);

}

void CBattle_TurnState::Exit()
{
	if(m_bIsToTarget_Right)
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fGapDot);
	/*else
		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fGapDot);*/
}