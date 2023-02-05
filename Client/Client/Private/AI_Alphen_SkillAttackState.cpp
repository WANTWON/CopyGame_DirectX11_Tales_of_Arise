#include "stdafx.h"
#include "..\Public\AI_Alphen_SkillAttackState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "UI_Skillmessage.h"


using namespace AIPlayer;

CAI_Alphen_SkillAttackState::CAI_Alphen_SkillAttackState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget , _uint skillindex)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	m_pTarget = pTarget;
	m_iCurrentAnimIndex = skillindex;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Alphen_SkillAttackState::Tick(_float fTimeDelta)
{
	if (CAlphen::ANIM::ANIM_ATTACK_HITENSYUOUKU_LOOP == m_iCurrentAnimIndex)
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

			if (CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN == m_iCurrentAnimIndex)
				m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.05f), fRotationRadian, m_pOwner->Get_Navigation());
			else
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
			case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();

				break;
			case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();

				break;
			case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();

				break;
			}

		}
		else
		{

			switch (m_iCurrentAnimIndex)
			{
			case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
				break;
			case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
				break;
			case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
				break;
			}
		}
	}

	return nullptr;
}

CAIState * CAI_Alphen_SkillAttackState::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsAnimationFinished)
	{
		if (Get_Target_Distance() <= 3.f)
		{
			switch (rand() % 3)
			{
			case 0: //Client::CAIState::STATE_NORMAL_ATTACK1:
				__super::Exit();
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_HIENZIN;
				m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0);
				break;
			case 1:
				__super::Exit();
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_AKIZAME;
				m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(1);
				break;
			case 2:
				__super::Exit();
				m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN;
				m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
				dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(2);
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
	//__super::Enter();

	//m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	switch (m_iCurrentAnimIndex)
	{
	case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(0);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(1);
		break;
	case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(2);
		break;
	}


}

void CAI_Alphen_SkillAttackState::Exit()
{
	__super::Exit();
}