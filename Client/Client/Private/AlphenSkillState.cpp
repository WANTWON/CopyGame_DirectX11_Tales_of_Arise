#include "stdafx.h"
#include "..\Public\AlphenSkillState.h"

#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "AlphenAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"

using namespace Player;

CAlphenSkillState::CAlphenSkillState(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CAlphenSkillState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CAlphenSkillState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
	}
		
	m_pOwner->Check_Navigation_Jump();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
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
								CEffect::PlayEffectAtLocation(TEXT("Ryuuseizin_2.dat"), mWorldMatrix);

								m_bRyuuseizinSecondEffect = true;
							}
						}
					}
					else
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
						_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);

						_vector vOffset = XMVectorSet(0.f, 1.5f, 0.f, 0.f);
						vPosition += vLook * 2;
						vPosition += vOffset;

						mWorldMatrix.r[3] = vPosition;

						if (!strcmp(pEvent.szName, "Hienzin_1"))
						{
							if (!m_bHienzinFirstEffect)
							{							
								CEffect::PlayEffectAtLocation(TEXT("Hienzin.dat"), mWorldMatrix);

								m_bHienzinFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Hienzin_2"))
						{
							if (!m_bHienzinSecondEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Hienzin.dat"), mWorldMatrix);

								m_bHienzinSecondEffect = true;
							}
						}
					}
					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK_R:
					if (m_bIsFly)
					{
						if (!strcmp(pEvent.szName, "Senkusyourepa_Particles"))
						{
							if (!m_bSenkusyourepaParticle)
							{
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

								_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vOffset = XMVectorSet(0.f, 1.5f, 0.f, 0.f);

								vPosition += vOffset;
								mWorldMatrix.r[3] = vPosition;

								m_SenkusyourepaParticles = CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_Particles.dat"), mWorldMatrix);

								m_bSenkusyourepaParticle = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Senkusyourepa_1"))
						{
							if (!m_bSenkusyourepaFirstEffect)
							{
								/* Destroy Particles first. */
								if (!m_SenkusyourepaParticles.empty())
								{
									for (auto& iter : m_SenkusyourepaParticles)
									{
										if (iter)
										{
											CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
											if (pParticleSystem != nullptr)
												pParticleSystem->Set_Stop(true);
										}
									}
								}

								/* Then Spawn Effect*/
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								m_SenkusyourepaParticles = CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_1.dat"), mWorldMatrix);

								m_bSenkusyourepaFirstEffect = true;
							}
						}
						else if (!strcmp(pEvent.szName, "Senkusyourepa_2"))
						{
							if (!m_bSenkusyourepaSecondEffect)
							{
								/* Spawn Effect Particles */
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

								_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								_vector vOffset = XMVectorSet(0.f, 1.5f, 0.f, 0.f);

								vPosition += vOffset;
								mWorldMatrix.r[3] = vPosition;

								m_SenkusyourepaParticles = CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_Explosion_Particles.dat"), mWorldMatrix);

								/* Then Spawn Effect*/
								mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								m_SenkusyourepaParticles = CEffect::PlayEffectAtLocation(TEXT("Senkusyourepa_2.dat"), mWorldMatrix);

								m_bSenkusyourepaSecondEffect = true;
							}
						}
					}
					else
					{
						if (!m_bAkizameEffect)
						{
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							CEffect::PlayEffectAtLocation(TEXT("Akizame.dat"), mWorldMatrix);

							m_bAkizameEffect = true;
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
								CEffect::PlayEffectAtLocation(TEXT("Engetu.dat"), mWorldMatrix);
								
								m_bEngetuFirstEffect = true;
							}
						}

						if (!strcmp(pEvent.szName, "Engetu_2"))
						{
							if (!m_bEngetuSecondEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Engetu.dat"), mWorldMatrix);

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
								_vector vLook = m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK);
								_vector vPosition = mWorldMatrix.r[3];
								vPosition += XMVectorSet(0.f, 4.f, 0.f, 0.f);
								vPosition += vLook * 2;

								mWorldMatrix.r[3] = vPosition;
								m_HousyutigakuzinStart = CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_Start.dat"), mWorldMatrix);

								m_bHousyutigakuzinFirstEffect = true;
							}
						}

						if (!strcmp(pEvent.szName, "Housyutigakuzin_2"))
						{
							if (!m_bHousyutigakuzinSecondEffect)
							{
								CEffect::PlayEffectAtLocation(TEXT("Housyutigakuzin_End.dat"), mWorldMatrix);

								m_bHousyutigakuzinSecondEffect = true;
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
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
		}
	}

	return nullptr;
}

CPlayerState * CAlphenSkillState::LateTick(_float fTimeDelta)
{
	if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN)
	{
		for (auto& pEffect : m_HousyutigakuzinStart)
		{
			if (pEffect)
			{
				if (pEffect->Get_PreDead())
					pEffect = nullptr;
				else
					pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));
			}
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner);
	}

	return nullptr;
}

CPlayerState * CAlphenSkillState::EventInput(void)
{
	if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
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
			return new CAlphenAttackState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);
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

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(1);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_ENGETU);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(2);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HIENZIN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(3);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_E.wav"), SOUND_EFFECT, 0.6f);
			//이 스킬 이펙트 소리는 Player_Weapon쪽에 있음. 여기서 이펙트 소리 넣으면, 애님이랑 타이밍이 안맞음. 
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_AKIZAME);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(4);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_R.wav"), SOUND_EFFECT, 0.6f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_R.wav"), SOUND_EFFECT, 0.6f);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(5);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_F.wav"), SOUND_EFFECT, 0.6f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_F.wav"), SOUND_EFFECT, 0.6f);
			break;
		}
	}

	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	RELEASE_INSTANCE(CBattleManager);
}

void CAlphenSkillState::Exit(void)
{
	__super::Exit();
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CAlphenSkillState::CallbackFunction(_uint iIndex)
{
	m_HousyutigakuzinStart[iIndex] = nullptr;
}
