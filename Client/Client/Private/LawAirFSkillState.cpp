#include "stdafx.h"
#include "..\Public\LawAirFSkillState.h"

#include "Monster.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

#include "LawAttackState.h"
#include "LawSkillState.h"

using namespace Player;

CLawAirFSkillState::CLawAirFSkillState(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CLawAirFSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CLawAirFSkillState::Tick(_float fTimeDelta)
{
	if (STATETYPE_MAIN == m_eStateType)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 0.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (STATETYPE_MAIN == m_eStateType)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.6f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	if (STATETYPE_MAIN != m_eStateType)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr == m_pLandCollider)
					{
						m_pLandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(10.f, 10.f, 10.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
						pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

						m_ColliderMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
							XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

						m_ColliderMatrix.r[0] = XMVector4Normalize(m_ColliderMatrix.r[0]);
						m_ColliderMatrix.r[1] = XMVector4Normalize(m_ColliderMatrix.r[1]);
						m_ColliderMatrix.r[2] = XMVector4Normalize(m_ColliderMatrix.r[2]);
					}
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					return EventInput();
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if (nullptr != m_pLandCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLandCollider);
						m_pLandCollider = nullptr;

						pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);
					}
				}
			}
		}

		if (nullptr != m_pLandCollider)
			m_pLandCollider->Update(m_ColliderMatrix);
	}
	
	return nullptr;
}

CPlayerState * CLawAirFSkillState::LateTick(_float fTimeDelta)
{
	if ((STATETYPE_END == m_eStateType) && (nullptr != m_pLandCollider))
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLandCollider);
#endif
	}

	if (STATETYPE_MAIN == m_eStateType)
	{
		if (m_pOwner->Check_Navigation_Jump())
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_END);
			m_eStateType = STATETYPE_END;
		}
	}

	if (m_bIsAnimationFinished)
	{
		switch (m_eStateType)
		{
		case Client::STATETYPE_START:
			if (!m_pOwner->Check_Navigation_Jump())
			{
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_LOOP);
				m_eStateType = STATETYPE_MAIN;
			}
			else
			{
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_END);
				m_eStateType = STATETYPE_END;
			}
			break;
		case Client::STATETYPE_END:
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
			break;
		}
	}

	return nullptr;
}

CPlayerState * CLawAirFSkillState::EventInput(void)
{
	if (GetKeyState(VK_LBUTTON) < 0)
		return new CLawAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if (floor(m_pOwner->Get_Info().fCurrentMp) > 1)
	{
		if (GetKeyState('E') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
		else if (GetKeyState('R') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
		else if (GetKeyState('F') < 0)
			return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
	}

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_BACKWARD_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S))
		return new CRunState(m_pOwner, DIR_BACKWARD);
	else if (pGameInstance->Key_Pressing(DIK_W))
		return new CRunState(m_pOwner, DIR_STRAIGHT);

	return nullptr;
}

void CLawAirFSkillState::Enter(void)
{
	__super::Enter();

	m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);

	m_eStateType = STATETYPE_START;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_START);

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_Jump_F.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
}

void CLawAirFSkillState::Exit(void)
{
	__super::Exit();

	if (m_bIsFly)
		m_pOwner->Off_IsFly();

	m_pOwner->Get_Model()->Reset_Anim(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_START);
	m_pOwner->Get_Model()->Reset_Anim(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_LOOP);
	m_pOwner->Get_Model()->Reset_Anim(CLaw::ANIM::BTL_ATTACK_ENHABAKUSAIKEN_END);
}

CCollider * CLawAirFSkillState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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
