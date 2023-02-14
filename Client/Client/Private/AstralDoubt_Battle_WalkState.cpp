#include "stdafx.h"

#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_SpearMultiState.h"
#include "AstralDoubt_Battle_HeadBeamState.h"
#include "AstralDoubt_Battle_UpperState.h"
#include "AstralDoubt_Battle_RushState.h"

using namespace Astral_Doubt;

CBattleWalkState::CBattleWalkState(CAstralDoubt* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fMoveTime = ((rand() % 4000) *0.001f )*((rand() % 100) * 0.01f);
	
}

CAstralDoubt_State * CBattleWalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattleWalkState::Tick(_float fTimeDelta)
{
	
	Find_Target();
	m_fTarget_Distance = m_fOutPutTarget_Distance;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
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


	//if (m_pCurTarget == nullptr)
	//	return nullptr;

	//if (!m_bIsAnimationFinished)
	//	m_pOwner->Check_Navigation();


	//if (!m_bIsAnimationFinished)
	//{
	//	_vector vecTranslation;
	//	_float fRotationRadian;

	//	m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

	//	m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}

	return nullptr;
}

CAstralDoubt_State * CBattleWalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_fTarget_Distance <= 15.f)
		m_Testbool = true;

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(vTargetPos);

	if (m_Testbool)
	{
		if (m_fTarget_Distance > 15.f && m_ePreState_Id != CAstralDoubt_State::STATE_HEADBEAM && bIs_TargetInFront == true)
		{
			return new CBattle_HeadBeamState(m_pOwner);

			/*if (m_fTimeDeltaAcc >= m_fMoveTime)
			{
				switch (rand()%2)
				{
				case 0:
					return new CBattle_HeadBeamState(m_pOwner);

				case 1:
					return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_RUN);
					
				default:
					break;
				}
			}*/
		}
			

		else
		{
			m_pOwner->Get_Transform()->LookAt(vTargetPos);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.55f, m_pOwner->Get_Navigation());

			if (m_fTarget_Distance <= 6.f)
			{
				if (m_fTimeDeltaAcc >= m_fMoveTime)
				{
					switch (rand() %2)
					{
					case 0:
						return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
					
					case 1:
						return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
						
					default:
						break;
					}
				}
				

			}
		}
	}
	return nullptr;
}


void CBattleWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_WALK_F);
	
}

void CBattleWalkState::Exit()
{
	
}

