#include "stdafx.h"
#include "..\Public\IceWolfAttackNormalState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttack_Elemental_Charge.h"

using namespace IceWolf;

CAttackNormalState::CAttackNormalState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);

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
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Attack.wav"), SOUND_VOICE, 0.4f);
					m_bAnimFinish = true;
				}

			}
		}
	}


	return nullptr;
}

CIceWolfState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	

	if (m_bIsAnimationFinished)
	{
		if (m_fTarget_Distance > 10.5f)
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);


		else
			return new CBattle_RunState(m_pOwner, STATE_END);

	}
	
	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_NORMAL);


}


void CAttackNormalState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}


