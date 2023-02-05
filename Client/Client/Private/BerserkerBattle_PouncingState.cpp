#include "stdafx.h"

#include "BerserkerBattle_PouncingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_RunState.h"
#include "BerserkerBattle_Multiple_FireState.h"
#include "BerserkerBattle_TurnState.h"

using namespace Berserker;

CBattle_PouncingState::CBattle_PouncingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;

	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CBattle_PouncingState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBattle_PouncingState::Tick(_float fTimeDelta)
{
	
	Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.7f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.02f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 2;

	return nullptr;
}

CBerserkerState * CBattle_PouncingState::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

		if (m_bIsAnimationFinished)
		{
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);
		}


	return nullptr;
}

void CBattle_PouncingState::Enter()
{
	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_POUNCING);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Pouncing.wav"), SOUND_VOICE, 1.0f);
}

void CBattle_PouncingState::Exit()
{

}