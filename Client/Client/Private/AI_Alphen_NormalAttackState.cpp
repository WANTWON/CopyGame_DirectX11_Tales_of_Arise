#include "stdafx.h"
#include "AI_Alphen_NormalAttackState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Alphen.h"
#include "Effect.h"

using namespace AIPlayer;

CAI_Alphen_NormalAttackState::CAI_Alphen_NormalAttackState(CPlayer* pPlayer, STATE_ID eStateType , CBaseObj* pTarget)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Alphen_NormalAttackState::Tick(_float fTimeDelta)
{
	

	if(CheckTarget() == false)
		return nullptr;
	
	

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
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				if (m_iCurrentAnimIndex != CAlphen::ANIM::ANIM_ATTACK_NORMAL_6)
				{
					m_bIsStateEvent = true;
					getchar();
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				_tchar wcEffectName[MAX_PATH] = TEXT("");
				switch (m_iCurrentAnimIndex)
				{
					case CAlphen::ANIM::ANIM_ATTACK_NORMAL_0:
					case CAlphen::ANIM::ANIM_ATTACK_NORMAL_8:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_1.dat"));
						break;
					case CAlphen::ANIM::ANIM_ATTACK_NORMAL_1:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_2.dat"));
						break;
					case CAlphen::ANIM::ANIM_ATTACK_NORMAL_6:
						wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_3.dat"));
						break;
				}

				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();

				if (!m_bEffectSlashSpawned)
				{
					if (!wcscmp(wcEffectName, TEXT("Normal_Attack_2.dat")))
						m_SlashEffect = CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);
					else
						CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);

					m_bEffectSlashSpawned = true;
				}
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
	if (CheckTarget() == false)
		return nullptr;

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_iCurrentAnimIndex)
		{
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_0: //Client::CAIState::STATE_NORMAL_ATTACK1:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_1;
			if (nullptr == m_pTarget)
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			}
			else
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_1:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_8;
			if (nullptr == m_pTarget)
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			}
			else
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		case CAlphen::ANIM::ANIM_ATTACK_NORMAL_8:
			__super::Exit();
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_6;
			if (nullptr == m_pTarget)
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
				m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			}
			else
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			break;
		}
		m_bIsStateEvent = false;
		m_bEffectSlashSpawned = false;
	}

	if (m_bIsAnimationFinished && m_iCurrentAnimIndex == CAlphen::ANIM::ANIM_ATTACK_NORMAL_6)
	{
		return new CAICheckState(m_pOwner , STATE_ATTACK);
	}

	return nullptr;
}

void CAI_Alphen_NormalAttackState::Enter()
{
	m_bEffectSlashSpawned = false;

	//__super::Enter();
	if (CheckTarget() == false)
		return;

	m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
	m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(false);
}

void CAI_Alphen_NormalAttackState::Exit()
{
	__super::Exit();
}