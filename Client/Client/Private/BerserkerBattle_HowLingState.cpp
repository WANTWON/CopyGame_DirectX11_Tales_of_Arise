#include "stdafx.h"

#include "BerserkerBattle_HowLingState.h"
#include "GameInstance.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_HowLingState::CBattle_HowLingState(CBerserker* pBerserker)
{
	m_pOwner = pBerserker;
}

CBerserkerState * CBattle_HowLingState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_HowLingState::Tick(_float fTimeDelta)
{

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.1f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay && m_bFisrtSound == false)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Howling.wav"), SOUND_VOICE, 0.4f);
				m_bFisrtSound = true;
			}
		}
	}

	return nullptr;
}

CBerserkerState * CBattle_HowLingState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();

	if (m_bIsAnimationFinished)
	{
		m_pOwner->Set_FinishHowling();
		m_pOwner->Set_OnBerserkerMode();
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_QUADRUPLE);
		
	}







	return nullptr;
}

void CBattle_HowLingState::Enter()
{
	m_eStateId = STATE_ID::STATE_BATTLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_HOWLING);

	m_pOwner->Set_OnGoingHowling();

}

void CBattle_HowLingState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	m_pOwner->SetOff_BedamageCount_Delay();
}