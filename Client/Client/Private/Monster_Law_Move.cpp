#include "stdafx.h"
#include "..\Public\Monster_Law_Move.h"
#include "GameInstance.h"

#include "Law.h"

#include "Monster_LawIdleState.h"






using namespace MonsterLaw;

CMonster_Law_Move::CMonster_Law_Move(CMonsterLaw* pPlayer)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());

}

CMonsterLawState * CMonster_Law_Move::Tick(_float fTimeDelta)
{

	
		
	m_fbreaktimer += fTimeDelta;


	




	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());


	if (m_bStopRunning)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;
			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
			//m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());
		}

	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
		if (nullptr == m_pTarget)
		{
			m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());
		}
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta*2.f, m_pOwner->Get_Navigation());
	}


	m_pOwner->Check_Navigation();


	return nullptr;
}

CMonsterLawState * CMonster_Law_Move::LateTick(_float fTimeDelta)
{
	
	
		

	
	m_pTarget = CPlayerManager::Get_Instance()->Get_EnumPlayer(m_pOwner->Get_Phase());



	if (!m_bStopRunning)
	{
	
			if (Get_Target_Distance() <= 4.5f)
			{
				m_pOwner->Get_Model()->Reset();

				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_BRAKE);
				m_bStopRunning = true;
			}

	}

	if(m_fbreaktimer > 0.5f && m_bStopRunning)
		return new CMonster_LawIdleState(m_pOwner);


	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	if (m_bStopRunning)
	{
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{

					return new CMonster_LawIdleState(m_pOwner);

				}


			}


		}
	}
	

	/*if (m_bIsAnimationFinished && m_bStopRunning)
	{
		
		return new CMonster_LawIdleState(m_pOwner);
		

	}*/






	return nullptr;
}

void CMonster_Law_Move::Enter()
{


m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_RUN);



}









//RinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f)


void CMonster_Law_Move::Exit()
{
	m_pOwner->Get_Model()->Reset();
}

//void CMonster_Law_Move::ChaseTarget(_float fTimeDelta)
//{
//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
//	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);
//
//	_vector vTargetDir = XMVector3Normalize(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION)
//		- m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
//	_vector vLook = XMVector3Normalize(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_LOOK));
//
//	vLook = XMVectorSetY(vLook, 0.f);
//	vTargetDir = XMVectorSetY(vTargetDir, 0.f);
//
//	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));
//
//	if (fDot < 0.7f)
//		m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.05f);
//
//	m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2, m_pOwner->Get_Navigation());
//}
