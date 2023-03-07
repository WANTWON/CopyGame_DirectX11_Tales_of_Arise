#include "stdafx.h"
#include "..\Public\Monster_LawNormalAttack.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "Alphen.h"
#include "Law.h"
#include "AI_LAW_SkillAttack_State.h"
#include "AI_DodgeState.h"
#include "Monster_LawIdleState.h"
#include "Monster_LawAirR.h"
#include "Monster_LawAirF.h"


using namespace MonsterLaw;

CMonster_LawNormalAttack::CMonster_LawNormalAttack(CMonsterLaw* pPlayer, STATE_ID state, CPlayer* pTarget , _int phase)//, _float fStartHeight, _float fTime)
{
	m_pOwner = pPlayer;
	m_iPhase = m_pOwner->Get_Phase();
	m_eStateId = state;
	
	if (nullptr == pTarget)
	{
		m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());//Find_Target(0/*rand() % 4*/);
		//(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;

	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CMonsterLawState * CMonster_LawNormalAttack::Tick(_float fTimeDelta)
{
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (NORMALATTACK_5 == m_eStateId && !m_pOwner->Get_IsFly())
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation_Jump();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case NORMALATTACK_1:
					if (nullptr == m_pLeftHandCollider)
						m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);

					break;
				case NORMALATTACK_2:
					if (nullptr == m_pRightHandCollider)
						m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);

					break;
				case NORMALATTACK_3:
					if (nullptr == m_pRightFootCollider)
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightFootCollider, m_pOwner);

					break;
				case NORMALATTACK_4:
					if (nullptr == m_pLeftHandCollider)
						m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);

					break;
				case NORMALATTACK_5:
					if (nullptr == m_pLeftFootCollider)
						m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);

					break;
				}

				m_fEventStartTime = pEvent.fStartTime;
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case NORMALATTACK_1:
					m_eStateId = NORMALATTACK_2;
					break;
				case NORMALATTACK_2:
					m_eStateId = NORMALATTACK_3;
					break;
				case NORMALATTACK_3:
					m_eStateId = NORMALATTACK_4;
					break;
				case NORMALATTACK_4:
					m_eStateId = NORMALATTACK_5;
					break;
				case NORMALATTACK_5:
					switch (rand() % 2)
					{
					case 0:
						return new CMonster_LawAirF(m_pOwner);
						break;

					case 1:
						return new CMonster_LawAirR(m_pOwner);
						break;
					}
			
					break;
				}
				Enter();

				return nullptr;
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case NORMALATTACK_1:

					if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
						m_pLeftHandCollider = nullptr;



						m_fEventStartTime = -1.f;
					}
					break;
				case NORMALATTACK_2:

					if (nullptr != m_pRightHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
						m_pRightHandCollider = nullptr;

						m_fEventStartTime = -1.f;
					}
					break;
				case NORMALATTACK_3:

					if (nullptr != m_pRightFootCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightFootCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
						m_pRightFootCollider = nullptr;

						m_fEventStartTime = -1.f;
					}
					break;
				case NORMALATTACK_4:
					if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
						m_pLeftHandCollider = nullptr;

						m_fEventStartTime = -1.f;
					}
					break;
				case NORMALATTACK_5:
					if (nullptr != m_pLeftFootCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
						m_pLeftFootCollider = nullptr;

						m_fEventStartTime = -1.f;
					}
					break;
				}
			}
		}
	}


	_matrix	WorldBoneMatrix = XMMatrixIdentity();

	if (nullptr != m_pLeftHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pRightHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pRightHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pLeftFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftFootCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pRightFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pRightFootCollider->Update(WorldBoneMatrix);
	}

	return nullptr;
}

CMonsterLawState * CMonster_LawNormalAttack::LateTick(_float fTimeDelta)
{

	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftHandCollider);
#endif
	}

	if (nullptr != m_pRightHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pRightHandCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightHandCollider);
#endif
	}

	if (nullptr != m_pLeftFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pLeftFootCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftFootCollider);
#endif
	}

	if (nullptr != m_pRightFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pRightFootCollider, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightFootCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
		//.return new CMonster_LawIdleState(m_pOwner);
		return new CMonster_LawAirR(m_pOwner);
			//return new CAI_LAW_AIRSKILLR(m_pOwner, STATE_SKILL_ATTACK_R);
	}

	return nullptr;
}

void CMonster_LawNormalAttack::Enter()
{
	

	switch (m_eStateId)
	{
	case NORMALATTACK_1:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_0);
		break;
	case NORMALATTACK_2:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_1);
		break;
	case NORMALATTACK_3:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_2);
		break;
	case NORMALATTACK_4:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_3);
		break;
	case NORMALATTACK_5:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_4);
		break;
	}


	//CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	//CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr == m_pTarget)
	{
		m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	

}

void CMonster_LawNormalAttack::Exit()
{
	//__super::Exit();

	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightFootCollider, m_pOwner);
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pRightHandCollider, m_pOwner);
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftHandCollider, m_pOwner);
	CCollision_Manager::Get_Instance()->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pLeftFootCollider, m_pOwner);


	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightHandCollider);
	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pRightFootCollider);
}

CCollider * CMonster_LawNormalAttack::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
{
	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.vScale = vScale;
	ColliderDesc.vRotation = vRotation;
	ColliderDesc.vPosition = vPosition;

	switch (eType)
	{
	case Engine::CCollider::TYPE_AABB:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_OBB:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		return pCollisionMgr->Reuse_Collider(eType, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
		break;
	}

	return nullptr;
}