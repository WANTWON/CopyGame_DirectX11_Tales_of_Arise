#include "stdafx.h"
#include "..\Public\Monster_LawDodge.h"
#include "Law.h"
#include "GameInstance.h"
#include "AICheckState.h"
#include "Monster_LawIdleState.h"




using namespace MonsterLaw;

CMonster_LawDodge::CMonster_LawDodge(CMonsterLaw * pPlayer , _bool gofront)
{
	//m_bbackstep = back;
	m_pOwner = pPlayer;
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
	
	m_bGofront = gofront;
}



CMonsterLawState * CMonster_LawDodge::Tick(_float fTimeDelta)
{
	
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.025f), fRotationRadian, m_pOwner->Get_Navigation());


		m_pOwner->Check_Navigation();

	}


	/*vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();


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
		m_pDodgeCollider->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());*/

	return nullptr;
}

CMonsterLawState * CMonster_LawDodge::LateTick(_float ftimeDelta)
{
	




	

	if (m_bIsAnimationFinished)
	{

		return new CMonster_LawIdleState(m_pOwner);
	}

	return nullptr;
}

void CMonster_LawDodge::Enter()
{

	//__super::Enter();

	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

	if (m_pTarget == nullptr)
		return;

	//m_eStateId = STATE_ID::STATE_DODGE;
	if (m_bLookat)
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	


	if (m_bGofront)
	{
		switch (rand() % 2)
		{
		case 0:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND);
			break;

		case 1:
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, -45.f ,0.f });
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_STEP_LAND);
			break;
		}
	}
	
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
		




}

void CMonster_LawDodge::Exit(void)
{
	//__super::Exit();

	//Safe_Release(m_pDodgeCollider);
}
