#include "stdafx.h"
#include "..\Public\Player_SionSkillAttack.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerJumpState.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "BattleManager.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "Monster.h"
#include "ParticleSystem.h"


using namespace Player;

CPlayer_SionSkillAttack::CPlayer_SionSkillAttack(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CPlayer_SionSkillAttack::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CPlayer_SionSkillAttack::Tick(_float fTimeDelta)
{
	if (CAlphen::ANIM::ANIM_ATTACK_HITENSYUOUKU_LOOP == m_pOwner->Get_Model()->Get_CurrentAnimIndex())
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

		m_pOwner->Check_Navigation();
	}
	else
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
		else
			m_pOwner->Check_Navigation();
	}

	if (!m_pBlastEffect.empty())
	{
		for (auto& iter : m_pBlastEffect)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;

			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}

	if (m_bIsFly)
		m_fTime += 0.1f;

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_SKILL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}

					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !m_bBulletMade)
					{
						if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE))
						{
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
							
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.fVelocity = 1.f;

							BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
							/*if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
							{
								pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							}
							else
							{
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
								(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
							}

							BulletDesc.eBulletType = CSionSkills::GRAVITY;
							BulletDesc.iDamage = 200.f;
							BulletDesc.fDeadTime = 10.f;

							if (pTarget != nullptr)
							{
								BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

							else if (pTarget == nullptr)
								BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
								(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);*/

								BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
							}	
							else 
								BulletDesc.vTargetDir = XMVector3Normalize( m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)*2.f);
							BulletDesc.pOwner = m_pOwner;
							
							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							m_bBulletMade = true;
						}
					}
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}

					break;
				case Client::CPlayerState::STATE_SKILL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}

					break;
				}
			}

		}
		
	

	return nullptr;
}

CPlayerState * CPlayer_SionSkillAttack::LateTick(_float fTimeDelta)
{
	/*if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN)
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
	}*/

	if (m_bIsStateEvent)
		return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if ((0 != m_iSkillEvent) && (floor(m_pOwner->Get_Info().fCurrentMp) > 0))
	{
		switch (m_iSkillEvent)
		{
		case 1:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK1);
			break;
		case 2:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK2);
			break;
		case 3:
			return new CPlayer_SionSkillAttack(m_pOwner, STATE_ID::STATE_SKILL_ATTACK3);
			break;
		}
	}

	if (m_bIsAnimationFinished)
	{

			return new CIdleState(m_pOwner);
	}

	return nullptr;
}

void CPlayer_SionSkillAttack::Enter(void)
{
	__super::Enter();

	
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_TRIPLE_STAR);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK3:
//				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
				break;
			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK2:
			{
				/* Make Effect */
				_vector vOffset = { 0.f,3.f,0.f,0.f };
				_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				mWorldMatrix.r[3] = vLocation + vOffset;
				m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GravitasField.dat"), mWorldMatrix);


				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE);
				break;
			}
			case Client::CPlayerState::STATE_SKILL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_BRAVE);
				break;
			}
		}
	

	_vector vTargetPos = { 0.f,0.f,0.f,1.f };
	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);
	if (pBattleMgr->Get_LackonMonster() != nullptr)
		_vector vTargetPos = pBattleMgr->Get_LackonMonster()->Get_TransformState(CTransform::STATE_TRANSLATION);

	m_pOwner->Get_Transform()->LookAt(vTargetPos);

	RELEASE_INSTANCE(CBattleManager);
}

void CPlayer_SionSkillAttack::Exit(void)
{
	__super::Exit();
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

void CPlayer_SionSkillAttack::CallbackFunction(_uint iIndex)
{
	m_HousyutigakuzinStart[iIndex] = nullptr;
}