#include "stdafx.h"

#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CBattle_720Spin_FirstState::CBattle_720Spin_FirstState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_720Spin_FirstState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_720Spin_FirstState::Tick(_float fTimeDelta)
{
	Find_Target();

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

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
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			//if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			//{
			//	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			//		dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 0.6f, 0.01f);
			//}

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				if (!m_bAnimFinish)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Attack_Spin.wav"), SOUND_VOICE, 0.6f);
					m_bAnimFinish = true;
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{


				if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
					dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 0.6f, 0.01f);

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

					ColliderDesc.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc.vPosition = _float3(0.f, -8.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);

				}
				else if (nullptr != m_pAtkColliderCom)
					m_pAtkColliderCom->Update(matWorld);


				if (nullptr == m_p2th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc2th;

					ColliderDesc2th.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc2th.vPosition = _float3(0.f, -8.f, 0.f);

					m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
					m_p2th_AtkColliderCom->Update(matWorld_2th);

				}
				else if (nullptr != m_p2th_AtkColliderCom)
					m_p2th_AtkColliderCom->Update(matWorld_2th);


				if (nullptr == m_p3th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc3th;

					ColliderDesc3th.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc3th.vPosition = _float3(0.f, -8.f, 0.f);

					m_p3th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc3th);
					m_p3th_AtkColliderCom->Update(matWorld_3th);

				}
				else if (nullptr != m_p3th_AtkColliderCom)
					m_p3th_AtkColliderCom->Update(matWorld_3th);


				if (nullptr == m_p4th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc4th;

					ColliderDesc4th.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc4th.vPosition = _float3(0.f, -8.f, 0.f);

					m_p4th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc4th);
					m_p4th_AtkColliderCom->Update(matWorld_4th);

				}

				else if (nullptr != m_p4th_AtkColliderCom)
					m_p4th_AtkColliderCom->Update(matWorld_4th);

				if (nullptr == m_p5th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc5th;

					ColliderDesc5th.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc5th.vPosition = _float3(0.f, -8.f, 0.f);

					m_p5th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc5th);
					m_p5th_AtkColliderCom->Update(matWorld_5th);

				}
				else if (nullptr != m_p5th_AtkColliderCom)
					m_p5th_AtkColliderCom->Update(matWorld_5th);


				if (nullptr == m_p6th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc6th;

					ColliderDesc6th.vScale = _float3(30.f, 30.f, 30.f);
					ColliderDesc6th.vPosition = _float3(0.f, -8.f, 0.f);

					m_p6th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BOSS, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc6th);
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

CAstralDoubt_State * CBattle_720Spin_FirstState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;


	if (m_bIsAnimationFinished)
	{
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_SPIN);
	}

	if (nullptr != m_pAtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
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
				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
		}

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p4th_AtkColliderCom, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
		}

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p5th_AtkColliderCom, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
		}

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_p6th_AtkColliderCom, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
		}
	}

	//if (nullptr != m_pAtkColliderCom)
	//{
	//	list<CGameObject*>* pPlayerList = CGameInstance::Get_Instance()->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Player"));

	//	for (auto& iter : *pPlayerList)
	//	{
	//		if (dynamic_cast<CPlayer*>(iter)->Get_Info().fCurrentHp <= 0)
	//			continue;

	//		CBaseObj* pCollisionTarget = nullptr;

	//		pCollisionTarget = dynamic_cast<CBaseObj*>(iter);

	//		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
	//		{
	//			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
	//			if (pCollided)
	//				pCollided->Take_Damage(rand() % 100, m_pOwner, true);
	//		}
	//	}
	//}

	//if (nullptr != m_pAtkColliderCom)
	//{
	//	CBaseObj* pCollisionTarget = nullptr;

	//	if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
	//	{
	//		CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
	//		if (pCollided)
	//			pCollided->Take_Damage(rand() % 100, m_pOwner, true);
	//	}
	//}

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

void CBattle_720Spin_FirstState::Enter()
{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SWING_360);
	
}

void CBattle_720Spin_FirstState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	
	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);
	Safe_Release(m_p3th_AtkColliderCom);
	Safe_Release(m_p4th_AtkColliderCom);
	Safe_Release(m_p5th_AtkColliderCom);
	Safe_Release(m_p6th_AtkColliderCom);
}