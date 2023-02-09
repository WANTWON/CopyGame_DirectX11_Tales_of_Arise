#include "stdafx.h"
#include "..\Public\IceWolfAttackBiteState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_HowLingState.h"
#include "IceWolfBattle_RunState.h"


using namespace IceWolf;

CAttackBiteState::CAttackBiteState(class CIce_Wolf* pIceWolf, CBaseObj* pCurTarget)
{
	m_pOwner = pIceWolf;
	m_pCurTarget = pCurTarget;
}

CIceWolfState * CAttackBiteState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CIceWolfState * CAttackBiteState::Tick(_float fTimeDelta)
{
	//m_fDegreeToTarget = RadianToTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 0.9, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
	}
	else 
		m_pCurTarget = pDamageCauser;

	m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.04f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay && m_bAnimFinish == false)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Bite_Start.wav"), SOUND_VOICE, 0.2f);
				m_bAnimFinish = true;

			}
		}
	}

	return nullptr;
}

CIceWolfState * CAttackBiteState::LateTick(_float fTimeDelta)
{
	//if (m_bTargetSetting = false)
	//{
	//	m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//	m_bTargetSetting = true;
	//}

	if (m_pCurTarget == nullptr)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();

		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}

	m_b_IsTargetInFront = Is_TargetInFront(m_vCurTargetPos);

	if (m_bIsAnimationFinished)
	{
		//m_pOwner->Set_Done_HitAnimState();
		m_pOwner->Set_FinishBite();

		if (m_b_IsTargetInFront)
			return new CBattle_SomerSaultState(m_pOwner);
			
		else
			return new CBattle_RunState(m_pOwner, CIceWolfState::STATE_ID::STATE_BITE);

		
		CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Bite_End.wav"), SOUND_VOICE, 0.4f);
	}



	else 
		m_pOwner->Set_OnGoingBite();
	return nullptr;
}

void CAttackBiteState::Enter()
{
	//m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_BITE);


}


void CAttackBiteState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}


