#include "stdafx.h"

#include "BerserkerChaseState.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerWalkState.h"
#include "BerserkerTurnR_State.h"

using namespace Berserker;

CChaseState::CChaseState(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CBerserkerState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CChaseState::Tick(_float fTimeDelta)
{
	Find_Target();
	m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CBerserkerState * CChaseState::LateTick(_float fTimeDelta)
{
	Find_Target();
	m_pOwner->Check_Navigation();

	CBaseObj* pTrigger = m_pOwner->Get_Trigger();
	_vector vTrigger_Pos = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);

	_bool bIs_TargetInFront = false;
	bIs_TargetInFront = Is_TargetInFront(vTrigger_Pos);

	_vector vTargetPos = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);


	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == true)
	{
		m_pOwner->Get_Transform()->LookAtExceptY(vTargetPos);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.4f, m_pOwner->Get_Navigation());
	}

	else
	{
		if (bIs_TargetInFront)
			return new CWalkState(m_pOwner, FIELD_STATE_ID::STATE_CHASE, true);

		else
			return new CTurnR_State(m_pOwner);

	}

	if (m_fTarget_Distance >= 15.f)
		return new CWalkState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_CHASE);

	return nullptr;
}

void CChaseState::Enter()
{
	//m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::MOVE_RUN);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Chase.wav"), SOUND_VOICE, 1.0f);
}

void CChaseState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}