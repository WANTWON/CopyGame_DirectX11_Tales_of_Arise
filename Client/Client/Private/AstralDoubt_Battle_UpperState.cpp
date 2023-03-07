#include "stdafx.h"

#include "AstralDoubt_Battle_UpperState.h"
#include "GameInstance.h"
#include "AstralDoubt_ChaseState.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_DetectStopState.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"
#include "Effect.h"

using namespace Astral_Doubt;

CBattle_UpperState::CBattle_UpperState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_UpperState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_UpperState::Tick(_float fTimeDelta)
{
	Find_Target();
	m_fTarget_Distance = m_fOutPutTarget_Distance;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	/*m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

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
	}*/

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				if (m_fSoundStart != pEvent.fStartTime)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAus_Attack_Upper.wav"), SOUND_EFFECT, 0.4f);
					m_fSoundStart = pEvent.fStartTime;
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				_matrix matWorld_2th = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld_2th.r[0] = XMVector4Normalize(matWorld_2th.r[0]);
				matWorld_2th.r[1] = XMVector4Normalize(matWorld_2th.r[1]);
				matWorld_2th.r[2] = XMVector4Normalize(matWorld_2th.r[2]);

				_matrix matWorld_3th = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND3_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld_3th.r[0] = XMVector4Normalize(matWorld_3th.r[0]);
				matWorld_3th.r[1] = XMVector4Normalize(matWorld_3th.r[1]);
				matWorld_3th.r[2] = XMVector4Normalize(matWorld_3th.r[2]);

				_matrix matWorld_4th = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld_4th.r[0] = XMVector4Normalize(matWorld_4th.r[0]);
				matWorld_4th.r[1] = XMVector4Normalize(matWorld_4th.r[1]);
				matWorld_4th.r[2] = XMVector4Normalize(matWorld_4th.r[2]);

				_matrix matWorld_5th = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld_5th.r[0] = XMVector4Normalize(matWorld_5th.r[0]);
				matWorld_5th.r[1] = XMVector4Normalize(matWorld_5th.r[1]);
				matWorld_5th.r[2] = XMVector4Normalize(matWorld_5th.r[2]);

				_matrix matWorld_6th = m_pOwner->Get_Model()->Get_BonePtr("IJ_ROLL_HAND3_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
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
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				if (!m_bSlash_1)
				{
					if (!strcmp(pEvent.szName, "Slash_1"))
					{
						vector<CEffect*> InOutUpper = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_1.dat"), mWorldMatrix);

						_matrix mParticlesMatrix;
						
						mParticlesMatrix = InOutUpper[0]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);
						
						mParticlesMatrix = InOutUpper[2]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						mParticlesMatrix = InOutUpper[4]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						m_bSlash_1 = true;
					}
				}
				if (!m_bSlash_2)
				{
					if (!strcmp(pEvent.szName, "Slash_2"))
					{
						vector<CEffect*> InOutUpper = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_2.dat"), mWorldMatrix);

						_matrix mParticlesMatrix;

						mParticlesMatrix = InOutUpper[0]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						mParticlesMatrix = InOutUpper[2]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						mParticlesMatrix = InOutUpper[4]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						m_bSlash_2 = true;
					}
				}
				if (!m_bSlash_3)
				{
					if (!strcmp(pEvent.szName, "Slash_3"))
					{
						vector<CEffect*> InOutUpper = CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_3.dat"), mWorldMatrix);

						_matrix mParticlesMatrix;

						mParticlesMatrix = InOutUpper[0]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						mParticlesMatrix = InOutUpper[2]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						mParticlesMatrix = InOutUpper[4]->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Astral_Doubt_InOutUpper_Particles.dat"), mParticlesMatrix);

						m_bSlash_3 = true;
					}
				}
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
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

	return nullptr;
}

CAstralDoubt_State * CBattle_UpperState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	if (nullptr != m_pAtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;
		//m_bCausedDamage = true;

		if (m_bCollision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

				m_bCollision = true;
			}
		}

		if (m_b2th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

				m_b2th_Collision = true;
			}
		}

		if (m_b3th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p3th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

				m_b3th_Collision = true;
			}
		}

		if (m_b4th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p4th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

				m_b4th_Collision = true;
			}
		}


		if (m_b5th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p5th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

				m_b5th_Collision = true;
			}
		}


		if (m_b6th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p6th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (600 - 400 + 1) + 400, m_pOwner, true);

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

	m_fTimeDeltaAcc += fTimeDelta;
	
	if (m_bIsAnimationFinished)
	{
		/*if (m_fTarget_Distance <= 10.f)
		{
			return new CBattle_720Spin_FirstState(m_pOwner);
		}

		else*/
		return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_UPPER);
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
#endif 

	return nullptr;
}

void CBattle_UpperState::Enter()
{
	Reset_Effect();

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_IN_OUT_UPPER);	
}

void CBattle_UpperState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

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

void CBattle_UpperState::Reset_Effect()
{
	m_bSlash_1 = false;
	m_bSlash_2 = false;
	m_bSlash_3 = false;
}