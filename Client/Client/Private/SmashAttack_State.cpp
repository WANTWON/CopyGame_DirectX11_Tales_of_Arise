#include "stdafx.h"
#include "..\Public\SmashAttack_State.h"
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

CSmashAttack_State::CSmashAttack_State(CPlayer* pPlayer, CBaseObj* pTarget)
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

CAIState * CSmashAttack_State::Tick(_float fTimeDelta)
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

				case CPlayer::LAW:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if (nullptr == m_pLawCollider)
						{
							CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

							CCollider::COLLIDERDESC		ColliderDesc;

							ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
							ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
							ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

							m_pLawCollider = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
						}
							
						m_fEventStart = pEvent.fStartTime;
					}
					break;
				}
				}
			}
			else
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::ALPHEN:
					break;
				case CPlayer::SION:
					break;
				case CPlayer::RINWELL:
					break;
				case CPlayer::LAW:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if (nullptr == m_pLawCollider)
						{
							CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

							pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLawCollider);
							m_pLawCollider = nullptr;

							pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_PBULLET, m_pOwner);
						}
					}
					break;
				}
			}
		}

		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	if (nullptr != m_pLawCollider)
	{
		_matrix	WorldBoneMatrix = XMMatrixIdentity();

		if (nullptr != m_pLawCollider)
		{
			WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
				XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

			WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
			WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
			WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

			m_pLawCollider->Update(WorldBoneMatrix);
		}
	}

	return nullptr;
}

CAIState * CSmashAttack_State::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pLawCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLawCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLawCollider);
#endif
	}

	if (m_bIsStateEvent)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (m_bIsAnimationFinished)
	{
		return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}
		

	return nullptr;
}

void CSmashAttack_State::Enter()
{
	_matrix mWorldMatrix = XMMatrixIdentity();

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		CPlayerManager::Get_Instance()->Get_EnumPlayer(0)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillSound_Booster_Fix2.wav"), SOUND_EFFECT, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("PlayerSkillVoice_Booster.wav"), SOUND_EFFECT, 0.5f);
		break;
	case CPlayer::SION:
		CPlayerManager::Get_Instance()->Get_EnumPlayer(1)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE;

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
		mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Sion_Boost.dat"), mWorldMatrix);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Booster.wav"), SOUND_EFFECT, 0.5f);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_Booster.wav"), SOUND_EFFECT, 0.5f);
		break;
	case CPlayer::LAW:
		CPlayerManager::Get_Instance()->Get_EnumPlayer(3)->Set_BoostGuage(0);
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_ATTACK_STRIKE;
		break;
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

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
//	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
	pCamera->Set_Target(m_pOwner);

	m_pOwner->Set_Manarecover(false);
}

void CSmashAttack_State::Exit()
{
	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
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
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	__super::Exit();

	Safe_Release(m_pLawCollider);
}