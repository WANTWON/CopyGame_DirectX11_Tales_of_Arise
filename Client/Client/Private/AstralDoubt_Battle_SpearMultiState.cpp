#include "stdafx.h"

#include "AstralDoubt_Battle_SpearMultiState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CBattle_SpearMultiState::CBattle_SpearMultiState(CAstralDoubt* pAstralDoubt, STATE_ID eState)
{
	m_pOwner = pAstralDoubt;
	m_eStateId = eState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_SpearMultiState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_SpearMultiState::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


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

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	
	if (m_eStateId == CAstralDoubt_State::STATE_FOOTPRESS)
	{
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					//Camera Shaking 
					if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
						dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.4f, 0.1f);


					/*CCollider* pSpherCollider = m_pOwner->Get_SPHERECollider();
					if (pSpherCollider)
					{
						CBaseObj* pCollisionTarget = nullptr;

						if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, pSpherCollider, &pCollisionTarget))
						{
							CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
							if (pCollided)
								pCollided->Take_Damage(rand() % 100, m_pOwner, true);
						}
					}*/

					CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

					_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("EX_CLAW2_3_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
					matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
					matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

					_matrix R_matWorld = m_pOwner->Get_Model()->Get_BonePtr("EX_CLAW2_3_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					R_matWorld.r[0] = XMVector4Normalize(R_matWorld.r[0]);
					R_matWorld.r[1] = XMVector4Normalize(R_matWorld.r[1]);
					R_matWorld.r[2] = XMVector4Normalize(R_matWorld.r[2]);

					if (nullptr == m_pFootColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc;

						ColliderDesc.vScale = _float3(14.f, 14.f, 14.f);
						ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

						m_pFootColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pFootColliderCom->Update(matWorld);

						
					}

					else if (nullptr != m_pFootColliderCom)
						m_pFootColliderCom->Update(matWorld);

					if (nullptr == m_p2th_FootColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(14.f, 14.f, 14.f);
						ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p2th_FootColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_FootColliderCom->Update(R_matWorld);

					}

					else if (nullptr != m_p2th_FootColliderCom)
						m_p2th_FootColliderCom->Update(R_matWorld);

					RELEASE_INSTANCE(CCollision_Manager);


				}

				

				else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

					CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

					_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
					matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
					matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

					_matrix matWorld_2th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_2th.r[0] = XMVector4Normalize(matWorld_2th.r[0]);
					matWorld_2th.r[1] = XMVector4Normalize(matWorld_2th.r[1]);
					matWorld_2th.r[2] = XMVector4Normalize(matWorld_2th.r[2]);

					_matrix matWorld_3th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_3th.r[0] = XMVector4Normalize(matWorld_3th.r[0]);
					matWorld_3th.r[1] = XMVector4Normalize(matWorld_3th.r[1]);
					matWorld_3th.r[2] = XMVector4Normalize(matWorld_3th.r[2]);

					_matrix matWorld_4th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_4th.r[0] = XMVector4Normalize(matWorld_4th.r[0]);
					matWorld_4th.r[1] = XMVector4Normalize(matWorld_4th.r[1]);
					matWorld_4th.r[2] = XMVector4Normalize(matWorld_4th.r[2]);

					_matrix matWorld_5th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_5th.r[0] = XMVector4Normalize(matWorld_5th.r[0]);
					matWorld_5th.r[1] = XMVector4Normalize(matWorld_5th.r[1]);
					matWorld_5th.r[2] = XMVector4Normalize(matWorld_5th.r[2]);

					_matrix matWorld_6th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_6th.r[0] = XMVector4Normalize(matWorld_6th.r[0]);
					matWorld_6th.r[1] = XMVector4Normalize(matWorld_6th.r[1]);
					matWorld_6th.r[2] = XMVector4Normalize(matWorld_6th.r[2]);


					if (nullptr == m_pAtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc;

						ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc.vPosition = _float3(-5.f, 0.f, 0.f);

						m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pAtkColliderCom->Update(matWorld);

						
					}
					else if (nullptr != m_pAtkColliderCom)
						m_pAtkColliderCom->Update(matWorld);


					if (nullptr == m_p2th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc2th.vPosition = _float3(-5.f, 0.f, 0.f);

						m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_AtkColliderCom->Update(matWorld_2th);

						
					}
					else if (nullptr != m_p2th_AtkColliderCom)
						m_p2th_AtkColliderCom->Update(matWorld_2th);


					if (nullptr == m_p3th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc3th;

						ColliderDesc3th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc3th.vPosition = _float3(-5.f, 0.f, 0.f);

						m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
						m_p3th_AtkColliderCom->Update(matWorld_3th);

		
					}
					else if (nullptr != m_p3th_AtkColliderCom)
						m_p3th_AtkColliderCom->Update(matWorld_3th);


					if (nullptr == m_p4th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc4th;

						ColliderDesc4th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc4th.vPosition = _float3(-5.f, 0.f, 0.f);

						m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
						m_p4th_AtkColliderCom->Update(matWorld_4th);

						
					}

					else if (nullptr != m_p4th_AtkColliderCom)
						m_p4th_AtkColliderCom->Update(matWorld_4th);

					if (nullptr == m_p5th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc5th;

						ColliderDesc5th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc5th.vPosition = _float3(-5.f, 0.f, 0.f);

						m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
						m_p5th_AtkColliderCom->Update(matWorld_5th);

						
					}
					else if (nullptr != m_p5th_AtkColliderCom)
						m_p5th_AtkColliderCom->Update(matWorld_5th);


					if (nullptr == m_p6th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc6th;

						ColliderDesc6th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc6th.vPosition = _float3(-5.f, 0.f, 0.f);

						m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
						m_p6th_AtkColliderCom->Update(matWorld_6th);

						
					}
					else if (nullptr != m_p6th_AtkColliderCom)
						m_p6th_AtkColliderCom->Update(matWorld_6th);

					RELEASE_INSTANCE(CCollision_Manager);
				}
			}

			else if (!pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_AtkColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p3th_AtkColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p4th_AtkColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p5th_AtkColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p6th_AtkColliderCom);

				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pFootColliderCom);
				pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_FootColliderCom);

				m_pAtkColliderCom = nullptr;
				m_p2th_AtkColliderCom = nullptr;
				m_p3th_AtkColliderCom = nullptr;
				m_p4th_AtkColliderCom = nullptr;
				m_p5th_AtkColliderCom = nullptr;
				m_p6th_AtkColliderCom = nullptr;

				m_pFootColliderCom = nullptr;
				m_p2th_FootColliderCom = nullptr;

				RELEASE_INSTANCE(CCollision_Manager);
			}

			/*else if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType && !pEvent.isPlay)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				

				m_pFootColliderCom = nullptr;
				m_p2th_FootColliderCom = nullptr;

				

				RELEASE_INSTANCE(CCollision_Manager);
			}*/
		}
	}
	else if (m_eStateId == CAstralDoubt_State::STATE_SPEARMULTI)
	{
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{

				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

					_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
					matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
					matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

					_matrix matWorld_2th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_2th.r[0] = XMVector4Normalize(matWorld_2th.r[0]);
					matWorld_2th.r[1] = XMVector4Normalize(matWorld_2th.r[1]);
					matWorld_2th.r[2] = XMVector4Normalize(matWorld_2th.r[2]);

					_matrix matWorld_3th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_3th.r[0] = XMVector4Normalize(matWorld_3th.r[0]);
					matWorld_3th.r[1] = XMVector4Normalize(matWorld_3th.r[1]);
					matWorld_3th.r[2] = XMVector4Normalize(matWorld_3th.r[2]);

					_matrix matWorld_4th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_4th.r[0] = XMVector4Normalize(matWorld_4th.r[0]);
					matWorld_4th.r[1] = XMVector4Normalize(matWorld_4th.r[1]);
					matWorld_4th.r[2] = XMVector4Normalize(matWorld_4th.r[2]);

					_matrix matWorld_5th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_5th.r[0] = XMVector4Normalize(matWorld_5th.r[0]);
					matWorld_5th.r[1] = XMVector4Normalize(matWorld_5th.r[1]);
					matWorld_5th.r[2] = XMVector4Normalize(matWorld_5th.r[2]);

					_matrix matWorld_6th = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld_6th.r[0] = XMVector4Normalize(matWorld_6th.r[0]);
					matWorld_6th.r[1] = XMVector4Normalize(matWorld_6th.r[1]);
					matWorld_6th.r[2] = XMVector4Normalize(matWorld_6th.r[2]);


					if (nullptr == m_pAtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc;

						ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc.vPosition = _float3(0.f, -8.f, 0.f);

						m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pAtkColliderCom->Update(matWorld);


					}
					else if (nullptr != m_pAtkColliderCom)
						m_pAtkColliderCom->Update(matWorld);


					if (nullptr == m_p2th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc2th.vPosition = _float3(0.f, -8.f, 0.f);

						m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_AtkColliderCom->Update(matWorld_2th);


					}
					else if (nullptr != m_p2th_AtkColliderCom)
						m_p2th_AtkColliderCom->Update(matWorld_2th);


					if (nullptr == m_p3th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc3th;

						ColliderDesc3th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc3th.vPosition = _float3(0.f, -8.f, 0.f);

						m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
						m_p3th_AtkColliderCom->Update(matWorld_3th);


					}
					else if (nullptr != m_p3th_AtkColliderCom)
						m_p3th_AtkColliderCom->Update(matWorld_3th);


					if (nullptr == m_p4th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc4th;

						ColliderDesc4th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc4th.vPosition = _float3(0.f, -8.f, 0.f);

						m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
						m_p4th_AtkColliderCom->Update(matWorld_4th);


					}

					else if (nullptr != m_p4th_AtkColliderCom)
						m_p4th_AtkColliderCom->Update(matWorld_4th);

					if (nullptr == m_p5th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc5th;

						ColliderDesc5th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc5th.vPosition = _float3(0.f, -8.f, 0.f);

						m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
						m_p5th_AtkColliderCom->Update(matWorld_5th);
					}
					else if (nullptr != m_p5th_AtkColliderCom)
						m_p5th_AtkColliderCom->Update(matWorld_5th);


					if (nullptr == m_p6th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc6th;

						ColliderDesc6th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc6th.vPosition = _float3(0.f, -8.f, 0.f);

						m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
						m_p6th_AtkColliderCom->Update(matWorld_6th);

						pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
					}
					else if (nullptr != m_p6th_AtkColliderCom)
						m_p6th_AtkColliderCom->Update(matWorld_6th);

					RELEASE_INSTANCE(CCollision_Manager);
					}
				

				else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
				{
					CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_AtkColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p3th_AtkColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p4th_AtkColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p5th_AtkColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p6th_AtkColliderCom);

					m_pAtkColliderCom = nullptr;
					m_p2th_AtkColliderCom = nullptr;
					m_p3th_AtkColliderCom = nullptr;
					m_p4th_AtkColliderCom = nullptr;
					m_p5th_AtkColliderCom = nullptr;
					m_p6th_AtkColliderCom = nullptr;

					RELEASE_INSTANCE(CCollision_Manager);
				}
			}
		}
	}
	return nullptr;
}

CAstralDoubt_State * CBattle_SpearMultiState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	
	if (!m_bCausedDamage)
	{
		if (nullptr != m_pAtkColliderCom)
		{
			CBaseObj* pCollisionTarget = nullptr;
			//m_bCausedDamage = true;

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p3th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p4th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p5th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p6th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner);
			}
		}
	}
	//FootAttack
	if (!m_bFootCauseDamage)
	{
		if (nullptr != m_pFootColliderCom)
		{
			CBaseObj* pFootCollisionTarget = nullptr;
			//m_bFootCauseDamage = true;

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pFootColliderCom, &pFootCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pFootCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner, true);
			}

			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_FootColliderCom, &pFootCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pFootCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % 100, m_pOwner, true);
			}
		}
	}

	if (m_bIsAnimationFinished)
	{
		//if (m_fTarget_Distance <= 10.f)
		//{
		//	return new CBattle_720Spin_FirstState(m_pOwner);
		//}

		//else
		switch (m_eStateId)
		{
		case CAstralDoubt_State::STATE_SPEARMULTI:
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
			
			break;

		case CAstralDoubt_State::STATE_FOOTPRESS:
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_FOOTPRESS);
			break;

		default:
			break;
		}

		
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);


	if (nullptr != m_p2th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p2th_AtkColliderCom);

	if (nullptr != m_p3th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p3th_AtkColliderCom);

	if (nullptr != m_p4th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p4th_AtkColliderCom);

	if (nullptr != m_p5th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p5th_AtkColliderCom);

	if (nullptr != m_p6th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p6th_AtkColliderCom);

	//FootAttack
	if (nullptr != m_pFootColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p6th_AtkColliderCom);

	if (nullptr != m_p2th_FootColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p6th_AtkColliderCom);
#endif 


	return nullptr;
}

void CBattle_SpearMultiState::Enter()
{
	
	
	switch (m_eStateId)
	{
	case CAstralDoubt_State::STATE_SPEARMULTI:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_MULTI);
		break;

	case CAstralDoubt_State::STATE_FOOTPRESS:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_HANDSTAND_FOOTPRESS);
		break;
	
	default:
		break;
	}

}

void CBattle_SpearMultiState::Exit()
{
	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);
	Safe_Release(m_p3th_AtkColliderCom);
	Safe_Release(m_p4th_AtkColliderCom);
	Safe_Release(m_p5th_AtkColliderCom);
	Safe_Release(m_p6th_AtkColliderCom);
	
	m_bCausedDamage = false;
	m_bFootCauseDamage = false;
}