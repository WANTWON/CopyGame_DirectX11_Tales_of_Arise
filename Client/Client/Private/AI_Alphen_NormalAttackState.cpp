#include "stdafx.h"
#include "AI_Alphen_NormalAttackState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"


using namespace AIPlayer;

CAI_Alphen_NormalAttackState::CAI_Alphen_NormalAttackState(CPlayer* pPlayer, STATE_ID eStateType , CBaseObj* pTarget)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	m_pTarget = pTarget;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Alphen_NormalAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			
				switch (m_iCurrentAnimIndex)
				{
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_0:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
							m_bIsStateEvent = true;

						    getchar();
					}
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
			
							m_bIsStateEvent = true;
						getchar();
					}
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_8:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
							m_bIsStateEvent = true;


						getchar();
					}
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_6:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					break;
				}
			
		}
		else
		{
			
				switch (m_iCurrentAnimIndex)
				{
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_0:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_8:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case CAlphen::ANIM::ANIM_ATTACK_NORMAL_6:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				
			}
		}
	}

	return nullptr;
}

CAIState * CAI_Alphen_NormalAttackState::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_iCurrentAnimIndex)
		{
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_0: //Client::CAIState::STATE_NORMAL_ATTACK1:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_1;
			m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_1:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_8;
			m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_8:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_6;
			m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		}
		m_bIsStateEvent = false;
	}

	if (m_bIsAnimationFinished && m_iCurrentAnimIndex == CAlphen::ANIM::ANIM_ATTACK_NORMAL_6)
	{
		return new CAICheckState(m_pOwner , STATE_ATTACK);
	}

	return nullptr;
}

void CAI_Alphen_NormalAttackState::Enter()
{
	//__super::Enter();

	m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

}

void CAI_Alphen_NormalAttackState::Exit()
{
	__super::Exit();
}