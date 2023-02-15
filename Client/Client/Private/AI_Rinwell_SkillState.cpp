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

CAI_Rinwell_SkillState::CAI_Rinwell_SkillState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget, _uint skillindex)//, _float fStartHeight, _float fTime)
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
						m_fEventStart = pEvent.fStartTime;
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

			default:
				return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);


			}
		}
		else
			return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void CAI_Rinwell_SkillState::Enter()
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
	}

	m_pOwner->Set_Manarecover(false);
	m_pOwner->Use_Mana(1.f);

}

void CAI_Rinwell_SkillState::Exit()
{
	__super::Exit();
}