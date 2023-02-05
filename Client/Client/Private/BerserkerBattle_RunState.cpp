#include "stdafx.h"

#include "BerserkerBattle_RunState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_Quadruple_ClawState.h"
#include "BerserkerBattle_FireBallState.h"
#include "BerserkerBattle_PouncingState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_HowLingState.h"

using namespace Berserker;

CBattle_RunState::CBattle_RunState(CBerserker* pBerserker, STATE_ID ePreState)
{
	m_pOwner   = pBerserker;
	m_ePreState_Id = ePreState;
}

CBerserkerState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CBerserkerState * CBattle_RunState::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

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
	}


	if (m_pCurTarget == nullptr)
		return nullptr;


	return nullptr;
	
}

CBerserkerState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;
	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 4;

	if (m_fTarget_Distance > 4.5f)
	{

		m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f);
		//////회전 코드 
		//CTransform* pMonSterTransform = m_pOwner->Get_Transform();

		//_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
		//_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

		//vLook = XMVectorSetY(vLook, 0.f);
		//vTargetDir = XMVectorSetY(vTargetDir, 0.f);

		//_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

		//if (fDot < 0.8f)
		//	pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.07f);

		//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.4f, m_pOwner->Get_Navigation());

		
	}

	else if (m_fTarget_Distance <= 4.5f)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_PouncingState(m_pOwner);
			
		case 1:
			return new CBattle_FireBallState(m_pOwner);
				
		case 2:
			return new CBattle_Multiple_FireState(m_pOwner);
			
		case 3:
			return new CBattle_Shock_WaveState(m_pOwner);
		default:
			break;
		}
	}


	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_RUN);

}

void CBattle_RunState::Exit()
{

}