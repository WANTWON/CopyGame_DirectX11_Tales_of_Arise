#include "stdafx.h"

#include "BerserkerBattle_TurnState.h"
#include "GameInstance.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_TurnState::CBattle_TurnState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_TurnState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_TurnState::Tick(_float fTimeDelta)
{




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

	//ÄÚ½Î


	m_fCosignTimeAcc += fTimeDelta;

	m_fCosign = m_fCosignTimeAcc * XMConvertToRadians(90.0f);



	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();


		if (m_fTarget_Cosign >= m_fCosign)
		{

			if (m_bTarget_isRight == true)
			{
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);
				m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
			}
			else
			{
				//m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_L);
				m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
			}
		}

		else
			m_fCosignTimeAcc = 0.f;
	}
	
	//if (m_bTarget_isRight == true && m_bSetAnim == false)
	//{
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);
	//	m_bSetAnim = true;
	//}
	//else
	//{
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_L);
	//	m_bSetAnim = true;
	//}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CBerserkerState * CBattle_TurnState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_MOVE);
	}


	
	
	return nullptr;
}

void CBattle_TurnState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::TURN_R);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_TurnState::Exit()
{

}