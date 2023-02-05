#include "stdafx.h"
#include "HawkBattle_PeckState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_PeckState::CBattle_PeckState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	m_fRandTime = ((rand() % 3000 + 1000.f) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CBattle_PeckState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;


}

CHawkState * CBattle_PeckState::Tick(_float fTimeDelta)
{
	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

		else if (m_pCurTarget)
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}


	else if (pDamageCauser != nullptr)
	{
		m_pCurTarget = pDamageCauser;

		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}

	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.3, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}
	return nullptr;
}

CHawkState * CBattle_PeckState::LateTick(_float fTimeDelta)
{

	//if (m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(m_vCurTargetPos);
	//	m_bTargetSetting = true;
	//}

	if (m_bIsAnimationFinished)
		return new CBattle_RunState(m_pOwner, CHawkState::STATE_ID::STATE_PECK);


	


	return nullptr;
}

void CBattle_PeckState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_PECK);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Hawk_Peck.wav"), SOUND_VOICE, 1.0f);
}

void CBattle_PeckState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	
}