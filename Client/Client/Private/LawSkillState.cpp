#include "stdafx.h"
#include "..\Public\LawSkillState.h"

#include "Monster.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "EffectMesh.h"
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
	Update_Skill();

	if ((m_eStateId == CPlayerState::STATE_SKILL_ATTACK_F) && (nullptr != m_pTarget))
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if ((m_eStateId == Client::CPlayerState::STATE_SKILL_ATTACK_R) && !m_bIsFly)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());
		else if ((m_eStateId == Client::CPlayerState::STATE_SKILL_ATTACK_F) && !m_bIsFly)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());
		else
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

						if (!m_bSkill_E_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_E_SkillSound.wav"), SOUND_RAW_EFF, 0.85f);
							m_bSkill_E_Sound = true;
						}
					}
					else
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));

						if (!m_bSkill_E_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_E_SkillSound.wav"), SOUND_RAW_EFF, 0.85f);
							m_bSkill_E_Sound = true;
						}
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (nullptr == m_pRightFootCollider)
					{
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}

					if (!m_bSkill_R_Sound)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_R_SkillSound.wav"), SOUND_RAW_EFF, 0.45f);
						m_bSkill_R_Sound = true;
					}
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

						if (!m_bSkill_F_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_F_SkillSound.wav"), SOUND_RAW_EFF, 0.25f);
							m_bSkill_F_Sound = true;
						}
					

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
							if (!strcmp(pEvent.szName, "Burn_Knuckle"))
							{
								if (!m_bBurnKnuckle)
								{
									m_BurnKnuckle = CEffect::PlayEffectAtLocation(TEXT("Burn_Knuckle.dat"), mWorldMatrix);
									m_bBurnKnuckle = true;

									if (m_BurnKnuckle[1])
										m_vBurnKnucklePosition = m_BurnKnuckle[1]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								}
							}
							if (!strcmp(pEvent.szName, "Burn_Knuckle_Particles"))
							{
								if (!m_bBurnKnuckleParticles)
								{
									mWorldMatrix.r[3] = m_vBurnKnucklePosition;

									CEffect::PlayEffectAtLocation(TEXT("Burn_Knuckle_Particles.dat"), mWorldMatrix);
									m_bBurnKnuckleParticles = true;
								}
							}
						}
						else
						{
							if (!strcmp(pEvent.szName, "Rondsenpu"))
							{
								if (!m_bRondsenpu)
								{
									vector<CEffect*> Rondsenpu = CEffect::PlayEffectAtLocation(TEXT("Rondsenpu_1.dat"), mWorldMatrix);

									_matrix EffectWorldMatrix = Rondsenpu.front()->Get_Transform()->Get_WorldMatrix();
									CEffect::PlayEffectAtLocation(TEXT("Rondsenpu_Particles.dat"), EffectWorldMatrix);

									m_bRondsenpu = true;
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
							if (!strcmp(pEvent.szName, "Garyoukuuha"))
							{
								if (!m_bGaryoukuuha)
								{
									CEffect::PlayEffectAtLocation(TEXT("Garyoukuuha_Particles.dat"), mWorldMatrix);
									CEffect::PlayEffectAtLocation(TEXT("Garyoukuuha_Ring.dat"), mWorldMatrix);
									m_Garyoukuuha = CEffect::PlayEffectAtLocation(TEXT("Garyoukuuha.dat"), mWorldMatrix);
									m_bGaryoukuuha = true;
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
							if (!strcmp(pEvent.szName, "Tyourengadan_1"))
							{
								if (!m_bTyourengadan_1)
								{
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_Ring.dat"), mWorldMatrix);
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_1.dat"), mWorldMatrix);
									m_bTyourengadan_1 = true;
								}
							}
							if (!strcmp(pEvent.szName, "Tyourengadan_2"))
							{
								if (!m_bTyourengadan_2)
								{
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_2.dat"), mWorldMatrix);
									m_bTyourengadan_2 = true;
								}
							}
							if (!strcmp(pEvent.szName, "Tyourengadan_3"))
							{
								if (!m_bTyourengadan_3)
								{
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_3.dat"), mWorldMatrix);
									m_bTyourengadan_3 = true;
								}
							}
							if (!strcmp(pEvent.szName, "Tyourengadan_5"))
							{
								if (!m_bTyourengadan_5)
								{
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_Ring.dat"), mWorldMatrix);
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_5.dat"), mWorldMatrix);
									m_bTyourengadan_5 = true;
								}
							}
							if (!strcmp(pEvent.szName, "Tyourengadan_Kick"))
							{
								if (!m_bTyourengadan_Kick)
								{
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_Ring.dat"), mWorldMatrix);
									CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_Kick.dat"), mWorldMatrix);
									m_bTyourengadan_Kick = true;
								}
							}
							if (!strcmp(pEvent.szName, "Tyourengadan_Punch"))
							{
								if (!m_bTyourengadan_Punch)
								{
									
									m_TyourengadanPunch = CEffect::PlayEffectAtLocation(TEXT("Tyourengadan_Punch.dat"), mWorldMatrix);
									m_bTyourengadan_Punch = true;

									if (m_TyourengadanPunch[1])
										m_vPunchPosition = m_TyourengadanPunch[1]->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
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
	Remove_Skill();

	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
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
				pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
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
				pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
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
				pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightFootCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, m_fTime);
		else
		{
			if (CPlayerState::STATE_SKILL_ATTACK_R == m_eStateId || CPlayerState::STATE_SKILL_ATTACK_F == m_eStateId)
				return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, 1.f);
			else
				return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
		}
	}

	return nullptr;
}

CPlayerState * CLawSkillState::EventInput(void)
{
	if (!m_bIsFly)
	{
		if (CPlayerState::STATE_SKILL_ATTACK_R == m_eStateId || CPlayerState::STATE_SKILL_ATTACK_F == m_eStateId)
			return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, 1.f);
	}

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
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_Jump_E_SkillVoice.wav"), SOUND_RAW_VOICE, 0.5f);
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
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_E_SkillVoice.wav"), SOUND_RAW_VOICE, 0.5f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_GARYOUKUUHA);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_R_SkillVoice.wav"), SOUND_RAW_VOICE, 0.5f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_TYOURENGADAN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Law_F_SkillVoice.wav"), SOUND_RAW_VOICE, 0.5f);
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

	//CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CLawSkillState::Update_Skill(void)
{
	for (auto& pEffect : m_Garyoukuuha)
	{
		if (pEffect && !wcscmp(pEffect->Get_PrototypeId(), TEXT("Akizame")))
		{
			_float4 vPlayerPosition;
			XMStoreFloat4(&vPlayerPosition, m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

			_float4 vEffectPosition;
			XMStoreFloat4(&vEffectPosition, pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

			vEffectPosition.y = vPlayerPosition.y + 7.f;

			pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, XMLoadFloat4(&vEffectPosition));
		}
	}
}

void CLawSkillState::Remove_Skill(void)
{
	for (auto& pEffect : m_Garyoukuuha)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}
}

void CLawSkillState::Reset_Skill(void)
{
	m_bRondsenpu = false;
	m_bGaryoukuuha = false;
	m_bTyourengadan_1 = false;
	m_bTyourengadan_2 = false;
	m_bTyourengadan_3 = false;
	m_bTyourengadan_4 = false;
	m_bTyourengadan_5 = false;
	m_bTyourengadan_Kick = false;
	m_bTyourengadan_Punch = false;

	m_bBurnKnuckle = false;
	m_bBurnKnuckleParticles = false;
	m_bEnhaBakusaiken = false;
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
