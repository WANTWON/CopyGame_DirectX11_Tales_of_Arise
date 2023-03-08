#include "stdafx.h"
#include "..\Public\AI_Sion_SkillState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "UI_Skillmessage.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AIAttackNormalState.h"
#include "AI_DodgeState.h"
#include "AI_JumpState.h"
#include "AI_Item_Use_State.h"



using namespace AIPlayer;

CAI_Sion_SkillState::CAI_Sion_SkillState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget, _uint skillindex ,_float fTime)//, _float fStartHeight, _float fTime)
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
	m_fTime = fTime;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Sion_SkillState::Tick(_float fTimeDelta)
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



	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}





	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (CSion::ANIM::BTL_ATTACK_TRESVENTOS)
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		else
			m_pOwner->Check_Navigation_Jump();
	}
	else
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		else
			m_pOwner->Check_Navigation();
	}



	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			switch (m_iCurrentAnimIndex)
			{
			case CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 1.f;
						BulletDesc.eBulletType = CSionSkills::GRAVITY;
						BulletDesc.iDamage = 100;
						BulletDesc.fDeadTime = 10.f;
						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							//BulletDesc.vTargetPosition
							BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
						}
						else
							BulletDesc.vTargetDir = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)*2.f);
						BulletDesc.pOwner = m_pOwner;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
					}


				}


				break;
			case CSion::ANIM::BTL_ATTACK_MAGNARAY:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CSion::ANIM::BTL_ATTACK_MAGNARAY))
						{
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.fVelocity = 1.f;
							BulletDesc.eBulletType = CSionSkills::MAGNA_RAY;
							BulletDesc.iDamage = 74;
							BulletDesc.fDeadTime = 2.f;
							BulletDesc.vTargetDir = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)*4.f);
							BulletDesc.pOwner = m_pOwner;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							m_fEventStart = pEvent.fStartTime;
						}
					}


					break;
			case CSion::ANIM::BTL_ATTACK_BRAVE:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime) && !m_bBulletMake)
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 1.f;
						BulletDesc.eBulletType = CSionSkills::EXPLOSION;
						BulletDesc.iDamage = 300 + rand() % 99;

						BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.vInitPositon = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.vInitPositon.m128_f32[1] = 8.f;
						BulletDesc.pOwner = m_pOwner;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						m_bBulletMake = true;
						m_fEventStart = pEvent.fStartTime;
					}
				}

				break;

			case CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime) && !m_bBulletMake)
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CSionSkills::AQUA_LUINA;
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;





						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						m_bBulletMake = true;
						m_fEventStart = pEvent.fStartTime;

					}
				}

				break;

			case CSion::ANIM::BTL_ATTACK_THUNDER_BOLT:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					m_bIsStateEvent = true;
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 2.f;
						BulletDesc.eBulletType = CSionSkills::GLACIA;
						BulletDesc.iDamage = 70 + rand() % 10;
						BulletDesc.fDeadTime = 5.f;

						BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.vTargetPosition.m128_f32[2] += 3.f;

						BulletDesc.vInitPositon = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.vInitPositon.m128_f32[2] += 3.f;

						BulletDesc.vInitPositon.m128_f32[1] = 8.f;
						BulletDesc.pOwner = m_pOwner;

						/* Make Effect */
						_vector vOffset = { 0.f,0.f,0.f,0.f };
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = BulletDesc.vInitPositon;
						m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GlacioStart.dat"), mWorldMatrix);

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] += 3.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] -= 6.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[2] -= 6.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[2] += 3.f;
							BulletDesc.vTargetPosition.m128_f32[0] += 3.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] -= 6.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[2] += 3.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							BulletDesc.vTargetPosition.m128_f32[0] += 3.f;
						}
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						m_fEventStart = pEvent.fStartTime;
					}
				}

				break;

			case CSion::ANIM::BTL_ATTACK_TRESVENTOS:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

					if ((m_fEventStart != pEvent.fStartTime))
					{
						_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK));
						_vector vRight = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_RIGHT));
						_vector vPostion = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CSionSkills::TRESVENTOS;
						BulletDesc.vInitPositon = XMVectorSetY(vPostion, XMVectorGetY(vPostion) + 3.f) + vLook*2.f;
						if (m_iCount == 0)
							BulletDesc.vInitPositon -= vRight*2.f;
						if (m_iCount == 1)
							BulletDesc.vInitPositon += vRight*2.f;

						BulletDesc.pOwner = m_pOwner;
						BulletDesc.fVelocity = 5.f;
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						BulletDesc.vTargetDir = vLook;
						BulletDesc.pTarget = pTarget;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
						m_iCount++;
					}

				}
				}

				break;
			}

			}

		}

		return nullptr;
}
	


CAIState * CAI_Sion_SkillState::LateTick(_float fTimeDelta)
{
	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();

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



	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	if (m_bIsStateEvent)
		m_fEventStart = -1.f;
	if (m_bIsStateEvent || m_bIsAnimationFinished)
	{
		__super::Exit();
		/*++m_iCurrentAnimIndex;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);*/
		m_bIsStateEvent = false;
		m_bBulletMake = false;
		//m_fEventStart = -1.f;

		if (m_pOwner->Get_IsFly())
			return new CAI_JumpState(m_pOwner, STATETYPE_MAIN ,false ,  m_fTime);

		if (m_pOwner->Get_Info().fCurrentMp < 1)
		{
			switch (rand() % 4)
			{
			case 0:
				return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
			case 1:
				return new CAICheckState(m_pOwner, m_eStateId);
			case 2:
				return new CAI_DodgeState(m_pOwner, m_pTarget);
			case 3:

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

	
		if (Get_Target_Distance() >= 5.f)
		{
			switch (rand() % 10)
			{
			case 0: //Client::CAIState::STATE_NORMAL_ATTACK1:
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE;
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GRAVITY);
				break;
			case 1:
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_MAGNARAY;
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_MAGNARAY);
				break;
			case 2:
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_BRAVE;
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_EXPLODE);
				break;
			case 3:
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET;
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
				break;
			case 4:
				__super::Exit();
				m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_THUNDER_BOLT;
				if (nullptr == m_pTarget)
				{
					m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
					(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				}
				else
					m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GLACIA);
				break;

			case 5:
				return new CAI_JumpState(m_pOwner, STATETYPE_START, true);


			default:
				return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);


			}
		}
		else
			return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void CAI_Sion_SkillState::Enter()
{
	//__super::Enter();
	m_bBulletMake = false;
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
	case CSion::ANIM::BTL_ATTACK_GRAVITY_FORCE:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GRAVITY);
		break;
	case CSion::ANIM::BTL_ATTACK_MAGNARAY:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_MAGNARAY);
		break;
	case CSion::ANIM::BTL_ATTACK_BRAVE:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_EXPLODE);
		break;
	case CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
		break;
	case CSion::ANIM::BTL_ATTACK_THUNDER_BOLT:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GLACIA);
		break;
	case CSion::ANIM::BTL_ATTACK_TRESVENTOS:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_TRESVENTUS);
		break;
	}

	m_pOwner->Set_Manarecover(false);
	m_pOwner->Use_Mana(1.f);

}

void CAI_Sion_SkillState::Exit()
{
	__super::Exit();
}