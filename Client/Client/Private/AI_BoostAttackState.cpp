#include "stdafx.h"

#include "AI_BoostAttackState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Law.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"

using namespace AIPlayer;

CAI_BoostAttack::CAI_BoostAttack(CPlayer* pPlayer, CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_BoostAttack::Tick(_float fTimeDelta)
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
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAI_BoostAttack::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	if (m_bIsAnimationFinished)
		return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
	else
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::ALPHEN:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if (!strcmp(pEvent.szName, "Alphen_Strike_1"))
						{
							if (!m_bAlphenStrike_1)
							{
								_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));

								CBullet::BULLETDESC BulletDesc;
								BulletDesc.eCollisionGroup = PLAYER;
								BulletDesc.fDeadTime = 3.f;
								BulletDesc.eBulletType = CAlphenSkills::STRIKE_1;
								BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook * 2.f;
								BulletDesc.pOwner = m_pOwner;
								BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AlphenSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;

								m_bAlphenStrike_1 = true;
							}
						}
						if (!strcmp(pEvent.szName, "Alphen_Strike_2"))
						{
							if (!m_bAlphenStrike_2)
							{
								_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));

								CBullet::BULLETDESC BulletDesc;
								BulletDesc.eCollisionGroup = PLAYER;
								BulletDesc.fDeadTime = 3.f;
								BulletDesc.eBulletType = CAlphenSkills::STRIKE_2;
								BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook * 2.f;
								BulletDesc.pOwner = m_pOwner;
								BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_AlphenSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;

								m_bAlphenStrike_2 = true;
							}
						}
					}
					break;
				}

				case CPlayer::SION:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.fDeadTime = 5.f;
							BulletDesc.eBulletType = CSionSkills::BOOST;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + vLook*2.f;
							BulletDesc.pOwner = m_pOwner;
							BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							m_fEventStart = pEvent.fStartTime;
						}
					}
					break;
				}

				case CPlayer::RINWELL:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							for (_int i = 0; i < CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 1; ++i)
							{
								if (CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp > 0)
								{
									CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Plus_HP(CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp*0.1f);
									if (CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp < CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fCurrentHp)
										CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Set_HP(CPlayerManager::Get_Instance()->Get_EnumPlayer(i)->Get_Info().fMaxHp);
								}


							}

							m_fEventStart = pEvent.fStartTime;
						}
					}
					break;
				}

				case CPlayer::LAW:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{


							m_fEventStart = pEvent.fStartTime;
						}
					}

					else if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if (!strcmp(pEvent.szName, "Law_Boost"))
						{

							if (!m_bLawBoost)
							{
								_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
								vector<CEffect*> Boost = CEffect::PlayEffectAtLocation(TEXT("Law_Boost.dat"), mWorldMatrix);


								_vector vPosition = Boost.front()->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
								mWorldMatrix.r[3] = vPosition;

								CEffect::PlayEffectAtLocation(TEXT("Law_Boost_Impact.dat"), mWorldMatrix);

								m_bLawBoost = true;
							}
						}
					}





					break;
				 }
				}
			}
		}
	}

	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;
		}
	}
	

	if (!m_pEffects2.empty())
	{
		for (auto& iter : m_pEffects2)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;
		}
	}

	if (!m_pEffects3.empty())
	{
		for (auto& iter : m_pEffects3)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;
		}
	}

	
	return nullptr;
}

void CAI_BoostAttack::Enter()
{
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		CPlayerManager::Get_Instance()->Get_EnumPlayer(0)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Booster_Fix2.wav"), SOUND_EFFECT, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_Booster.wav"), SOUND_EFFECT, 0.5f);
		break;
	case CPlayer::SION:
	{
		CPlayerManager::Get_Instance()->Get_EnumPlayer(1)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE;

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Sion_Boost.dat"), mWorldMatrix);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Booster.wav"), SOUND_EFFECT, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_Booster.wav"), SOUND_EFFECT, 0.5f);
		break;
	}
	case CPlayer::RINWELL:
	{
		CPlayerManager::Get_Instance()->Get_EnumPlayer(2)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_STRIKE;
		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartFlash.dat"), mWorldMatrix);
		m_pEffects3 = CEffect::PlayEffectAtLocation(TEXT("Rinwell_Boost_Effect.dat"), mWorldMatrix);

		vOffset = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
		mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects2 = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartRing.dat"), mWorldMatrix);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellSkillSound_Booster.wav"), SOUND_EFFECT, 0.5f);
		break;
	}
	case CPlayer::LAW:
	{
		CPlayerManager::Get_Instance()->Get_EnumPlayer(3)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_ATTACK_STRIKE;
		
		// Sound
		break;
	}
	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
		pCamera->Set_Target(m_pOwner);
	}
	

	m_pOwner->Set_Manarecover(false);
}

void CAI_BoostAttack::Exit()
{
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	for (auto& iter : m_pEffects2)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	for (auto& iter : m_pEffects3)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr && iter->Get_PreDead() )
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	if (!m_pEffects2.empty())
	{
		for (auto& iter : m_pEffects2)
		{
			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	if (!m_pEffects3.empty())
	{
		for (auto& iter : m_pEffects3)
		{
			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	__super::Exit();
}