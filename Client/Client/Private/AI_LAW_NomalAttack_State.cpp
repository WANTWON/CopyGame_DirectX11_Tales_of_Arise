#include "stdafx.h"
#include "..\Public\AI_LAW_NomalAttack_State.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "Law.h"
#include "AICheckState.h"
#include "AI_LAW_SkillAttack_State.h"
#include "AI_DodgeState.h"


using namespace AIPlayer;

AI_LAW_NomalAttack_State::AI_LAW_NomalAttack_State(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;

	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * AI_LAW_NomalAttack_State::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
	}
	else
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	if (m_pTarget == nullptr)
		return nullptr;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
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
				case Client::CAIState::STATE_NORMAL_ATTACK1:

					if (nullptr == m_pLeftHandCollider)
						m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					break;
				case Client::CAIState::STATE_NORMAL_ATTACK2:

					if (nullptr == m_pRightHandCollider)
						m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					break;
				case Client::CAIState::STATE_NORMAL_ATTACK3:
					if (nullptr == m_pRightFootCollider)
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					break;
				case Client::CAIState::STATE_NORMAL_ATTACK4:
					if (nullptr == m_pLeftHandCollider)
						m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					break;
				case Client::CAIState::STATE_NORMAL_ATTACK5:
					if (nullptr == m_pLeftFootCollider)
						m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

					break;


					pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

					m_fEventStartTime = pEvent.fStartTime;
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CAIState::STATE_NORMAL_ATTACK1:
					m_eStateId = STATE_NORMAL_ATTACK2;
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK2:
					m_eStateId = STATE_NORMAL_ATTACK3;
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK3:
					m_eStateId = STATE_NORMAL_ATTACK4;
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK4:
					m_eStateId = STATE_NORMAL_ATTACK5;
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK5:
					//if(m_pTarget != nullptr)
					if (m_pOwner->Get_Info().fCurrentMp < 1.f)
					{
						return new CAI_DodgeState(m_pOwner, m_pTarget);
					}
					else
					{
						switch (rand() % 5)
						{

						case 0:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);

						case 1:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);

						case 2:
							return new CAI_LAW_SkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);

						case 3:
							return new CAI_DodgeState(m_pOwner, m_pTarget);

						}
					}
					
					return nullptr;
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
				case Client::CAIState::STATE_NORMAL_ATTACK1:

					if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
						m_pLeftHandCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

						m_fEventStartTime = -1.f;
					}
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK2:

					if (nullptr != m_pRightHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
						m_pRightHandCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

						m_fEventStartTime = -1.f;
					}
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK3:

					if (nullptr != m_pRightFootCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
						m_pRightFootCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

						m_fEventStartTime = -1.f;
					}
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK4:
					if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
						m_pLeftHandCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

						m_fEventStartTime = -1.f;
					}
					break;
				case Client::CAIState::STATE_NORMAL_ATTACK5:
					if (nullptr != m_pLeftFootCollider && (m_fEventStartTime == pEvent.fStartTime))
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
						m_pLeftFootCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

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

CAIState * AI_LAW_NomalAttack_State::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pRightHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
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

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pRightFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightFootCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
	 return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void AI_LAW_NomalAttack_State::Enter()
{
	//__super::Enter();

	//__super::Enter();

	/*if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_1);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_2);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_3);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_4);
			break;
		}
	}
	else
	{*/
		switch (m_eStateId)
		{
		case Client::CAIState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_0);
			break;
		case Client::CAIState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_1);
			break;
		case Client::CAIState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_2);
			break;
		case Client::CAIState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_3);
			break;
		case Client::CAIState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_4);
			break;
		}
//	}

	//m_fResultTime = m_fTime;

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	//m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);

}

void AI_LAW_NomalAttack_State::Exit()
{
	__super::Exit();

	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightHandCollider);
	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pRightFootCollider);
}

CCollider * AI_LAW_NomalAttack_State::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
{
	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.vScale = vScale;
	ColliderDesc.vRotation = vRotation;
	ColliderDesc.vPosition = vPosition;

	switch (eType)
	{
	case Engine::CCollider::TYPE_AABB:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_AABB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_OBB:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_OBB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
		break;
	}

	return nullptr;
}