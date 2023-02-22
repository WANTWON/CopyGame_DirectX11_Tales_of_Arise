#include "stdafx.h"

#include "AstralDoubt_Battle_HeadBeamState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"

using namespace Astral_Doubt;

CBattle_HeadBeamState::CBattle_HeadBeamState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_HeadBeamState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_HeadBeamState::Tick(_float fTimeDelta)
{
	Find_Target();
	m_fTarget_Distance = m_fOutPutTarget_Distance;

  	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	m_vCurTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	//if (pDamageCauser == nullptr)
	//{
	//	if (m_pCurTarget == nullptr)
	//	{
	//		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	//		if (m_pCurTarget == nullptr)
	//			return nullptr;

	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}
	//	else
	//	{
	//		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	//	}

	//}
	//else
	//{
	//	m_pCurTarget = pDamageCauser;
	//	m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	//}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			{
				if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
					dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 0.5f, 0.1f);
			}

			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

				_matrix matWorld = m_pOwner->Get_Model()->Get_BonePtr("HEAD1_C")->Get_CombinedTransformationMatrix() * XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_PivotFloat4x4()) * m_pOwner->Get_Transform()->Get_WorldMatrix();
				matWorld.r[0] = XMVector4Normalize(matWorld.r[0]);
				matWorld.r[1] = XMVector4Normalize(matWorld.r[1]);
				matWorld.r[2] = XMVector4Normalize(matWorld.r[2]);

				if (nullptr == m_pAtkColliderCom)
				{
					CCollider::COLLIDERDESC		ColliderDesc;

					ColliderDesc.vScale = _float3(1.5f, 1.0f, 60.1f);
					ColliderDesc.vPosition = _float3(0.f, 0.f, -30.f);

					m_pAtkColliderCom = pCollisionMgr->Reuse_Collider(CCollider::TYPE_OBB, LEVEL_BOSS, TEXT("Prototype_Component_Collider_OBB"), &ColliderDesc);
					m_pAtkColliderCom->Update(matWorld);
				}
				else
					m_pAtkColliderCom->Update(matWorld);

				RELEASE_INSTANCE(CCollision_Manager);
			}
		}

		else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType && !pEvent.isPlay)
		{
			CCollision_Manager* pCollisionMgr = GET_INSTANCE(CCollision_Manager);

			pCollisionMgr->Collect_Collider(CCollider::TYPE_OBB, m_pAtkColliderCom);
			m_pAtkColliderCom = nullptr;


			RELEASE_INSTANCE(CCollision_Manager);
		}
	}

	return nullptr;
}

CAstralDoubt_State * CBattle_HeadBeamState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	

	if (m_bUpdatTargetPos)
		AimTarget(fTimeDelta);


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


	//_bool bIs_TargetInFront = false;d
	//bIs_TargetInFront = Is_TargetInFront(vTargetPos);
	


	//	////회전 코드 
	//	 m_pMonSterTransform = m_pOwner->Get_Transform();

	//	 m_vTargetDir = XMVector3Normalize(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pMonSterTransform->Get_State(CTransform::STATE_TRANSLATION));
	//	 m_vLook = XMVector3Normalize(m_pMonSterTransform->Get_State(CTransform::STATE_LOOK));

	//	 m_vLook = XMVectorSetY(m_vLook, 0.f);
	//	 m_vTargetDir = XMVectorSetY(m_vTargetDir, 0.f);
	//	 m_fDot = XMVectorGetX(XMVector3Dot(m_vTargetDir, m_vLook));

	//	 m_vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//	 m_bIs_TargetInRight = Is_TargetRightSide(m_vTargetPos);
	//	


	//if (!m_bIsAnimationFinished)
	//	{
	//		m_fBeamTimeDeltaAcc += fTimeDelta;

	//		if (m_bUpdatTargetPos == false)
	//		{
	//			if (m_bIs_TargetInRight)
	//			{
	//				if (m_fDot < 0.85f)
	//				{
	//					/*if (m_fBeamTimeDeltaAcc <= 1.f)
	//					{*/
	//						m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);
	//		/*			}
	//					else
	//					{
	//						m_fBeamTimeDeltaAcc = 0.f;
	//						m_bUpdatTargetPos = true;
	//					}*/
	//				
	//				}
	//			}
	//			else if(m_bIs_TargetInRight == false)
	//			{
	//				if (m_fDot < 0.85f)
	//				{
	//					//if (m_fBeamTimeDeltaAcc <= 1.f)
	//					//{
	//						m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -0.03f);
	//					//}
	//					//else
	//					//{
	//					//	m_fBeamTimeDeltaAcc = 0.f;
	//					//	m_bUpdatTargetPos = true;
	//					//}
	//				}
	//			}
	//		}
	//	}
		//	m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta *1.6f, m_pOwner->Get_Navigation());
			//if (m_bUpdatTargetPos == false)
			//{
			//	m_pOwner->Get_Transform()->LookAt(vTargetPos);
			//	m_bUpdatTargetPos = true;
			//	m_fTimeDeltaAcc = 0.f;
			//}
		//}
			
	else if (m_bIsAnimationFinished)
	{
		if (m_fTarget_Distance <= 8.f)
		{
			return new CBattle_720Spin_FirstState(m_pOwner);
		}

		else
		return new CBattleWalkState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_HEADBEAM);
	}

#ifdef _DEBUG
	if (nullptr != m_pAtkColliderCom)
		m_pOwner->Get_Renderer()->Add_Debug(m_pAtkColliderCom);
#endif 

	return nullptr;
}

void CBattle_HeadBeamState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_HEAD_BEAM);
}

void CBattle_HeadBeamState::Exit()
{
	Safe_Release(m_pAtkColliderCom);
}

void CBattle_HeadBeamState::AimTarget(_float fTimeDelta)
{
		m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc >= 0.5f)
	{
		m_bUpdatTargetPos = false;
		m_fTimeDeltaAcc = 0.f;
	}
}
