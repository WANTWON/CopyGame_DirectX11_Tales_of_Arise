#include "stdafx.h"
#include "..\Public\AlphenAttackState.h"

#include "Monster.h"
#include "ThrowingObject.h"

#include "GameInstance.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"
#include "AlphenSkillState.h"
#include "Effect.h"

#include "Level_Restaurant.h"

using namespace Player;

CAlphenAttackState::CAlphenAttackState(CPlayer * pPlayer, STATE_ID eStateType, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fTime = fTime;
}

CPlayerState * CAlphenAttackState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAlphenAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.1f);

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
				if (!strcmp(pEvent.szName, "Foot"))
				{
					if (nullptr == m_pFootCollider)
						m_pFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.5f, 2.5f, 2.5f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
				}
				else
				{
					if (nullptr == m_pSwordCollider)
						m_pSwordCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.5f, 2.5f, 2.5f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -3.f));
				}

				m_fColEventStartTime = pEvent.fStartTime;
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				_tchar wcEffectName[MAX_PATH] = TEXT("");

				if (m_bIsFly)
				{
					switch (m_eStateId)
					{
					case Client::CPlayerState::STATE_NORMAL_ATTACK1:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_1.dat"));
						break;
					case Client::CPlayerState::STATE_NORMAL_ATTACK2:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_2.dat"));
						break;
					case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					{
						if (!strcmp(pEvent.szName, "Jump_Normal_Attack_3"))
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_3.dat"));
						if (!strcmp(pEvent.szName, "Jump_Normal_Attack_4"))
						{
							if (!m_bEffectKickSpawned)
							{
								CEffect::PlayEffectAtLocation(TEXT("Jump_Normal_Attack_4.dat"), mWorldMatrix);
								m_bEffectKickSpawned = true;
							}
						}
						break;
					}
					case Client::CPlayerState::STATE_NORMAL_ATTACK4:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_5.dat"));
						break;
					}

					if (!m_bEffectSlashSpawned && !m_bEffectKickSpawned)
					{
						CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);
						m_bEffectSlashSpawned = true;
					}
				}
				else
				{
					switch (m_eStateId)
					{
					case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					case Client::CPlayerState::STATE_NORMAL_ATTACK3:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_1.dat"));
						break;
					case Client::CPlayerState::STATE_NORMAL_ATTACK2:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_2.dat"));
						break;
					case Client::CPlayerState::STATE_NORMAL_ATTACK4:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_3.dat"));
						break;
					}

					if (!m_bEffectSlashSpawned)
					{
						if (!wcscmp(wcEffectName, TEXT("Normal_Attack_2.dat")))
							m_SlashEffect = CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);
						else
							CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);

						m_bEffectSlashSpawned = true;
					}
				}
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				if (m_fColEventStartTime == pEvent.fStartTime)
				{
					if (nullptr != m_pSwordCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pSwordCollider);
						m_pSwordCollider = nullptr;
					}
					else if (nullptr != m_pFootCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pFootCollider);
						m_pFootCollider = nullptr;
					}

					m_fColEventStartTime = -1.f;
				}
			}
		}
	}

	_matrix WorldBoneMatrix = XMMatrixIdentity();

	if (nullptr != m_pSwordCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("pinky_03_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pSwordCollider->Update(WorldBoneMatrix);
	}

	if (nullptr != m_pFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pFootCollider->Update(WorldBoneMatrix);
	}

	return nullptr;
}

CPlayerState * CAlphenAttackState::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pSwordCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSwordCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MINIGAME1, m_pSwordCollider, &pCollisionTarget))
		{

			CThrowingObject* pCollied = dynamic_cast<CThrowingObject*>(pCollisionTarget);
			if (pCollied)
			{
				CGameInstance* pGameInstance = CGameInstance::Get_Instance();

				_int m_iScore = 0;

				switch (dynamic_cast<CThrowingObject*>(pCollied)->Get_Type())
				{
				case CThrowingObject::TYPE_POTATO:
					m_iScore = 5;
					break;
				case CThrowingObject::TYPE_APPLE:
					m_iScore = 10;
					break;
				case CThrowingObject::TYPE_REDONION:
					m_iScore = 15;
					break;
				case CThrowingObject::TYPE_BREAD:
					m_iScore = 20;
					break;
				case CThrowingObject::TYPE_MANGO:
					m_iScore = 25;
					break;
				case CThrowingObject::TYPE_PINEAPPLE:
					m_iScore = -10;
					break;
				}

				dynamic_cast<CLevel_Restaurant*>(pGameInstance->Get_CurrentLevel())->Increase_Score(m_iScore);

				pCollied->Set_Dead(true);


				_bool bSound = false;
				if (!bSound)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("FruitCut.wav"), SOUND_EFFECT, 1.0f);
					//bSound = true;

				}
			}

#ifdef _DEBUG
			m_pOwner->Get_Renderer()->Add_Debug(m_pSwordCollider);
#endif
		}
	}
	if (nullptr != m_pFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}
#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pFootCollider);
#endif
	}

	for (auto& pEffect : m_SlashEffect)
	{
		if (pEffect)
		{
			if (pEffect->Get_PreDead())
				pEffect = nullptr;
			else
			{
				_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + XMVectorSet(0.f, 2.5f, 0.f, 0.f);
				pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
			}
		}
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

CPlayerState * CAlphenAttackState::EventInput(void)
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
			return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
			break;
		}

		Enter();
	}

	if (m_bIsFly)
	{
		if (GetKeyState('E') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
		}
		else if (GetKeyState('R') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
		}
		else if (GetKeyState('F') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
		}
	}
	else
	{
		if (GetKeyState('E') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
		}
		else if (GetKeyState('R') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
		}
		else if (GetKeyState('F') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
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

void CAlphenAttackState::Enter()
{
	__super::Enter();

	m_bEffectSlashSpawned = false;
	m_bEffectKickSpawned = false;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	_uint iCurLevelIndex = pGameInstance->Get_CurrentLevelIndex();




	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack1_Fruit.wav"), SOUND_EFFECT, 0.5f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack2_Fruit.wav"), SOUND_EFFECT, 0.5f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_2);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack3_Fruit.wav"), SOUND_EFFECT, 0.5f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_3);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack4_Fruit.wav"), SOUND_EFFECT, 0.5f);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			if (iCurLevelIndex == LEVEL_RESTAURANT)
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack1_Fruit.wav"), SOUND_EFFECT, 0.5f);
			else
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack1.wav"), SOUND_EFFECT, 0.7f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);
			if (iCurLevelIndex == LEVEL_RESTAURANT)
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack2_Fruit.wav"), SOUND_EFFECT, 0.5f);
			else
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack2.wav"), SOUND_EFFECT, 0.7f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_8);
			if (iCurLevelIndex == LEVEL_RESTAURANT)
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack3_Fruit.wav"), SOUND_EFFECT, 0.5f);
			else
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack3.wav"), SOUND_EFFECT, 0.7f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_6);
			if (iCurLevelIndex == LEVEL_RESTAURANT)
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Normal_Attack4_Fruit.wav"), SOUND_EFFECT, 0.5f);
			else
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack4.wav"), SOUND_EFFECT, 0.7f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		}
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	//m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);
}

void CAlphenAttackState::Exit()
{
	__super::Exit();

	Safe_Release(m_pSwordCollider);

	//CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

CCollider * CAlphenAttackState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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
