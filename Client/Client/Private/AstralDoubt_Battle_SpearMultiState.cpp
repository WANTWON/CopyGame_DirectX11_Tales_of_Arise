#include "stdafx.h"

#include "AstralDoubt_Battle_SpearMultiState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"
#include "Effect.h"
#include "BossSkills.h"

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
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		if (m_eStateId == CAstralDoubt_State::STATE_SPEARMULTI)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.025f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	
	if (m_eStateId == CAstralDoubt_State::STATE_FOOTPRESS)
	{
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{       
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
						dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.4f, 0.1f);

					if (m_bAdventSound == false)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Boss_Asu_FootPress.wav"), SOUND_VOICE, 0.25f);
						m_bAdventSound = true;
					}

					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

					_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("FOOT_L"/*"EX_CLAW2_3_L"*/)->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
					matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
					matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

					_matrix R_matWorld = m_pOwner->Get_Model()->Get_BonePtr("FOOT_R"/*"EX_CLAW2_3_R"*/)->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
					R_matWorld.r[0] = XMVector4Normalize(R_matWorld.r[0]);
					R_matWorld.r[1] = XMVector4Normalize(R_matWorld.r[1]);
					R_matWorld.r[2] = XMVector4Normalize(R_matWorld.r[2]);

					if (nullptr == m_pFootColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc;

						ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
						//ColliderDesc.vScale = _float3(14.f, 14.f, 14.f);
						//ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

						m_pFootColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pFootColliderCom->Update(matWorld);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pFootColliderCom, m_pOwner);
					}
					else if (nullptr != m_pFootColliderCom)
						m_pFootColliderCom->Update(matWorld);

					if (nullptr == m_p2th_FootColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(15.f, 15.f, 15.f);
						ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p2th_FootColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_FootColliderCom->Update(R_matWorld);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_FootColliderCom, m_pOwner);
					}
					else if (nullptr != m_p2th_FootColliderCom)
						m_p2th_FootColliderCom->Update(R_matWorld);

					
				}
				else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

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

						ColliderDesc.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

						m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pAtkColliderCom->Update(matWorld);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_pAtkColliderCom)
						m_pAtkColliderCom->Update(matWorld);

					if (nullptr == m_p2th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_AtkColliderCom->Update(matWorld_2th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p2th_AtkColliderCom)
						m_p2th_AtkColliderCom->Update(matWorld_2th);

					if (nullptr == m_p3th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc3th;

						ColliderDesc3th.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc3th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
						m_p3th_AtkColliderCom->Update(matWorld_3th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p3th_AtkColliderCom)
						m_p3th_AtkColliderCom->Update(matWorld_3th);

					if (nullptr == m_p4th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc4th;

						ColliderDesc4th.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc4th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
						m_p4th_AtkColliderCom->Update(matWorld_4th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p4th_AtkColliderCom)
						m_p4th_AtkColliderCom->Update(matWorld_4th);

					if (nullptr == m_p5th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc5th;

						ColliderDesc5th.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc5th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
						m_p5th_AtkColliderCom->Update(matWorld_5th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p5th_AtkColliderCom)
						m_p5th_AtkColliderCom->Update(matWorld_5th);

					if (nullptr == m_p6th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc6th;

						ColliderDesc6th.vScale = _float3(8.f, 8.f, 8.f);
						ColliderDesc6th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
						m_p6th_AtkColliderCom->Update(matWorld_6th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p6th_AtkColliderCom)
						m_p6th_AtkColliderCom->Update(matWorld_6th);

					
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					_matrix mWorldMatrix = XMMatrixIdentity();

					CHierarchyNode* pBone = nullptr;
					_float4x4 PivotMatrix = m_pOwner->Get_Model()->Get_PivotFloat4x4();
					_matrix ParentWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

					if (!m_bHit_1)
					{
						if (!strcmp(pEvent.szName, "Hit_1"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;
							
							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);

							m_bHit_1 = true;
						}
					}
					if (!m_bHit_2)
					{
						if (!strcmp(pEvent.szName, "Hit_2"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;
							
							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);

							m_bHit_2 = true;
						}
					}
					if (!m_bHit_3)
					{
						if (!strcmp(pEvent.szName, "Hit_3"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);

							m_bHit_3 = true;
						}
					}
					if (!m_bHit_4)
					{
						if (!strcmp(pEvent.szName, "Hit_4"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);
							
							m_bHit_4 = true;
						}
					}
					if (!m_bHit_5)
					{
						if (!strcmp(pEvent.szName, "Hit_5"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);

							m_bHit_5 = true;
						}
					}
					if (!m_bHit_6)
					{
						if (!strcmp(pEvent.szName, "Hit_6"))
						{
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_float4 vPosition;
							XMStoreFloat4(&vPosition, SocketMatrix.r[3]);
							vPosition.y = .01f;

							mWorldMatrix.r[3] = XMLoadFloat4(&vPosition);

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand.dat"), mWorldMatrix);

							m_bHit_6 = true;
						}
					}
					if (!m_bHit_7)
					{
						if (!strcmp(pEvent.szName, "Hit_7"))
						{
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

							vector<CEffect*> Effects = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand_Big.dat"), mWorldMatrix);
							for (auto& pEffect : Effects)
							{
								if (pEffect)
								{
									_vector vPosition = pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
									_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);

									vPosition += XMVector4Normalize(vLook) * 3;

									_float4 vOffset;
									XMStoreFloat4(&vOffset, vPosition);

									if (!wcscmp(pEffect->Get_PrototypeId(), TEXT("Plane")))
										vOffset.y = 0.02f;
									else if (!wcscmp(pEffect->Get_PrototypeId(), TEXT("Smoke_Mask")))
										vOffset.y = 2.f;
									else
										vOffset.y = 0.01f;

									pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, XMLoadFloat4(&vOffset));
								}
							}

							vector<CEffect*> EffectsRing = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_HandStand_Ring.dat"), mWorldMatrix);
							for (auto& pEffect : EffectsRing)
							{
								if (pEffect)
								{
									_vector vPosition = pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
									_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);

									vPosition += XMVector4Normalize(vLook) * 3;

									pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
							}

							m_bHit_7 = true;
						}
					}
				}
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pFootColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_FootColliderCom, m_pOwner);

					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);
					
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pFootColliderCom);
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_FootColliderCom);

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

					m_pFootColliderCom = nullptr;
					m_p2th_FootColliderCom = nullptr;

					
				}
			}
		}
	}
	else if (m_eStateId == CAstralDoubt_State::STATE_SPEARMULTI)
	{
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
						dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 1.4f, 0.1f);

					if (m_bSpearMultiEndSound == false)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Attack_SpearMultiEnd.wav"), SOUND_VOICE, 0.4f);
						m_bSpearMultiEndSound = true;
					}
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

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

						ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
						//ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
						//ColliderDesc.vPosition = _float3(0.f, -8.f, 0.f);

						m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						m_pAtkColliderCom->Update(matWorld);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_pAtkColliderCom)
						m_pAtkColliderCom->Update(matWorld);

					if (nullptr == m_p2th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc2th;

						ColliderDesc2th.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
						m_p2th_AtkColliderCom->Update(matWorld_2th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p2th_AtkColliderCom)
						m_p2th_AtkColliderCom->Update(matWorld_2th);

					if (nullptr == m_p3th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc3th;

						ColliderDesc3th.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc3th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
						m_p3th_AtkColliderCom->Update(matWorld_3th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p3th_AtkColliderCom)
						m_p3th_AtkColliderCom->Update(matWorld_3th);

					if (nullptr == m_p4th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc4th;

						ColliderDesc4th.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc4th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
						m_p4th_AtkColliderCom->Update(matWorld_4th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p4th_AtkColliderCom)
						m_p4th_AtkColliderCom->Update(matWorld_4th);

					if (nullptr == m_p5th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc5th;

						ColliderDesc5th.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc5th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
						m_p5th_AtkColliderCom->Update(matWorld_5th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p5th_AtkColliderCom)
						m_p5th_AtkColliderCom->Update(matWorld_5th);

					if (nullptr == m_p6th_AtkColliderCom)
					{
						CCollider::COLLIDERDESC		ColliderDesc6th;

						ColliderDesc6th.vScale = _float3(5.f, 5.f, 5.f);
						ColliderDesc6th.vPosition = _float3(0.f, 0.f, 0.f);

						m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
						m_p6th_AtkColliderCom->Update(matWorld_6th);

						pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);
					}
					else if (nullptr != m_p6th_AtkColliderCom)
						m_p6th_AtkColliderCom->Update(matWorld_6th);

					
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					_matrix mWorldMatrix = XMMatrixIdentity();

					CHierarchyNode* pBone = nullptr;
					_float4x4 PivotMatrix = m_pOwner->Get_Model()->Get_PivotFloat4x4();
					_matrix ParentWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

					CBullet::BULLETDESC BulletDesc;
					BulletDesc.eCollisionGroup = PLAYER;
					BulletDesc.pOwner = m_pOwner;
					BulletDesc.fDeadTime = 10.f;
					BulletDesc.fVelocity = 2.f;
					BulletDesc.vTargetDir = XMVector4Normalize(m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK));

					if (!m_bBullet_1)
					{
						if (!strcmp(pEvent.szName, "Bullet_1"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_1;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_1 = true;
						}
					}
					if (!m_bBullet_2)
					{
						if (!strcmp(pEvent.szName, "Bullet_2"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_2;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_2 = true;
						}
					}
					if (!m_bBullet_3)
					{
						if (!strcmp(pEvent.szName, "Bullet_3"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_3;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_3 = true;
						}
					}
					if (!m_bBullet_4)
					{
						if (!strcmp(pEvent.szName, "Bullet_4"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE3_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_4;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_4 = true;
						}
					}
					if (!m_bBullet_5)
					{
						if (!strcmp(pEvent.szName, "Bullet_5"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE2_3_R");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_5;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_5 = true;
						}
					}
					if (!m_bBullet_6)
					{
						if (!strcmp(pEvent.szName, "Bullet_6"))
						{
							/* Effect */
							pBone = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_3_L");
							_matrix	SocketMatrix = pBone->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&PivotMatrix) * ParentWorldMatrix;

							_vector vSocketPosition = SocketMatrix.r[3];
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vSocketPosition += XMVector4Normalize(vLook) * 2;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vSocketPosition;

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Aura.dat"), mWorldMatrix);

							/* Bullet */
							BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_SPEAR_MULTI_6;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_bBullet_6 = true;
						}
					}
					if (!m_bSmash)
					{
						if (!strcmp(pEvent.szName, "Smash"))
						{
							/* Effect */
							_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
							vPosition += XMVector4Normalize(vLook) * 9;

							_float4 vOffset;
							XMStoreFloat4(&vOffset, vPosition);
							vOffset.y = 3.f;

							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = XMLoadFloat4(&vOffset);

							CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_Spear_Multi_Smash.dat"), mWorldMatrix);

							m_bSmash = true;
						}
					}
				}
			}
			else 
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
					pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);

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

					
				}
			}
		}
	}

	return nullptr;
}

CAstralDoubt_State * CBattle_SpearMultiState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (nullptr != m_pAtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;

		if (m_bCollision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 +1) + 400, m_pOwner);

				m_bCollision = true;
			}
		}

		if (m_b2th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner);

				m_b2th_Collision = true;
			}
		}

		if (m_b3th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p3th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner);

				m_b3th_Collision = true;
			}
		}

		if (m_b4th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p4th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner);

				m_b4th_Collision = true;
			}
		}

		if (m_b5th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p5th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner);

				m_b5th_Collision = true;
			}
		}

		if (m_b6th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p6th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner);

				m_b6th_Collision = true;
			}
		}
	}
	
	if (m_bCollision)
	{
		m_fAtkCollision_Delay += fTimeDelta;

		if (m_fAtkCollision_Delay >= 1.5f)
		{
			m_fAtkCollision_Delay = 0.f;
			m_bCollision = false;
		}
	}

	if (m_b2th_Collision)
	{
		m_f2th_AtkCollision_Delay += fTimeDelta;

		if (m_f2th_AtkCollision_Delay >= 1.5f)
		{
			m_f2th_AtkCollision_Delay = 0.f;
			m_b2th_Collision = false;
		}
	}

	if (m_b3th_Collision)
	{
		m_f3th_AtkCollision_Delay += fTimeDelta;

		if (m_f3th_AtkCollision_Delay >= 1.5f)
		{
			m_f3th_AtkCollision_Delay = 0.f;
			m_b3th_Collision = false;
		}
	}

	if (m_b4th_Collision)
	{
		m_f4th_AtkCollision_Delay += fTimeDelta;

		if (m_f4th_AtkCollision_Delay >= 1.5f)
		{
			m_f4th_AtkCollision_Delay = 0.f;
			m_b2th_Collision = false;
		}
	}

	if (m_b5th_Collision)
	{
		m_f5th_AtkCollision_Delay += fTimeDelta;

		if (m_f5th_AtkCollision_Delay >= 1.5f)
		{
			m_f5th_AtkCollision_Delay = 0.f;
			m_b5th_Collision = false;
		}
	}

	if (m_b6th_Collision)
	{
		m_f6th_AtkCollision_Delay += fTimeDelta;

		if (m_f6th_AtkCollision_Delay >= 1.5f)
		{
			m_f6th_AtkCollision_Delay = 0.f;
			m_b6th_Collision = false;
		}
	}

	if (m_eStateId == CAstralDoubt_State::STATE_FOOTPRESS)
	{
		if (nullptr != m_pFootColliderCom)
		{
			CBaseObj* pFootCollisionTarget = nullptr;
			//m_bFootCauseDamage = true;
			if (m_bFootCollision == false)
			{
				if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pFootColliderCom, &pFootCollisionTarget))
				{
					CPlayer* pCollided = dynamic_cast<CPlayer*>(pFootCollisionTarget);
					if (pCollided)
						pCollided->Take_Damage(rand() % (1100 - 1000 + 1) + 1000, m_pOwner, true);

					m_bFootCollision = true;
				}
			}

			if (m_b2th_FootCollision == false)
			{
				if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_FootColliderCom, &pFootCollisionTarget))
				{
					CPlayer* pCollided = dynamic_cast<CPlayer*>(pFootCollisionTarget);
					if (pCollided)
						pCollided->Take_Damage(rand() % (1100 - 1000 + 1) + 1000, m_pOwner, true);

					m_b2th_FootCollision = true;
				}
			}
		}

		if (m_bFootCollision)
		{
			m_fAtkFootCollision_Delay += fTimeDelta;

			if (m_fAtkFootCollision_Delay >= 0.8f)
			{
				m_fAtkFootCollision_Delay = 0.f;
				m_bFootCollision = false;
			}
		}

		if (m_b2th_FootCollision)
		{
			m_f2th_AtkFootCollision_Delay += fTimeDelta;

			if (m_f2th_AtkFootCollision_Delay >= 0.8f)
			{
				m_f2th_AtkFootCollision_Delay = 0.f;
				m_b2th_FootCollision = false;
			}
		}
	}
	else if (m_eStateId == CAstralDoubt_State::STATE_SPEARMULTI)
	{
		Find_Target();

		m_vActiveTargetPos = m_pActiveTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector vPosition = XMVectorSetY(m_vActiveTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAt(vPosition);
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case CAstralDoubt_State::STATE_SPEARMULTI:
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
		case CAstralDoubt_State::STATE_FOOTPRESS:
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_FOOTPRESS);
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
	
	if (nullptr != m_pFootColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pFootColliderCom);
	if (nullptr != m_p2th_FootColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p2th_FootColliderCom);
#endif 

	return nullptr;
}

void CBattle_SpearMultiState::Enter()
{
	Reset_Effect();

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

	m_pOwner->Get_Model()->Reset();
}

void CBattle_SpearMultiState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);


	if (m_eStateId == CAstralDoubt_State::STATE_FOOTPRESS)
	{


		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pFootColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_FootColliderCom, m_pOwner);

		Safe_Release(m_pFootColliderCom);
		Safe_Release(m_p2th_FootColliderCom);

		/////////////////////////////////////////////
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);

		Safe_Release(m_pAtkColliderCom);
		Safe_Release(m_p2th_AtkColliderCom);
		Safe_Release(m_p3th_AtkColliderCom);
		Safe_Release(m_p4th_AtkColliderCom);
		Safe_Release(m_p5th_AtkColliderCom);
		Safe_Release(m_p6th_AtkColliderCom);

		
	}
	else
	{
		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p3th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p4th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p5th_AtkColliderCom, m_pOwner);
		pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p6th_AtkColliderCom, m_pOwner);

		Safe_Release(m_pAtkColliderCom);
		Safe_Release(m_p2th_AtkColliderCom);
		Safe_Release(m_p3th_AtkColliderCom);
		Safe_Release(m_p4th_AtkColliderCom);
		Safe_Release(m_p5th_AtkColliderCom);
		Safe_Release(m_p6th_AtkColliderCom);

		
	}
	m_bCausedDamage = false;
	m_bFootCauseDamage = false;
}

void CBattle_SpearMultiState::Reset_Effect()
{
	m_bHit_1 = false;
	m_bHit_2 = false;
	m_bHit_3 = false;
	m_bHit_4 = false;
	m_bHit_5 = false;
	m_bHit_6 = false;
	m_bHit_7 = false;

	
}