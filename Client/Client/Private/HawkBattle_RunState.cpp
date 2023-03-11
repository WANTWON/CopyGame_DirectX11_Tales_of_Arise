#include "stdafx.h"
#include "HawkBattle_RunState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_PeckState.h"
#include "HawkBattle_TornadeState.h"
#include "HawkBattle_DashState.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_RunState::CBattle_RunState(CHawk* pHawk, STATE_ID ePreBattleState)
{
	m_pOwner = pHawk;
	m_ePreBattleState = ePreBattleState;

	m_fRandTime = ((rand() % 100) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CBattle_RunState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CHawkState * CBattle_RunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	if (m_pCurTarget == nullptr)
	{
		int iRand = rand() % 2;
		if (iRand == 0)
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		else
			m_pCurTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}
	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);

	m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 35);
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	if (m_ePreBattleState != STATE_ID::STATE_BATTLE)
	{
		if (m_fTarget_Distance > 8.5f)
		{
			if (m_b_IsTargetInsight == true)
			{
				m_pOwner->Get_Transform()->LookAtExceptY(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.3f, m_pOwner->Get_Navigation());
			}
			else
			{
				CTransform* pMonSterTransform = m_pOwner->Get_Transform();
				_vector vTargetDir = XMVector3Normalize(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
				_vector vLook = XMVector3Normalize(pMonSterTransform->Get_State(CTransform::STATE_LOOK));

				vLook = XMVectorSetY(vLook, 0.f);
				vTargetDir = XMVectorSetY(vTargetDir, 0.f);
				if (XMVectorGetX(XMVectorEqual(vTargetDir, XMVectorSet(0.f, 0.f, 0.f, 0.f))) != 0)
					return nullptr;
				_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));
				if (fDot < 0.8f)
					pMonSterTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
				m_pOwner->Get_Transform()->Go_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
			}
		}
		else
		{
			m_pOwner->Get_Transform()->LookAtExceptY(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			
		}
	}


	m_pOwner->Check_Navigation();
	return nullptr;
}

CHawkState * CBattle_RunState::LateTick(_float fTimeDelta)
{
	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}

	if (m_pCurTarget->Get_Info().fCurrentHp <= 0)
		return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);

	m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 35);

	if (m_ePreBattleState != STATE_ID::STATE_BATTLE)
	{
		if (m_fTarget_Distance > 4.5f)
		{
			if (m_b_IsTargetInsight == true)
			{
				switch (m_iRand)
				{
				case 0:
					return new CBattle_PeckState(m_pOwner);
				case 1:
					return new CBattle_TornadeState(m_pOwner);
				case 2:
					return new CBattle_DashState(m_pOwner);
				}
			}
		}
		else
		{
			if (m_b_IsTargetInsight == true)
			{
				switch (m_iRand)
				{
				case 0:
					return new CBattle_PeckState(m_pOwner);
				case 1:
					return new CBattle_TornadeState(m_pOwner);
				case 2:
					return new CBattle_DashState(m_pOwner);
				}
			}
			else
			{
				if (m_bIsAnimationFinished == true)
				{
					switch (m_iRand)
					{
					case 0:
						return new CBattle_PeckState(m_pOwner);
					case 1:
						return new CBattle_TornadeState(m_pOwner);
					case 2:
						return new CBattle_DashState(m_pOwner);
					}
				}
			}
		}
	}
	else if (m_ePreBattleState == STATE_ID::STATE_BATTLE)
		return new CBattle_DashState(m_pOwner);


	return nullptr;
}

void CBattle_RunState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_RUN);

	CBaseObj* m_pCurTarget = nullptr;
	m_pCurTarget = m_pOwner->Get_DamageCauser();
	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}
}

void CBattle_RunState::Exit()
{


}