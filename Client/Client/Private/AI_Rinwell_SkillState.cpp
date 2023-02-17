#include "stdafx.h"
#include "..\Public\AI_Rinwell_SkillState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Rinwell.h"
#include "UI_Skillmessage.h"
#include "Bullet.h"
#include "RinwellSkills.h"
#include "AIAttackNormalState.h"
#include "AI_DodgeState.h"



using namespace AIPlayer;

CAI_Rinwell_SkillState::CAI_Rinwell_SkillState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	//m_iCurrentAnimIndex = skillindex;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Rinwell_SkillState::Tick(_float fTimeDelta)
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

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		m_pOwner->Check_Navigation();
	}
	else
		m_pOwner->Check_Navigation();

//////////////////////////////////////////////////////////////////////////////////

	if (m_iCurrentAnimIndex == CRinwell::ANIM::BTL_MAGIC_START && m_bIsAnimationFinished)
	{
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_EMIT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	}
		

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			switch (m_eStateId)
			{
			case STATE_GALEFORCE:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

						//Bullet
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::GALE_FORCE;
						BulletDesc.vTargetDir = XMVector3Normalize(vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
						BulletDesc.fVelocity = 10.f;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + BulletDesc.vTargetDir*2.f;
						BulletDesc.vTargetPosition = vTargetPosition;
						BulletDesc.fDeadTime = 2.f;
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
					}


				}


				break;
			case STATE_THUNDERFIELD:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						m_fEventStart = pEvent.fStartTime;
					}


					break;
			case STATE_METEOR:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime) && !m_bBulletMake)
					{
						CBullet::BULLETDESC BulletDesc;


						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::METEOR;
						
						BulletDesc.vTargetDir = { -0.3f, -1.f, -0.1f, 0.f };


						for (int i = 0; i < 7; ++i)
						{
							BulletDesc.fVelocity = 2.f + ((_float)(rand() % 20 + 1))*0.1f;
							_vector pos = { (_float)(rand() % 40 + 40) , 12.f + i*2.5f , (_float)(rand() % 40 + 40), 1.f };
							BulletDesc.vInitPositon = pos;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
						}

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
						

						m_fEventStart = pEvent.fStartTime;
					}
				}

				break;
				}

			}

		}


	}
	return nullptr;
}

CAIState * CAI_Rinwell_SkillState::LateTick(_float fTimeDelta)
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

	if (m_bIsStateEvent || m_bIsAnimationFinished)
	{
		__super::Exit();
		/*++m_iCurrentAnimIndex;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);*/
		m_bIsStateEvent = false;
		m_bBulletMake = false;
		m_fEventStart = -1.f;
		if (m_bStateFinish)
		{
			m_bStateFinish = false;
			if (m_pOwner->Get_Info().fCurrentMp < 1)
			{
				switch (rand() % 3)
				{
				case 0:
					return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
				case 1:
					return new CAICheckState(m_pOwner, m_eStateId);
				case 2:
					return new CAI_DodgeState(m_pOwner, m_pTarget);

				}
			}


			if (Get_Target_Distance() >= 5.f)
			{
				

				switch (rand() % 6)
				{

				case 0: //Client::CAIState::STATE_NORMAL_ATTACK1:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_GALEFORCE;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_FUATU;
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
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_THUNDERFIELD;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI;
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
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_METEOR;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
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
				/*case 3:
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
					break;*/

				default:
					return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);


				}
			}
			else
				return new CAICheckState(m_pOwner, m_eStateId);
		}
		
	}

	return nullptr;
}

void CAI_Rinwell_SkillState::Enter()
{
	//__super::Enter();
	m_bBulletMake = false;
	//m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
	
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	switch (m_eStateId)
	{
	case STATE_GALEFORCE:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_FUATU;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GRAVITY);
		break;
	case STATE_THUNDERFIELD:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_MAGNARAY);
		break;
	case STATE_METEOR:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_EXPLODE);
		break;
	/*case CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
		break;
	case CSion::ANIM::BTL_ATTACK_THUNDER_BOLT:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GLACIA);
		break;*/
	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Set_Manarecover(false);
	m_pOwner->Use_Mana(1.f);

}

void CAI_Rinwell_SkillState::Exit()
{
	__super::Exit();
}