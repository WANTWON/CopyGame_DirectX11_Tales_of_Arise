#include "stdafx.h"

#include "AstralDoubt_ChaseState.h"
#include "GameInstance.h"
#include "AstralDoubt_WalkState.h"

using namespace Astral_Doubt;

CChaseState::CChaseState(CAstralDoubt* pAstralDoubt)
{
	m_pOwner = pAstralDoubt;
}

CAstralDoubt_State * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CChaseState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		else
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	else
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}


	if (m_pCurTarget == nullptr)
		return nullptr;
	return nullptr;
}

CAstralDoubt_State * CChaseState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (nullptr == m_pCurTarget)
	{
		m_pCurTarget = m_pOwner->Find_MinDistance_Target();
		m_pOwner->Get_Transform()->LookAt(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAt(m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

	if (m_fTarget_Distance >= 15.f)
		return new CWalkState(m_pOwner, CAstralDoubt_State::FIELD_STATE_ID::STATE_CHASE);

	return nullptr;
}

void CChaseState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::SYMBOL_RUN);

	//CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Chase.wav"), SOUND_VOICE, 1.0f);
}

void CChaseState::Exit()
{
	//CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}