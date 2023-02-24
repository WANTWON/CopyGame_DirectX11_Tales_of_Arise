+-
#include "stdafx.h"

#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_SpearMultiState.h"
#include "AstralDoubt_Battle_HeadBeamState.h"
#include "AstralDoubt_Battle_UpperState.h"
#include "AstralDoubt_Battle_RushState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CBattleWalkState::CBattleWalkState(CAstralDoubt* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fMoveTime = ((rand() % 1000) *0.001f )*((rand() % 100) * 0.01f);
	
}

CAstralDoubt_State * CBattleWalkState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattleWalkState::Tick(_float fTimeDelta)
{
	
	//Find_Target();
	//m_fTarget_Distance = m_fOutPutTarget_Distance;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	CBaseObj* pOrigin_DamageCause = nullptr;
	pOrigin_DamageCause = m_pOwner->Get_OrginDamageCauser();
	//CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	////if (m_bTargetPickOn == false)
	////{
	//	if (pDamageCauser == nullptr)
	//	{
	//		if (m_pCurTarget == nullptr)
	//		{
	//			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	//			//if (m_pCurTarget == nullptr)
	//			//	return nullptr;

	//			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//			m_bTargetPickOn = true;
	//		}
	//		else
	//		{
	//			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//			m_bTargetPickOn = true;
	//		}
	//	}

	//	else if (pDamageCauser != nullptr)
	//	{
	//		if (m_pCurTarget == nullptr)
	//		{
	//			m_pCurTarget = pDamageCauser;
	//			m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
	//			m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	//			m_bTargetPickOn = true;
	//		}

	//		else
	//		{
	//			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//			m_bTargetPickOn = true;
	//		}
	//	}

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

					else
						return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);

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

					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
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
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
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
					return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_BRAVE);
				//return new CBattle_IdleState(m_pOwner, STATE_ID::STATE_LOOKOUT);
			}
		}

		else if (m_pCurTarget->Get_Info().fCurrentHp > 0)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	
	
	

	

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

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();


	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				//Camera Shaking 
				if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
					dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 0.5f, 0.1f);
			
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("EX_CLAW2_3_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				_matrix R_matWorld = m_pOwner->Get_Model()->Get_BonePtr("EX_CLAW2_3_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				R_matWorld.r[0] = XMVector4Normalize(R_matWorld.r[0]);
				R_matWorld.r[1] = XMVector4Normalize(R_matWorld.r[1]);
				R_matWorld.r[2] = XMVector4Normalize(R_matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);
				}

				else if (nullptr != m_pAtkColliderCom)
					m_pAtkColliderCom->Update(matWorld);

				if (nullptr == m_p2th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc2th;

					ColliderDesc2th.vScale = _float3(6.f, 6.f, 6.f);
					ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

					m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
					m_p2th_AtkColliderCom->Update(R_matWorld);
				}

				else if (nullptr != m_p2th_AtkColliderCom)
					m_p2th_AtkColliderCom->Update(R_matWorld);

				RELEASE_INSTANCE(CCollision_Manager);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_AtkColliderCom);

			m_pAtkColliderCom = nullptr;
			m_p2th_AtkColliderCom = nullptr;

			RELEASE_INSTANCE(CCollision_Manager);
		}
	}

	return nullptr;
}

CAstralDoubt_State * CBattleWalkState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	m_fTimeDeltaAcc += fTimeDelta;
	

	if (m_fTimeDeltaAcc > m_fMoveTime)
		m_iRand = rand() % 2;
	//if (m_pCurTarget == nullptr)
	//{
	//	m_pCurTarget = m_pOwner->Find_MinDistance_Target();

	//	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	m_bTargetPickOn = true;
	//}

	/*if (m_fTarget_Distance <= 15.f)
		m_Testbool = true;*/
	m_Testbool = true;
	//_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(m_vCurTargetPos);

	if (m_Testbool)
	{
		if (m_fTarget_Distance > 24.f)
		{
			if (m_ePreState_Id != CAstralDoubt_State::STATE_HEADBEAM)
			{
				if (bIs_TargetInFront == true)
				{
					if (m_bBeamTargetOn == false)
					{
						_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAt(vPosition);
						m_bBeamTargetOn = true;
						return new CBattle_HeadBeamState(m_pOwner);
					}
					
				}
				
				else if (bIs_TargetInFront == false)
				{
					if (m_bBeamTargetOn == false)
					{
						_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAt(vPosition);
						m_bBeamTargetOn = true;
						return new CBattle_HeadBeamState(m_pOwner);
					}
				}

			}

			else if (m_ePreState_Id == CAstralDoubt_State::STATE_HEADBEAM)
			{
				_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAt(vPosition);
				m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.55f, m_pOwner->Get_Navigation());
			}
		}
			

		else
		{
			_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAt(vPosition);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.55f, m_pOwner->Get_Navigation());

			
				if (m_fTarget_Distance <= 20.f)
				{
					if (m_fTarget_Distance <= 6.f)
					{
						if (m_PreState_IsSpin == false)
							return new CBattle_720Spin_FirstState(m_pOwner, CAstralDoubt_State::STATE_IDLE);
						else
						{
							if(m_ePreState_Id == CAstralDoubt_State::STATE_UPPER)
								return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

							else if(CAstralDoubt_State::STATE_SPEARMULTI)
								return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

							else
							{
								switch (m_iRand)
								{
								case 0:
									return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

								case 1:
									return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

								default:
									break;
								}
							}
							
						}
					}

					else
					{
						if (bIs_TargetInFront == true)
						{
							if (m_ePreState_Id == CAstralDoubt_State::STATE_UPPER)
								return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

							else if (CAstralDoubt_State::STATE_SPEARMULTI)
								return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

							else
							{
								switch (m_iRand)
								{
								case 0:
									return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

								case 1:
									return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

								default:
									break;
								}
							}
						}

						else
						{
							m_pOwner->Get_Transform()->LookAt(vPosition);

							if (m_ePreState_Id == CAstralDoubt_State::STATE_UPPER)
								return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

							else if (CAstralDoubt_State::STATE_SPEARMULTI)
								return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

							else
							{
								switch (m_iRand)
								{
								case 0:
									return new CBattle_SpearMultiState(m_pOwner, CAstralDoubt_State::STATE_END);

								case 1:
									return new CBattle_UpperState(m_pOwner, CAstralDoubt_State::STATE_END);

								default:
									break;
								}
							}
						}
					}
				}

				/*else
					return new CBattleWalkState(m_pOwner, CAstralDoubt_State::STATE_IDLE);*/
				/*}*/
			
		}
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);

	if (nullptr != m_p2th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p2th_AtkColliderCom);
#endif // _DEBUG

	return nullptr;
}


void CBattleWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::MOVE_WALK_F);
	
}

void CBattleWalkState::Exit()
{
	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);
}

