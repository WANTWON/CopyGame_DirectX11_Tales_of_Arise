#include "stdafx.h"

#include "BerserkerBattle_WalkState.h"
#include "GameInstance.h"
#include "BerserkerBattle_Double_CrowState.h"
#include "BerserkerBattle_Double_ClawState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_HowLingState.h"
#include "BerserkerBattle_PouncingState.h"
#include "BerserkerBattle_IdleState.h"

using namespace Berserker;

CBattle_WalkState::CBattle_WalkState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 400 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_WalkState::AI_Behaviour(_float fTimeDelta)
{



	return nullptr;
}

CBerserkerState * CBattle_WalkState::Tick(_float fTimeDelta)
{
	
	//m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");



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
					else
						return new CBattle_HowLingState(m_pOwner);//CBattle_IdleState(m_pOwner, STATE_ID::STATE_LOOKOUT);
				}
			}

			else if (pOrigin_DamageCause->Get_Info().fCurrentHp > 0)

			m_pCurTarget = pOrigin_DamageCause;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
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
				//return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_LOOKOUT);
			}
		}

		else if (m_pCurTarget->Get_Info().fCurrentHp > 0)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}

	//02-22 1738
	//CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	//if (pDamageCauser == nullptr)
	//{
	//	if (m_pCurTarget == nullptr)
	//	{
	//		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

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


	//if (m_pCurTarget == nullptr)
	//	return nullptr;

	//m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	//if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}

	
	m_fTimeDeltaAcc += fTimeDelta;

	return nullptr;
}

CBerserkerState * CBattle_WalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}

	m_b_IsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 35);

	

	if (m_fTimeDeltaAcc >= m_fRandTime)
		m_iRand = rand() % 3;
	//if (m_pTarget == nullptr)
	//	return nullptr;

	/*if (m_pTarget == nullptr)
		return new CBattle_WalkState(m_pOwner);*/

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	
	
		if (m_fTarget_Distance <= 11.5f)
		{
			if (m_b_IsTargetInsight == true)
			{

				/*if (m_bAngry == false)
					return new CBattle_Double_ClawState(m_pOwner);*/

				/*else
				{*/
					switch (m_iRand)
					{
					case 0:
						return new CBattle_PouncingState(m_pOwner);

					case 1:
						return new CBattle_Shock_WaveState(m_pOwner);

					case 2:
						return new CBattle_Double_ClawState(m_pOwner);
					default:
						break;
					}
				/*}*/
			}

			else if (m_b_IsTargetInsight == false)
			{
				_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAt(vPosition);

				/*if (m_bAngry == false)
					return new CBattle_Double_ClawState(m_pOwner);*/

			/*	else
				{*/
					switch (m_iRand)
					{
					case 0:
						return new CBattle_PouncingState(m_pOwner);

					case 1:
						return new CBattle_Shock_WaveState(m_pOwner);

					case 2:
						return new CBattle_Double_ClawState(m_pOwner);
					default:
						break;
					}
				/*}*/
			}
		}

		else if (m_fTarget_Distance > 11.5f)
		{
			_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAt(vPosition);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.2f, m_pOwner->Get_Navigation());

			if (m_fTarget_Distance <= 11.5f)
			{
				if (m_b_IsTargetInsight == true)
				{

					if (m_bAngry == false)
						return new CBattle_Double_ClawState(m_pOwner);

					else
					{
						switch (m_iRand)
						{
						case 0:
							return new CBattle_PouncingState(m_pOwner);

						case 1:
							return new CBattle_Shock_WaveState(m_pOwner);

						case 2:
							return new CBattle_Double_ClawState(m_pOwner);
						default:
							break;
						}
					}
				}

				else if (m_b_IsTargetInsight == false)
				{
					_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAt(vPosition);

					/*if (m_bAngry == false)
						return new CBattle_Double_ClawState(m_pOwner);*/

					/*else
					{*/
						switch (m_iRand)
						{
						case 0:
							return new CBattle_PouncingState(m_pOwner);

						case 1:
							return new CBattle_Shock_WaveState(m_pOwner);

						case 2:
							return new CBattle_Double_ClawState(m_pOwner);
						default:
							break;
						}
					/*}*/
				}
			}
		}
	

	
	

		//m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());



	//if (m_fTarget_Distance > 11.5f)
	//{
	//	m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.2f, m_pOwner->Get_Navigation());
	//}

	//else if (m_fTarget_Distance <= 11.5f)
	//{
	//	if(m_bAngry == false)
	//	return new CBattle_Double_ClawState(m_pOwner);

	//	else
	//	{
	//		switch (m_iRand)
	//		{
	//		case 0:
	//			return new CBattle_PouncingState(m_pOwner);

	//		case 1:
	//			return new CBattle_Shock_WaveState(m_pOwner);

	//		case 2:
	//			return new CBattle_Double_ClawState(m_pOwner);
	//		default:
	//			break;
	//		}
	//	}
	//}
	
	



	return nullptr;
}

void CBattle_WalkState::Enter()
{

	m_eStateId = STATE_ID::STATE_CALM;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_WALK_F);

	////


}

void CBattle_WalkState::Exit()
{
		
}