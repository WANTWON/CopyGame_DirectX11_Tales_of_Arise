#include "stdafx.h"
#include "..\Public\AlphenSkillState.h"

#include "Monster.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "AlphenAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

using namespace Player;

CAlphenSkillState::CAlphenSkillState(CPlayer * pPlayer, STATE_ID eStateType, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fTime = fTime;
}

CPlayerState * CAlphenSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CAlphenSkillState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.1f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (m_eStateId == CPlayerState::STATE_SKILL_ATTACK_F && !m_bIsFly)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.04f), fRotationRadian, m_pOwner->Get_Navigation());
		else if (m_eStateId == CPlayerState::STATE_SKILL_ATTACK_E && !m_bIsFly)
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.005f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
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
				if (m_bIsFly && (STATE_SKILL_ATTACK_R == m_eStateId))
				{
					if (nullptr == m_pSwordCollider)
					{
						m_pSwordCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(12.5f, 12.5f, 12.5f), _float3(0.f, 0.f, 0.f), _float3(0.f, 2.5f, 0.f));
						m_fColEventStartTime = pEvent.fStartTime;
					}
				}
				else
				{
					if (nullptr == m_pSwordCollider)
					{
						m_pSwordCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.5f, 2.5f, 2.5f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, -3.f));
						m_fColEventStartTime = pEvent.fStartTime;
					}
				}

				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (m_bIsFly)
					{
						if (!m_bSkill_R_Sound)
						{
							//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Jump_R.wav"), SOUND_EFFECT, 0.5f);
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Jump_R_SkillSound.wav"), SOUND_EFFECT, 0.4f);
							m_bSkill_R_Sound = true;
						}
					}
					else
						if (!m_bSkill_R_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_R_SkillSound.wav"), SOUND_EFFECT, 0.4f);
							m_bSkill_R_Sound = true;
						}
			
						break;

				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (m_bIsFly)
					{
						if (!m_bSkill_E_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_Jump_E_SkillSound.wav"), SOUND_EFFECT, 0.5f);
							m_bSkill_E_Sound = true;
						}
					}

					else
						if (!m_bSkill_E_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_E_SkillSound.wav"), SOUND_EFFECT, 0.5f);
							m_bSkill_E_Sound = true;
						}
						break;

				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (m_bIsFly)
						if (!m_bSkill_JumpF_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Jump_F.wav"), SOUND_EFFECT, 0.7f);
							m_bSkill_JumpF_Sound = true;
						}

					else
						if (!m_bSkill_F_Sound)
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_F.wav"), SOUND_EFFECT, 0.3f);
							m_bSkill_F_Sound = true;
						}
						break;

				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				CPlayerState* pState = EventInput();
				if (nullptr != pState)
					return pState;
			}
		
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK_E:
					if (m_bIsFly)
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

						if (!strcmp(pEvent.szName, "Ryuuseizin_1"))
						{
							if (!m_bRyuuseizinFirstEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Ryuuseizin_1.dat"), mWorldMatrix);

								m_bRyuuseizinFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Ryuuseizin_2"))
						{
							if (!m_bRyuuseizinSecondEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Ryuuseizin_Ring.dat"), mWorldMatrix);
								
								vector<CEffect*> Ryuuseizin = CEffect::PlayEffectAtLocation(TEXT("Ryuuseizin_2.dat"), mWorldMatrix);
								_matrix EffectWorldMatrix = Ryuuseizin.front()->Get_Transform()->Get_WorldMatrix();

								vector<CEffect*> RyuuseizinParticles = CEffect::PlayEffectAtLocation(TEXT("Ryuuseizin_Particles.dat"), EffectWorldMatrix);

								m_bRyuuseizinSecondEffect = true;
							}
						}
					}
					else
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
					
						if (!strcmp(pEvent.szName, "Hienzin_1"))
						{
							if (!m_bHienzinFirstEffect)
							{							
								vector<CEffect*> Hienzin_1 = CEffect::PlayEffectAtLocation(TEXT("Hienzin_1.dat"), mWorldMatrix);
								for (auto& pEffect : Hienzin_1)
								{
									if (pEffect && !wcscmp(pEffect->Get_PrototypeId(), TEXT("CrossBuffer1")))
									{
										_matrix ParticleWorldMatrix = pEffect->Get_Transform()->Get_WorldMatrix();
										CEffect::PlayEffectAtLocation(TEXT("Hienzin_Particles.dat"), ParticleWorldMatrix);
									}
								}

								CEffect::PlayEffectAtLocation(TEXT("Hienzin_Ring.dat"), mWorldMatrix);
								m_bHienzinFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Hienzin_2"))
						{
							if (!m_bHienzinSecondEffect)
							{
								vector<CEffect*> Hienzin_2 = CEffect::PlayEffectAtLocation(TEXT("Hienzin_2.dat"), mWorldMatrix);
								for (auto& pEffect : Hienzin_2)
								{
									if (pEffect && !wcscmp(pEffect->Get_PrototypeId(), TEXT("CrossBuffer1")))
									{
										_matrix ParticleWorldMatrix = pEffect->Get_Transform()->Get_WorldMatrix();
										CEffect::PlayEffectAtLocation(TEXT("Hienzin_Particles.dat"), ParticleWorldMatrix);
									}
								}

								m_bHienzinSecondEffect = true;
							}
						}
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (m_bIsFly)
					{
						if (!strcmp(pEvent.szName, "Senkusyourepa_1"))
						{
							if (!m_bSenkusyourepaFirstEffect)
							{
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_1.dat"), mWorldMatrix);
								CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_Wind.dat"), mWorldMatrix);

								m_bSenkusyourepaFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Senkusyourepa_2"))
						{
							if (!m_bSenkusyourepaSecondEffect)
							{
								/* Spawn Particles Effect */
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

								_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vOffset = XMVectorSet(0.f, 1.5f, 0.f, 0.f);

								vPosition += vOffset;
								mWorldMatrix.r[3] = vPosition;

								CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_Explosion_Particles.dat"), mWorldMatrix);

								/* Then Spawn Effect */
								mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_2.dat"), mWorldMatrix);

								m_bSenkusyourepaSecondEffect = true;
							}
						}
					}
					else
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

						if (!strcmp(pEvent.szName, "Akizame_1"))
						{
							if (!m_bAkizameFirstEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Akizame_1.dat"), mWorldMatrix);

								m_bAkizameFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Akizame_2"))
						{
							if (!m_bAkizameSecondEffect)
							{
								vector<CEffect*> Akizame_2 = CEffect::PlayEffectAtLocation(TEXT("Akizame_2.dat"), mWorldMatrix);
								for (auto& pEffect : Akizame_2)
								{
									if (pEffect && !wcscmp(pEffect->Get_PrototypeId(), TEXT("Cone01")))
									{
										_vector vPosition = pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

										vector<CEffect*> Akizame_Particles = CEffect::PlayEffectAtLocation(TEXT("Akizame_Particles.dat"), mWorldMatrix);
										Akizame_Particles.front()->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
									}
								}

								m_bAkizameSecondEffect = true;
							}
						}
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_F:
					if (m_bIsFly)
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

						if (!strcmp(pEvent.szName, "Engetu_1"))
						{
							if (!m_bEngetuFirstEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Engetu_Ring.dat"), mWorldMatrix);
								vector<CEffect*> Engetu = CEffect::PlayEffectAtLocation(TEXT("Engetu.dat"), mWorldMatrix);
								
								_vector vPosition = Engetu.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

								vector<CEffect*> EngetuParticles = CEffect::PlayEffectAtLocation(TEXT("Engetu_Particles.dat"), mWorldMatrix);
								for (auto& pEffect : EngetuParticles)
								{
									if (pEffect)
										pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}
								
								m_bEngetuFirstEffect = true;
							}
						}

						if (!strcmp(pEvent.szName, "Engetu_2"))
						{
							if (!m_bEngetuSecondEffect)
							{
								vector<CEffect*> Engetu = CEffect::PlayEffectAtLocation(TEXT("Engetu.dat"), mWorldMatrix);

								_vector vPosition = Engetu.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

								vector<CEffect*> EngetuParticles = CEffect::PlayEffectAtLocation(TEXT("Engetu_Particles.dat"), mWorldMatrix);
								for (auto& pEffect : EngetuParticles)
								{
									if (pEffect)
										pEffect->Get_Transform()->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
								}

								m_bEngetuSecondEffect = true;
							}
						}
					}
					else
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

						if (!strcmp(pEvent.szName, "Housyutigakuzin_1"))
						{
							if (!m_bHousyutigakuzinFirstEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_1.dat"), mWorldMatrix);

								m_bHousyutigakuzinFirstEffect = true;
							}
						}

						if (!strcmp(pEvent.szName, "Housyutigakuzin_2"))
						{
							if (!m_bHousyutigakuzinSecondEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_2.dat"), mWorldMatrix);

								m_bHousyutigakuzinSecondEffect = true;
							}
						}

						if (!strcmp(pEvent.szName, "Housyutigakuzin_3"))
						{
							if (!m_bHousyutigakuzinThirdEffect)
							{
								vector<CEffect*> Housyutigakuzin = CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_3.dat"), mWorldMatrix);
								for (auto& pEffect : Housyutigakuzin)
								{
									if (pEffect && !wcscmp(pEffect->Get_PrototypeId(), TEXT("Plane")))
									{
										_vector vPosition = pEffect->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

										_matrix ParticleWorldMatrix = XMMatrixIdentity();
										ParticleWorldMatrix.r[3] = vPosition;
										CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_Particles.dat"), ParticleWorldMatrix);
									}
								}

								CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_4.dat"), mWorldMatrix);

								m_bHousyutigakuzinThirdEffect = true;
							}
						}
					}
					break;
				}
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				if ((nullptr != m_pSwordCollider) && (m_fColEventStartTime == pEvent.fStartTime))
				{
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pSwordCollider);
					m_pSwordCollider = nullptr;

					m_fColEventStartTime = -1.f;

					if (STATE_SKILL_ATTACK_F == m_eStateId && !m_bIsFly)
					{
						if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
						{
							CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
							pCamera->Set_Zoom(false);
						}
					}
					
				}
			}
		}
	}

	_matrix WorldBoneMatrix = XMMatrixIdentity();

	if (nullptr != m_pSwordCollider)
	{
		if ((STATE_SKILL_ATTACK_R == m_eStateId) && m_bIsFly)
			WorldBoneMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		else
		{
			WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("pinky_03_R")->Get_CombinedTransformationMatrix() *
				XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
		}
		
		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pSwordCollider->Update(WorldBoneMatrix);
	}

	return nullptr;
}

CPlayerState * CAlphenSkillState::LateTick(_float fTimeDelta)
{	if (nullptr != m_pSwordCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pSwordCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
			{
				

				if (!m_bIsFly)
				{
					
					if (STATE_SKILL_ATTACK_E == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.05f;
						m_HitLagDesc.bHitLag = true;
						m_HitLagDesc.fHitLagTimer = 0.15f;
						m_HitLagDesc.bCritical = true;
						m_HitLagDesc.bShaking = true;
						m_HitLagDesc.bZoom = true;
						m_HitLagDesc.fShakingPower = 0.2f;
						m_HitLagDesc.fShakingMinusPower = 0.2f;
						m_HitLagDesc.fZoomDistance = 2.5f;
						m_HitLagDesc.fZoomSpeed = 0.5f;
						m_HitLagDesc.fBlurPower = 5.f;
						m_HitLagDesc.fBlurDetail = 7.f;
					}
					else if (STATE_SKILL_ATTACK_R == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.05f;
						m_HitLagDesc.bHitLag = true;
						m_HitLagDesc.fHitLagTimer = 0.02f;
						m_HitLagDesc.bCritical = true;
						m_HitLagDesc.bShaking = false;
						m_HitLagDesc.fZoomDistance = 5.f;
						m_HitLagDesc.fZoomSpeed = 2.5f;
						m_HitLagDesc.bZoom = true;
						m_HitLagDesc.fBlurPower = 6.f;
						m_HitLagDesc.fBlurDetail = 10.f;

					}
					else if (STATE_SKILL_ATTACK_F == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.1f;
						m_HitLagDesc.bHitLag = true;
						m_HitLagDesc.fHitLagTimer = 0.05f;
						m_HitLagDesc.bCritical = true;
						m_HitLagDesc.bShaking = true;
						m_HitLagDesc.fShakingPower = 1.f;
						m_HitLagDesc.fShakingMinusPower = 0.2f;
						m_HitLagDesc.fZoomDistance = 8.f;
						m_HitLagDesc.fZoomSpeed = 2.f;
						m_HitLagDesc.fBlurPower = 6.f;
						m_HitLagDesc.fBlurDetail = 10.f;
						m_HitLagDesc.bZoom = true;

					}
						

					pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
				}
				else
				{
					if (STATE_SKILL_ATTACK_E == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.5f;
						m_HitLagDesc.bHitLag = true;
						m_HitLagDesc.fHitLagTimer = 0.2f;

						m_HitLagDesc.bShaking = true;
						m_HitLagDesc.fShakingPower = 0.6f;
						m_HitLagDesc.fShakingMinusPower = 0.1f;

						m_HitLagDesc.fZoomDistance = -2.5f;
						m_HitLagDesc.fZoomSpeed = 0.5f;
						m_HitLagDesc.bZoom = true;
						m_HitLagDesc.fBlurPower = 3.f;
						m_HitLagDesc.fBlurDetail = 7.f;
						
					}
					else if (STATE_SKILL_ATTACK_R == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.1f;
						m_HitLagDesc.bHitLag = false;
						m_HitLagDesc.bShaking = false;
						m_HitLagDesc.fZoomDistance = -20.f;
						m_HitLagDesc.fZoomSpeed = 5.f;
						m_HitLagDesc.fBlurPower = 6.f;
						m_HitLagDesc.fBlurDetail = 10.f;
						m_HitLagDesc.bZoom = true;
					}
					else if (STATE_SKILL_ATTACK_F == m_eStateId)
					{
						m_HitLagDesc.fTakeDamageTimer = 0.3f;
						m_HitLagDesc.bHitLag = true;
						m_HitLagDesc.fHitLagTimer = 0.2f;
						m_HitLagDesc.bCritical = true;
						m_HitLagDesc.bShaking = true;
						m_HitLagDesc.fShakingPower = 1.4f;
						m_HitLagDesc.fShakingMinusPower = 0.2f;

						m_HitLagDesc.fZoomDistance = -20.f;
						m_HitLagDesc.fZoomSpeed = 5.f;
						m_HitLagDesc.bZoom = true;
						m_HitLagDesc.fBlurPower = 6.f;
						m_HitLagDesc.fBlurDetail = 10.f;
					}
					pCollided->Take_Damage(rand() % 100, m_pOwner, m_HitLagDesc);
				}
					
			}
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pSwordCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CAlphenSkillState::EventInput(void)
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
			m_pOwner->Get_Model()->Reset();
			m_eStateId = STATE_SKILL_ATTACK_R;
			Enter();
		}
		else if (GetKeyState('F') < 0)
		{
			m_pOwner->Get_Model()->Reset();
			m_eStateId = STATE_SKILL_ATTACK_F;
			Enter();
		}
	}

	if (m_bIsFly)
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CAlphenAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1, m_fTime);
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CAlphenAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

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

void CAlphenSkillState::Enter(void)
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
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_YOUSEONGJIN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound+Voice_Jump_E.wav"), SOUND_EFFECT, 1.0f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Jump_E.wav"), SOUND_EFFECT_ALPHEN, 0.3f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_Jump_E.wav"), SOUND_EFFECT_ALPHEN, 0.25f);
			
			//CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("AlphenAirE.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA);

			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_SUMGONGSANGEULPA);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Jump_R.wav"), SOUND_EFFECT, 0.3f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_Jump_R.wav"), SOUND_EFFECT, 0.4f);
		//	CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("AlphenAirR.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_ENGETU);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_ONEWALL);
			/*CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Jump_F.wav"), SOUND_EFFECT, 0.5f);*/
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_Jump_F.wav"), SOUND_EFFECT, 0.5f);

			break;
		case Client::CPlayerState::STATE_SKILL_BOOST:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_STRIKE_AIR);

			
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HIENZIN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BEEYEONIN);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound+Voice_E.wav"), SOUND_EFFECT, 1.0f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Alphen_E_SkillVoice.wav"), SOUND_EFFECT, 0.5f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_E.wav"), SOUND_EFFECT, 0.2f);*/
			
			
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_AKIZAME);

			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOOSAWOO);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound+Voice_R.wav"), SOUND_EFFECT, 1.0f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_R_Test2.wav"), SOUND_EFFECT, 0.3f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_R.wav"), SOUND_EFFECT, 0.4f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);

			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BOONGSUPGEEAKJIN);

			/*CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_F.wav"), SOUND_EFFECT, 0.3f);*/
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_F.wav"), SOUND_EFFECT, 0.3f);
			break;
		case Client::CPlayerState::STATE_SKILL_BOOST:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_STRIKE);
			break;
		}
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CAlphenSkillState::Exit(void)
{
	__super::Exit();

	Safe_Release(m_pSwordCollider);

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);

	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(false);
	}
	

}

void CAlphenSkillState::Reset_Skill()
{
	/* E */
	m_bHienzinFirstEffect = false;
	m_bHienzinSecondEffect = false;
	m_bRyuuseizinFirstEffect = false;
	m_bRyuuseizinSecondEffect = false;

	/* R */
	m_bAkizameFirstEffect = false;
	m_bAkizameSecondEffect = false;
	m_bSenkusyourepaParticle = false;
	m_bSenkusyourepaFirstEffect = false;
	m_bSenkusyourepaSecondEffect = false;

	/* F */
	m_bHousyutigakuzinFirstEffect = false;
	m_bHousyutigakuzinSecondEffect = false;
	m_bHousyutigakuzinThirdEffect = false;
	m_bEngetuFirstEffect = false;
	m_bEngetuSecondEffect = false;
}

CCollider * CAlphenSkillState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
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
