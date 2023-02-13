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

CAIState * CAI_Alphen_SkillAttackState::Tick(_float fTimeDelta)
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

			switch (m_iCurrentAnimIndex)
			{
			case CAlphen::ANIM::ANIM_ATTACK_HIENZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
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
			case CAlphen::ANIM::ANIM_ATTACK_AKIZAME:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
				{
					if (!m_bAkizameEffect)
					{
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						CEffect::PlayEffectAtLocation(TEXT("Akizame.dat"), mWorldMatrix);

						m_bAkizameEffect = true;
					}
				}
				
				break;
			case CAlphen::ANIM::ANIM_ATTACK_HOUSYUTIGAKUZIN:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
				if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
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

	/* R */
	m_bAkizameEffect = false;

	/* F */
	m_bHousyutigakuzinFirstEffect = false;
	m_bHousyutigakuzinSecondEffect = false;

	m_HousyutigakuzinStart.clear();
}