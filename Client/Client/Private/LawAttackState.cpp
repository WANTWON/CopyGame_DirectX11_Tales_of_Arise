#include "stdafx.h"
#include "..\Public\LawAttackState.h"

#include "GameInstance.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

#include "LawSkillState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"

#include "Effect.h"
#include "Monster.h"

using namespace Player;

CLawAttackState::CLawAttackState(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CLawAttackState::HandleInput()
{
	return nullptr;
}

CPlayerState * CLawAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
		else
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
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (m_bIsFly)
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else
					{
						if (nullptr == m_pLeftHandCollider)
							m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
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
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
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
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (m_bIsFly)
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else
					{
						if (nullptr == m_pLeftHandCollider)
							m_pLeftHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK5:
					if (m_bIsFly)
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					break;
				}

				pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

				m_fEventStartTime = pEvent.fStartTime;
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (m_bIsFly)
					{
						if (nullptr != m_pRightFootCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
							m_pRightFootCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					else
					{
						if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
							m_pLeftHandCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (m_bIsFly)
					{
						if (nullptr != m_pLeftFootCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
							m_pLeftFootCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					else
					{
						if (nullptr != m_pRightHandCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
							m_pRightHandCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (m_bIsFly)
					{
						if (nullptr != m_pRightHandCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
							m_pRightHandCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					else
					{
						if (nullptr != m_pRightFootCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
							m_pRightFootCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (m_bIsFly)
					{
						if (nullptr != m_pRightFootCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
							m_pRightFootCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					else
					{
						if (nullptr != m_pLeftHandCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftHandCollider);
							m_pLeftHandCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK5:
					if (m_bIsFly)
					{
						if (nullptr != m_pRightHandCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
							m_pRightHandCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
					}
					else
					{
						if (nullptr != m_pLeftFootCollider && (m_fEventStartTime == pEvent.fStartTime))
						{
							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
							m_pLeftFootCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);

							m_fEventStartTime = -1.f;
						}
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

CPlayerState * CLawAttackState::LateTick(_float fTimeDelta)
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
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_START, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CLawAttackState::EventInput(void)
{
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_eStateId = STATE_NORMAL_ATTACK2;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_eStateId = STATE_NORMAL_ATTACK3;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_eStateId = STATE_NORMAL_ATTACK4;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_eStateId = STATE_NORMAL_ATTACK5;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			return new CLawAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);
			break;
		}

		Enter();
	}

	if (m_bIsFly)
	{
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 1)
		{
			if (GetKeyState('E') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fStartHeight, m_fTime);
			else if (GetKeyState('R') < 0)
				return new CLawAirRSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fStartHeight, m_fTime);
			else if (GetKeyState('F') < 0)
				return new CLawAirFSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fStartHeight, m_fTime);
		}

		if (100.f <= m_pOwner->Get_Info().fCurrentBoostGuage)
		{
			if (GetKeyState('4') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_BOOST, m_fStartHeight, m_fTime);
		}
	}
	else
	{
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 1)
		{
			if (GetKeyState('E') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
			else if (GetKeyState('R') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
			else if (GetKeyState('F') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
		}

		if (100.f <= m_pOwner->Get_Info().fCurrentBoostGuage)
		{
			if (GetKeyState('4') < 0)
				return new CLawSkillState(m_pOwner, STATE_SKILL_BOOST);
		}

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

void CLawAttackState::Enter()
{
	__super::Enter();

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_JumpNormal_Attack1.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_JumpNormal_Attack2.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_2);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_JumpNormal_Attack3.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_3);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_JumpNormal_Attack4.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_AIR_4);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Normal_Attack1.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Normal_Attack2.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_2);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Normal_Attack3.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_3);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Normal_Attack4.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_NORMAL_4);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Normal_Attack5.wav"), SOUND_EFFECT_ALPHEN, 1.0f);
			break;
		}
	}

	m_fResultTime = m_fTime;

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	//m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);
}

void CLawAttackState::Exit()
{
	__super::Exit();

	Safe_Release(m_pLeftHandCollider);
	Safe_Release(m_pRightHandCollider);
	Safe_Release(m_pLeftFootCollider);
	Safe_Release(m_pRightFootCollider);

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

CCollider * CLawAttackState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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