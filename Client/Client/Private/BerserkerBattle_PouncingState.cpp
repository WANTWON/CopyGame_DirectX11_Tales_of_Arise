#include "stdafx.h"

#include "BerserkerBattle_PouncingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_TurnState.h"

using namespace Berserker;

CBattle_PouncingState::CBattle_PouncingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;

	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_PouncingState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_PouncingState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;


	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{

				if (!m_bAnimFinish)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Pouncing.wav"), SOUND_VOICE, 0.4f);
					m_bAnimFinish = true;
				}


			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HTHUMB1_2_L")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(2.f, 2.f, 2.f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_BATTLE, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);
					pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
				}
				else
					m_pAtkColliderCom->Update(matWorld);

				RELEASE_INSTANCE(CCollision_Manager);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
			m_pAtkColliderCom = nullptr;

			pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);

			RELEASE_INSTANCE(CCollision_Manager);
		}
	}

	return nullptr;
}

CBerserkerState * CBattle_PouncingState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	m_pOwner->Check_Navigation();

		if (m_bIsAnimationFinished)
		{
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);
		}

		if (nullptr != m_pAtkColliderCom)
			m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);

	return nullptr;
}

void CBattle_PouncingState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_POUNCING);

}

void CBattle_PouncingState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	Safe_Release(m_pAtkColliderCom);
}