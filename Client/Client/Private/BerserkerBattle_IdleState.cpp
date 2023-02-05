#include "stdafx.h"

#include "BerserkerBattle_IdleState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_WalkState.h"

using namespace Berserker;

CBattle_IdleState::CBattle_IdleState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 5000 + 2000) *0.001f)*((rand() % 100) * 0.01f);
	m_fCosignTimeAcc = 0.f;
}

CBerserkerState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
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
	
	
	m_fTarget_Cosign = Find_ToTargetCosign(m_vCurTargetPos);
	m_bTarget_isRight = Find_ToTargetRightSide(m_vCurTargetPos);

	////ÄÚ½Î
	//
	//
	//m_fCosignTimeAcc += fTimeDelta;

	//m_fCosign = m_fCosignTimeAcc * XMConvertToRadians(90.0f);

	//
	//if (m_fTarget_Cosign >= m_fCosign)
	//{
	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	//	if(m_bTarget_isRight == true)
	//		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	//	
	//	else
	//		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
	//	
	//}

	//else
	//	m_fCosignTimeAcc = 0.f;


	//if (m_fTarget_Radian > m_fRadianAcc )
	//{
	//	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	//	
	//	if (m_fRadianAcc == m_fTarget_Radian)
	//		m_fRadianAcc = 0.f;
	//}
	
	
	return nullptr;
}

CBerserkerState * CBattle_IdleState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	if (m_pCurTarget == nullptr)
		return nullptr;

	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
		return new CBattle_WalkState(m_pOwner);



	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{

}