#include "stdafx.h"
#include "..\Public\AI_LAW_SkillAttack_State.h"
#include "Monster.h"
#include "BattleManager.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "ParticleSystem.h"

#include "LawAttackState.h"
#include "LawAirRSkillState.h"
#include "LawAirFSkillState.h"
#include "AICheckState.h"
#include "AI_LAW_NomalAttack_State.h"
#include "AI_DodgeState.h"

using namespace AIPlayer;

CAI_LAW_SkillAttack_State::CAI_LAW_SkillAttack_State(CPlayer * pPlayer, STATE_ID eStaetType)
{
	m_eStateId = eStaetType;
	m_pOwner = pPlayer;

}


CAIState * CAI_LAW_SkillAttack_State::Tick(_float fTimeDelta)
{
	if (CheckTarget() == false)
		return nullptr;

	if ((m_eStateId == CPlayerState::STATE_SKILL_ATTACK_F) && (nullptr != m_pTarget))
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation_Jump();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CAIState::STATE_SKILL_ATTACK_E:
					if (nullptr == m_pRightFootCollider)
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					break;
				case Client::CAIState::STATE_SKILL_ATTACK_R:
					if (nullptr == m_pRightFootCollider)
						m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					break;
				case Client::CAIState::STATE_SKILL_ATTACK_F:
					if (!strcmp(pEvent.szName, "RH"))
					{
						if (nullptr == m_pRightHandCollider)
							m_pRightHandCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "LF"))
					{
						if (nullptr == m_pLeftFootCollider)
							m_pLeftFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					else if (!strcmp(pEvent.szName, "RF"))
					{
						if (nullptr == m_pRightFootCollider)
							m_pRightFootCollider = Get_Collider(CCollider::TYPE_SPHERE, _float3(2.f, 2.f, 2.f), _float3(0.f, 0.f, 0.f), _float3(0.f, 0.f, 0.f));
					}
					m_fEventStartTime = pEvent.fStartTime;
					break;
				}
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
			{
				if (m_pOwner->Get_Info().fCurrentMp > 1)
				{
					switch (rand() % 3)
					{
					case 0:
						m_pOwner->Get_Model()->Reset();
						m_eStateId = STATE_SKILL_ATTACK_E;
						Enter();
						break;
					case 1:
						m_pOwner->Get_Model()->Reset();
						m_eStateId = STATE_SKILL_ATTACK_R;
						Enter();
						break;
					case 2:
						m_pOwner->Get_Model()->Reset();
						m_eStateId = STATE_SKILL_ATTACK_F;
						Enter();
						break;
					}
				}
				else
				{
					switch (rand() % 2)
					{
					case 0:
						//m_pOwner->Get_Model()->Reset();
						return new AI_LAW_NomalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1, m_pTarget);
						break;
					case 1:
						//m_pOwner->Get_Model()->Reset();
						return new CAI_DodgeState(m_pOwner, m_pTarget);
						break;
					}
				}
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				switch (m_eStateId)
				{
				case Client::CAIState::STATE_SKILL_ATTACK_E:
					if (nullptr != m_pRightFootCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
						m_pRightFootCollider = nullptr;
					}
					break;
				case Client::CAIState::STATE_SKILL_ATTACK_R:
					if (nullptr != m_pRightFootCollider)
					{
						pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
						m_pRightFootCollider = nullptr;
					}
					break;
				case Client::CAIState::STATE_SKILL_ATTACK_F:
					if (pEvent.fStartTime == m_fEventStartTime)
					{
						if (!strcmp(pEvent.szName, "RH"))
						{
							if (nullptr != m_pRightHandCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightHandCollider);
								m_pRightHandCollider = nullptr;
							}
						}
						else if (!strcmp(pEvent.szName, "LF"))
						{
							if (nullptr != m_pLeftFootCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pLeftFootCollider);
								m_pLeftFootCollider = nullptr;
							}
						}
						else if (!strcmp(pEvent.szName, "RF"))
						{
							if (nullptr != m_pRightFootCollider)
							{
								pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pRightFootCollider);
								m_pRightFootCollider = nullptr;
							}
						}
						m_fEventStartTime = -1.f;
					}
					break;
				}
			}
		}
	}

	_matrix	WorldBoneMatrix = XMMatrixIdentity();

	if (nullptr != m_pLeftHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pRightHandCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("SLA_GNT_00_E_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pRightHandCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pLeftFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_L")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pLeftFootCollider->Update(WorldBoneMatrix);
	}
	if (nullptr != m_pRightFootCollider)
	{
		WorldBoneMatrix = m_pOwner->Get_Model()->Get_BonePtr("ball_R")->Get_CombinedTransformationMatrix() *
			XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();

		WorldBoneMatrix.r[0] = XMVector4Normalize(WorldBoneMatrix.r[0]);
		WorldBoneMatrix.r[1] = XMVector4Normalize(WorldBoneMatrix.r[1]);
		WorldBoneMatrix.r[2] = XMVector4Normalize(WorldBoneMatrix.r[2]);

		m_pRightFootCollider->Update(WorldBoneMatrix);
	}

	return nullptr;
}

CAIState * CAI_LAW_SkillAttack_State::LateTick(_float fTimeDelta)
{
	if (CheckTarget() == false)
		return nullptr;

	if (nullptr != m_pLeftHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftHandCollider);
#endif
	}

	if (nullptr != m_pRightHandCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pRightHandCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightHandCollider);
#endif
	}

	if (nullptr != m_pLeftFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pLeftFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pLeftFootCollider);
#endif
	}

	if (nullptr != m_pRightFootCollider)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_MONSTER, m_pRightFootCollider, &pCollisionTarget))
		{
			CMonster* pCollided = dynamic_cast<CMonster*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}

#ifdef _DEBUG
		m_pOwner->Get_Renderer()->Add_Debug(m_pRightFootCollider);
#endif
	}

	if (m_bIsAnimationFinished)
		return new CAICheckState(m_pOwner, m_eStateId);

	return nullptr;
}



void CAI_LAW_SkillAttack_State::Enter(void)
{
	//__super::Enter();
	if (CheckTarget() == false)
		return;
	m_pOwner->Use_Mana(1.f);
	m_pOwner->Set_Manarecover(false);


		switch (m_eStateId)
		{
		case Client::CAIState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_RONDSENPU);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_WARYUNGGONGPA);
			break;
		case Client::CAIState::STATE_SKILL_ATTACK_R:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_GARYOUKUUHA);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_YOONMUSUNPOONG);
			break;
		case Client::CAIState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_ATTACK_TYOURENGADAN);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_CHOYUNATAN);
			break;
		}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	

	if (nullptr != m_pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
}

void CAI_LAW_SkillAttack_State::Exit(void)
{
	__super::Exit();
}

void CAI_LAW_SkillAttack_State::Reset_Skill(void)
{

}

CCollider * CAI_LAW_SkillAttack_State::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
{
	CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

	CCollider::COLLIDERDESC		ColliderDesc;

	ColliderDesc.vScale = vScale;
	ColliderDesc.vRotation = vRotation;
	ColliderDesc.vPosition = vPosition;

	switch (eType)
	{
	case Engine::CCollider::TYPE_AABB:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_AABB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_OBB:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_OBB"), &ColliderDesc);
		break;
	case Engine::CCollider::TYPE_SPHERE:
		return pCollisionMgr->Reuse_Collider(eType, m_pOwner->Get_Level(), TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
		break;
	}

	return nullptr;
}
