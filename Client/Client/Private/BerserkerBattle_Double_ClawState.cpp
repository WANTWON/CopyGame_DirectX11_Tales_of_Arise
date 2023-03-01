#include "stdafx.h"

#include "BerserkerBattle_Double_ClawState.h"
#include "GameInstance.h"

#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_WalkState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_Double_ClawState::CBattle_Double_ClawState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
	m_fRandTime = ((rand() % 3000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_Double_ClawState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::Tick(_float fTimeDelta)
{
	//Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.3f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();


	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				if (m_fSoundStart != pEvent.fStartTime)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Double_Claw.wav"), SOUND_VOICE, 0.4f);
					m_fSoundStart = pEvent.fStartTime;
				}
			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				_matrix R_matWorld = m_pOwner->Get_Model()->Get_BonePtr("HMIDDLE1_2_R")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				R_matWorld.r[0] = XMVector4Normalize(R_matWorld.r[0]);
				R_matWorld.r[1] = XMVector4Normalize(R_matWorld.r[1]);
				R_matWorld.r[2] = XMVector4Normalize(R_matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
				}

				else if (nullptr != m_pAtkColliderCom)
					m_pAtkColliderCom->Update(matWorld);

				if (nullptr == m_p2th_AtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc2th;

					ColliderDesc2th.vScale = _float3(6.f, 6.f, 6.f);
					ColliderDesc2th.vPosition = _float3(0.f, 0.f, 0.f);

					m_p2th_AtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc2th);
					m_p2th_AtkColliderCom->Update(R_matWorld);

					pCollisionMgr->Add_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);
				}

				else if (nullptr != m_p2th_AtkColliderCom)
					m_p2th_AtkColliderCom->Update(R_matWorld);

				RELEASE_INSTANCE(CCollision_Manager);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

			pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_pAtkColliderCom, m_pOwner);
			pCollisionMgr->Out_CollisionGroupCollider(CCollision_Manager::COLLISION_MBULLET, m_p2th_AtkColliderCom, m_pOwner);

			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_p2th_AtkColliderCom);

			m_pAtkColliderCom = nullptr;
			m_p2th_AtkColliderCom = nullptr;

			RELEASE_INSTANCE(CCollision_Manager);
		}
	}

	return nullptr;
}

CBerserkerState * CBattle_Double_ClawState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;
	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	m_bAngry = m_pOwner->Get_IsAngry();

	if (m_bIsAnimationFinished)
	{
		/*	if (m_bAngry == false)
			{
				return new CBattle_WalkState(m_pOwner);
			}

			else
			{*/
		switch (m_iRand)
		{
		case 0:
			return new CBattle_WalkState(m_pOwner);

		case 1:
			return new CBattle_RunState(m_pOwner);
		}
		/*}*/
	}

	if (nullptr != m_pAtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;

		if (m_bCollision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (370 - 250 + 1) + 250, m_pOwner);

				m_bCollision = true;
			}
		}
	}

	if (m_bCollision)
	{
		m_fAtkCollision_Delay += fTimeDelta;

		if (m_fAtkCollision_Delay >= 0.8f)
		{
			m_fAtkCollision_Delay = 0.f;
			m_bCollision = false;
		}
	}


	if (nullptr != m_p2th_AtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;

		if (m_b2th_Collision == false)
		{
			if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
			{
				CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
				if (pCollided)
					pCollided->Take_Damage(rand() % (370 - 250 + 1) + 250, m_pOwner);

				m_b2th_Collision = true;
			}
		}
	}

	if (m_b2th_Collision)
	{
		m_f2th_AtkCollision_Delay += fTimeDelta;

		if (m_f2th_AtkCollision_Delay >= 0.8f)
		{
			m_f2th_AtkCollision_Delay = 0.f;
			m_b2th_Collision = false;
		}
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);

	if (nullptr != m_p2th_AtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_p2th_AtkColliderCom);
#endif // _DEBUG

	return nullptr;
}

void CBattle_Double_ClawState::Enter()
{

	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_DOUBLE_CLAW);

	m_fSoundStart = -1.f;
}

void CBattle_Double_ClawState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	Safe_Release(m_pAtkColliderCom);
	Safe_Release(m_p2th_AtkColliderCom);

	m_pOwner->SetOff_BedamageCount_Delay();
}