#include "stdafx.h"
#include "..\Public\AI_Alphen_SkillAttackState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "UI_Skillmessage.h"
#include "ParticleSystem.h"
#include "AI_JumpState.h"
#include "AI_DodgeState.h"
#include "AI_Item_Use_State.h"


using namespace AIPlayer;

CAI_Alphen_SkillAttackState::CAI_Alphen_SkillAttackState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget , _uint skillindex , _float ftime)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	m_iCurrentAnimIndex = skillindex;
	m_fTime = ftime;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Alphen_SkillAttackState::Tick(_float fTimeDelta)
{
	if (CheckTarget() == false)
		return nullptr;

	switch (m_eAIMode)
	{
	case ATTACK_LOCKONMODE:
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
		break;
	case ATTACK_FREEMODE:
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		if (CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.04f), fRotationRadian, m_pOwner->Get_Navigation());
		else if (CAlphen::ANIM::ANIM_ATTACK_HIENZIN == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.005f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
	}
	else if (m_pOwner->Get_IsFly())
		m_fTime += fTimeDelta;

	m_pOwner->Check_Navigation_Jump();

	/* Animation Events */
	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				switch (m_iCurrentAnimIndex)
				{
				case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
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

					break;
				}
				case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
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

					break;
				}
				case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
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

					break;
				}
				case CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN:
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

				case CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA:
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

				case CAlphen::ANIM::ANIM_ATTACK_ENGETU:
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

					break;
				}
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


CAIState * CAI_Alphen_SkillAttackState::LateTick(_float fTimeDelta)
{
	if (CheckTarget() == false)
		return nullptr;

	switch (m_eAIMode)
	{
	case ATTACK_LOCKONMODE:
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
		break;
	case ATTACK_FREEMODE:
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsAnimationFinished)
	{
		if (m_pOwner->Get_IsFly())
			return new CAI_JumpState(m_pOwner, STATETYPE_MAIN, false, m_fTime);

		if (Get_Target_Distance() <= 3.f)
		{
			if (m_pOwner->Get_Info().fCurrentMp > 1)
			{
				switch (rand() % 4)
				{
				case 0: //Client::CAIState::STATE_NORMAL_ATTACK1:
					__super::Exit();
					m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_HIENZIN;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
					dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BEEYEONIN);
					break;
				case 1:
					__super::Exit();
					m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_AKIZAME;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
					dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOOSAWOO);
					break;
				case 2:
					__super::Exit();
					m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
					dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BOONGSUPGEEAKJIN);
					break;

				case 3:
					return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

				}
			
			}
			else
				switch (rand() % 3)
				{
				case 0:
					return new CAICheckState(m_pOwner, m_eStateId);
					break;
				case 1:
					return new CAI_DodgeState(m_pOwner, m_pTarget, true);
				case 2:

					if (CBattleManager::Get_Instance()->Get_AIuseItem())
					{
						ITEM_NAME item;
						switch (rand() % 3)
						{
						case 0:
							item = ITEMNAME_LEMONJELLY;
							break;
						case 1:
							item = ITEMNAME_LEMONJELLY;
							break;
						case 2:
							item = ITEMNAME_LEMONJELLY;
							break;

						}
						return new CAI_Item_Use_State(m_pOwner, item);
					}
					else
						return new CAI_DodgeState(m_pOwner, m_pTarget);

					break;
				}
				
			
		}
		else
			return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void CAI_Alphen_SkillAttackState::Enter()
{
	if (CheckTarget() == false)
		return;

	Reset_Skill();

	//m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
	m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	switch (m_iCurrentAnimIndex)
	{
	case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BEEYEONIN);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOOSAWOO);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BOONGSUPGEEAKJIN);
		break;

	case CAlphen::ANIM::ANIM_ATTACK_RYUUSEIZIN:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_YOUSEONGJIN);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_SENKUSYOUREPA:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_SUMGONGSANGEULPA);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_ENGETU:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_ONEWALL);
		break;
	}

	m_pOwner->Set_Manarecover(false);
	m_pOwner->Use_Mana(1.f);


}

void CAI_Alphen_SkillAttackState::Exit()
{
	__super::Exit();
}

void CAI_Alphen_SkillAttackState::Reset_Skill()
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