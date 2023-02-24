#include "stdafx.h"
#include "..\Public\AI_DodgeState.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Law.h"
#include "GameInstance.h"
#include "AIAttackNormalState.h"
#include "AI_Alphen_NormalAttackState.h"
#include "AI_Alphen_SkillAttackState.h"
#include "AICheckState.h"
#include "AI_Sion_SkillState.h"
#include "AI_Overlimit_State.h"



using namespace AIPlayer;

CAI_DodgeState::CAI_DodgeState(CPlayer * pPlayer, CBaseObj* pTarget ,_bool back , _bool lookat)
{
	m_bbackstep = back;
	m_pOwner = pPlayer;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}



CAIState * CAI_DodgeState::Tick(_float fTimeDelta)
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

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.025f), fRotationRadian, m_pOwner->Get_Navigation());


			m_pOwner->Check_Navigation();

	}


	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();


	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			{
				m_pOwner->On_JustDodge();

				if (nullptr == m_pDodgeCollider)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(2.5f, 2.5f, 2.5f);
					ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
					ColliderDesc.vPosition = _float3(0.f, 2.5f, 0.f);

					m_pDodgeCollider = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
				}
			}
			
			
		}
		else
		{
			if ((ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType) && (m_pOwner->Get_IsJustDodge() == true))
			{
				m_pOwner->Off_JustDodge();

				if (nullptr != m_pDodgeCollider)
				{
					pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pDodgeCollider);
					m_pDodgeCollider = nullptr;
				}
			}
		}
	}

	if (nullptr != m_pDodgeCollider)
		m_pDodgeCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CAIState * CAI_DodgeState::LateTick(_float ftimeDelta)
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


	if (nullptr != m_pDodgeCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MBULLET, m_pDodgeCollider, &pCollisionTarget))
		{
			m_pOwner->Plus_Overcount();
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pDodgeCollider);
#endif
	}

	if (m_bIsAnimationFinished)
	{
		if (m_pOwner->Get_Info().idodgecount >= 5)
		{
			if (m_pTarget == nullptr)
				return nullptr;



			if (nullptr == m_pTarget)
			{
				m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
				(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			}
			m_pOwner->Set_Overcount(0);
			return new CAI_Overlimit_State(m_pOwner , m_pTarget);

		}

		return new CAICheckState(m_pOwner, m_eStateId);
	}

	return nullptr;
}

void CAI_DodgeState::Enter()
{
	
	//__super::Enter();

	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return;

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
		return;
	m_eStateId = STATE_ID::STATE_DODGE;
	if(m_bLookat)
	m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		switch (rand() % 3)
		{
		case 0:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_FRONT);
			break;

		case 1:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, -45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_FRONT);
			break;

		case 2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DODGE_BACK);
			break;

		}
		break;
	case CPlayer::SION:
		if (m_bbackstep)
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND_BACK);
			break;
		}
		else if (!m_bLookat)
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND);
			break;
		}

			switch (rand() % 4)
			{
			case 0:
				m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 90.f ,0.f });
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND);
				break;

			case 1:
				m_pOwner->Get_Transform()->Set_Rotation({ 0.f, -90.f ,0.f });
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND_BACK);
				break;

			default:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_STEP_LAND_BACK);
				break;

			}
			break;
		



	case CPlayer::RINWELL:
		if (m_bbackstep)
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND_BACK);
			break;
		}
		else if (!m_bLookat)
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND);
			break;
		}

		switch (rand() % 4)
		{
		case 0:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND);
			break;

		case 1:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, -45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND);
			break;

		default:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_STEP_LAND_BACK);
			break;

		}
		break;

	case CPlayer::LAW:
		switch (rand() % 3)
		{
		case 0:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND);
			break;

		case 1:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, -45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND);
			break;

		case 2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND_BACK);
			break;

		}
		break;

	}

	m_pOwner->Set_Manarecover(true);

}

void CAI_DodgeState::Exit(void)
{
	__super::Exit();

	Safe_Release(m_pDodgeCollider);
}
