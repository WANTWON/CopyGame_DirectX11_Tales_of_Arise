#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_SomerSaultState.h"

using namespace IceWolf;

CAttackNormalState::CAttackNormalState(class CIce_Wolf* pIceWolf, STATE_ID ePreState)
{
	m_pOwner = pIceWolf;
	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
	m_ePreState = ePreState;
}

CIceWolfState * CAttackNormalState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.4f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");



	//CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	//if (pDamageCauser == nullptr)
	//{
	//	if (m_pCurTarget == nullptr)
	//	{
	//		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}

	//	else if (m_pCurTarget)
	//	{
	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}
	//}

	//else if (pDamageCauser != nullptr)
	//{
	//	m_pCurTarget = pDamageCauser;

	//	m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	//}


	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();


	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{

				if (!m_bAnimFinish)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Attack.wav"), SOUND_VOICE, 0.2f);
					m_bAnimFinish = true;
				}

			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("EX_JAWB1_1_C_end")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(6.f, 6.f, 6.f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_SPHERE, LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);
					pCollisionMgr->Add_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
				}
				else
					m_pAtkColliderCom->Update(matWorld);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

			pCollisionMgr->Collect_Collider(CCollider::TYPE_SPHERE, m_pAtkColliderCom);
			m_pAtkColliderCom = nullptr;

			pCollisionMgr->Out_CollisionGroup(CCollision_Manager::COLLISION_MBULLET, m_pOwner);
		}
	}


	return nullptr;
}

CIceWolfState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	if (nullptr != m_pAtkColliderCom)
	{
		CBaseObj* pCollisionTarget = nullptr;

		if (CCollision_Manager::Get_Instance()->CollisionwithGroup(CCollision_Manager::COLLISION_PLAYER, m_pAtkColliderCom, &pCollisionTarget))
		{
			CPlayer* pCollided = dynamic_cast<CPlayer*>(pCollisionTarget);
			if (pCollided)
				pCollided->Take_Damage(rand() % 100, m_pOwner);
		}
	}

	//if (m_bIsAnimationFinished)
	//{
	//	if (m_fTarget_Distance > 10.5f)
	//		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);


	//	else
	//		return new CBattle_RunState(m_pOwner, STATE_END);

	//}

	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}

	_bool bIsTargetInsight = false;
	bIsTargetInsight = Is_TargetInSight(m_vCurTargetPos, 30.f);
	m_b_IsTargetInFront = Is_TargetInFront(m_vCurTargetPos);


	if (m_bIsAnimationFinished)
	{
		if (m_ePreState == STATE_ID::STATE_BACKSTEP && bIsTargetInsight)
			return new CAttackBiteState(m_pOwner);

		else
		{
			if (m_b_IsTargetInFront)
			{
				switch (m_iRand)
				{
				case 0:
					return new CBattle_BackStepState(m_pOwner);

				case 1:
					return new CBattle_SomerSaultState(m_pOwner);

				default:
					break;
				}
			}

			else
				return new CAttack_Elemental_Charge(m_pOwner, STATE_CHARGE_START, false, false);
		}
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);
#endif // _DEBUG

	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);


}


void CAttackNormalState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);

	Safe_Release(m_pAtkColliderCom);
}


