#include "stdafx.h"

#include "AstralDoubt_Battle_RushState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CBattle_RushState::CBattle_RushState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_RushState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_RushState::Tick(_float fTimeDelta)
{
	
	
	switch (m_ePreState_Id)
	{
	case CAstralDoubt_State::STATE_RUSH_START:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;

	case CAstralDoubt_State::STATE_RUSH_LOOP:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);
		break;

	case CAstralDoubt_State::STATE_RUSH_END:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);
		break;

	default:
		break;
	}


	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

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

					ColliderDesc.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc.vPosition = _float3(0.f, -4.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);
				}
				else if (nullptr != m_pAtkColliderCom)
					m_pAtkColliderCom->Update(matWorld);


				if (nullptr == m_p2th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc2th;

					ColliderDesc2th.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc2th.vPosition = _float3(0.f, -4.f, 0.f);

					m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
					m_p2th_AtkColliderCom->Update(matWorld_2th);
				}
				else if (nullptr != m_p2th_AtkColliderCom)
					m_p2th_AtkColliderCom->Update(matWorld_2th);


				if (nullptr == m_p3th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc3th;

					ColliderDesc3th.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc3th.vPosition = _float3(0.f, -4.f, 0.f);

					m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
					m_p3th_AtkColliderCom->Update(matWorld_3th);
				}
				else if (nullptr != m_p3th_AtkColliderCom)
					m_p3th_AtkColliderCom->Update(matWorld_3th);


				if (nullptr == m_p4th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc4th;

					ColliderDesc4th.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc4th.vPosition = _float3(0.f, -4.f, 0.f);

					m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
					m_p4th_AtkColliderCom->Update(matWorld_4th);
				}

				else if (nullptr != m_p4th_AtkColliderCom)
					m_p4th_AtkColliderCom->Update(matWorld_4th);

				if (nullptr == m_p5th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc5th;

					ColliderDesc5th.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc5th.vPosition = _float3(0.f, -4.f, 0.f);

					m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
					m_p5th_AtkColliderCom->Update(matWorld_5th);
				}
				else if (nullptr != m_p5th_AtkColliderCom)
					m_p5th_AtkColliderCom->Update(matWorld_5th);


				if (nullptr == m_p6th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc6th;

					ColliderDesc6th.vScale = _float3(15.f, 15.f, 15.f);
					ColliderDesc6th.vPosition = _float3(0.f, -4.f, 0.f);

					m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
					m_p6th_AtkColliderCom->Update(matWorld_6th);
				}
				else if (nullptr != m_p6th_AtkColliderCom)
					m_p6th_AtkColliderCom->Update(matWorld_6th);

				RELEASE_INSTANCE(CCollision_Manager);
			}
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

	return nullptr;
}

CAstralDoubt_State * CBattle_RushState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;


	if (m_bIsAnimationFinished)
	{
		if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_START)
			return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_RUSH_LOOP);

		else if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_LOOP)
		{
			return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_RUSH_END);
		}

		else if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_END)
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_SPEARMULTI);
	}





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
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

				m_bCollision = true;
			}
		}

		if (m_b2th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p2th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

				m_b2th_Collision = true;
			}
		}

		if (m_b3th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p3th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

				m_b3th_Collision = true;
			}
		}

		if (m_b4th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p4th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

				m_b4th_Collision = true;
			}
		}


		if (m_b5th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p5th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

				m_b5th_Collision = true;
			}
		}


		if (m_b6th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p6th_AtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (800 - 700 + 1) + 700, m_pOwner, true);

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

void CBattle_RushState::Enter()
{
	if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_START)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_START);
	
	}

	else if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_LOOP)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Attack_Rush.wav"), SOUND_VOICE, 0.6f);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_LOOP);
	}

	else if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_END)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_END);

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
}

void CBattle_RushState::Exit()
{
	if (m_eStateId == Client::CAstralDoubt_State::STATE_RUSH_END)
	{
		m_pOwner->Set_FinishGoingDown();
		m_pOwner->Set_FinishDownState();
		CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	}
	
	/*if (m_eStateId == Client::CAstralDoubt_State::STATE_RUSH_LOOP)
	{
		
	}*/


	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);
	Safe_Release(m_p3th_AtkColliderCom);
	Safe_Release(m_p4th_AtkColliderCom);
	Safe_Release(m_p5th_AtkColliderCom);
	Safe_Release(m_p6th_AtkColliderCom);
}