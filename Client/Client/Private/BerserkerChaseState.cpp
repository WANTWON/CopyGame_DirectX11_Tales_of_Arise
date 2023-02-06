#include "stdafx.h"

#include "BerserkerChaseState.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerWalkState.h"

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
	m_fTarget_Distance = Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	
	return nullptr;
}

CBerserkerState * CChaseState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);


	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta * 1.4f, m_pOwner->Get_Navigation());

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