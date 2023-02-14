#include "stdafx.h"

#include "BerserkerBattle_FireBallState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_FireBallState::CBattle_FireBallState(CBerserker* pBerserker)
{
	
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_FireBallState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_FireBallState::Tick(_float fTimeDelta)
{
	//m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);


	m_pOwner->Check_Navigation();
	

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				if (m_fSoundStart != pEvent.fStartTime)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_FireBall.wav"), SOUND_VOICE, 0.4f);
					m_bFisrtSound = true;
				}
			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("DB_GHAIR1_1_C")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(5.f, 5.f, 5.f);
					ColliderDesc.vPosition = _float3(0.f, -2.f, 0.f);

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

CBerserkerState * CBattle_FireBallState::LateTick(_float fTimeDelta)
{
	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (false == m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	
	//	m_bTargetSetting = true;
	//}

	if (m_bIsAnimationFinished)
	{
		
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);

	}

	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);

	return nullptr;
}

void CBattle_FireBallState::Enter()
{

	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_FIRE_BALL);
		

}

void CBattle_FireBallState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	Safe_Release(m_pAtkColliderCom);
}