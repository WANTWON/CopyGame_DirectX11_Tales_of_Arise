#include "stdafx.h"
#include "..\Public\LawSkillState.h"

#include "Monster.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "LawAttackState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

using namespace Player;

CLawSkillState::CLawSkillState(CPlayer * pPlayer, STATE_ID eStaetType, _float fTime)
{
	m_eStateId = eStaetType;
	m_pOwner = pPlayer;
	
	m_fTime = fTime;
}

CPlayerState * CLawSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CLawSkillState::Tick(_float fTimeDelta)
{
	if ((m_eStateId == CPlayerState::STATE_SKILL_ATTACK_F) && (nullptr != m_pTarget))
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation_Jump();

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
				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (m_bIsFly)
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (nullptr == m_pRightFootCollider)
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (!strcmp(pEvent.szName, "RH"))
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "LF"))
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "RF"))
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					m_fEventStartTime = pEvent.fStartTime;
					break;
				case Client::CPlayerState::STATE_SKILL_BOOST:
					if (m_bIsFly)
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					break;
				}

				pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				switch (m_eStateId)
				{
					case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					{
						if (m_bIsFly)
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									// 
								}
							}
						}
						else
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									//
								}
							}
						}
						break;
					}
					case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					{
						if (m_bIsFly)
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									// 
								}
							}
						}
						else
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									//
								}
							}
						}
						break;
					}
					case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					{
						if (m_bIsFly)
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									// 
								}
							}
						}
						else
						{
							if (!strcmp(pEvent.szName, ""))
							{
								if (!true)
								{
									CEffect::PlayEffectAtLocation(TEXT(".dat"), mWorldMatrix);
									//
								}
							}
						}
						break;
					}
				}
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (m_bIsFly)
					{
						if (nullptr != m_pRightHandCollider)
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
							m_pRightHandCollider = nullptr;
						}
					}
					else
					{
						if (nullptr != m_pRightFootCollider)
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
							m_pRightFootCollider = nullptr;
						}
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (nullptr != m_pRightFootCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
						m_pRightFootCollider = nullptr;
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (pEvent.fStartTime == m_fEventStartTime)
					{
						if (!strcmp(pEvent.szName, "RH"))
						{
							if (nullptr != m_pRightHandCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
								m_pRightHandCollider = nullptr;
							}
						}
						else if (!strcmp(pEvent.szName, "LF"))
						{
							if (nullptr != m_pLeftFootCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
								m_pLeftFootCollider = nullptr;
							}
						}
						else if (!strcmp(pEvent.szName, "RF"))
						{
							if (nullptr != m_pRightFootCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
								m_pRightFootCollider = nullptr;
							}
						}
						m_fEventStartTime = -1.f;
					}
					break;
				case Client::CPlayerState::STATE_SKILL_BOOST:
					if (m_bIsFly)
					{
						if (nullptr != m_pLeftFootCollider)
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
							m_pLeftFootCollider = nullptr;
						}
					}
					else
					{
						if (nullptr != m_pRightHandCollider)
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
							m_pRightHandCollider = nullptr;
						}
					}
					break;
				}

				pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);
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

CPlayerState * CLawSkillState::LateTick(_float fTimeDelta)
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
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CLawSkillState::EventInput(void)
{
	if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
	{
		if (GetKeyState('E') < 0)
		{
			m_pOwner->Get_Model()->Reset();
			m_eStateId = STATE_SKILL_ATTACK_E;
			Enter();
		}
		else if (GetKeyState('R') < 0)
		{
			if (m_bIsFly)
				return new CLawAirRSkillState(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_R);
			else
			{
				m_pOwner->Get_Model()->Reset();
				m_eStateId = STATE_SKILL_ATTACK_R;
				Enter();
			}
		}
		else if (GetKeyState('F') < 0)
		{
			if (m_bIsFly)
				return new CLawAirFSkillState(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_F);
			else
			{
				m_pOwner->Get_Model()->Reset();
				m_eStateId = STATE_SKILL_ATTACK_F;
				Enter();
			}
		}
	}

	if (m_bIsFly)
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CLawAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1, m_fTime);
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CLawAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_BACKWARD_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S))
			return new CRunState(m_pOwner, DIR_BACKWARD, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_W))
			return new CRunState(m_pOwner, DIR_STRAIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	}

	return nullptr;
}

void CLawSkillState::Enter(void)
{
	__super::Enter();

	if (CPlayerState::STATE_SKILL_BOOST != m_eStateId)
	{
		m_pOwner->Use_Mana(1.f);
		m_pOwner->Set_Manarecover(false);
	}
	
	Reset_Skill();

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_BURN_KNUCKLE);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_Jump_E.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_SKILL_BOOST:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_STRIKE_AIR);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_RONDSENPU);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_E.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_GARYOUKUUHA);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_R.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_TYOURENGADAN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSkillSound_F.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_SKILL_BOOST:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_STRIKE);
			break;
		}
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	m_pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != m_pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CLawSkillState::Exit(void)
{
	__super::Exit();

	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightFootCollider);
	Safe_Release(m_pRightHandCollider);

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CLawSkillState::Reset_Skill(void)
{

}

CCollider * CLawSkillState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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
