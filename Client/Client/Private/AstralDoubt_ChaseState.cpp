#include "stdafx.h"

#include "AstralDoubt_ChaseState.h"
#include "GameInstance.h"
#include "AstralDoubt_WalkState.h"
#include "AstralDoubt_TurnState.h"

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
	Find_Target();
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CAstralDoubt_State * CChaseState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
	{
		m_pOwner->Get_Transform()->LookAt(vTargetPos);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 0.7f, m_pOwner->Get_Navigation());
	}

	else
	{
		if (bIs_TargetInFront)
			return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_CHASE, true);

		else
			return new CTurnState(m_pOwner);

	}


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