#include "stdafx.h"

#include "BerserkerBattle_Multiple_FireState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_Multiple_FireState::CBattle_Multiple_FireState(CBerserker* pBerserker)
{
	
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_Multiple_FireState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_Multiple_FireState::Tick(_float fTimeDelta)
{
	/*m_fTarget_Distance = Find_BattleTarget();


	
	if (!m_bIsAnimationFinished)*/
		m_pOwner->Check_Navigation();
	
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					if (m_fSoundStart != pEvent.fStartTime)
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_Multiple_Fire.wav"), SOUND_VOICE, 0.1f);
						m_fSoundStart = pEvent.fStartTime;
					}
				}
			}
		}

	return nullptr;
}

CBerserkerState * CBattle_Multiple_FireState::LateTick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (false == m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	//m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//	m_bTargetSetting = true;
	//}

	if (m_bIsAnimationFinished)
	{

		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);
	/*	m_pOwner->Get_Transform()->LookAt(vTargetPosition);

		if (m_fTarget_Distance > 7)
			return new CBattle_RunState(m_pOwner, STATE_ID::STATE_FIREBALL);

		else
			return new CBattle_Shock_WaveState(m_pOwner);*/

	}

	return nullptr;
}

void CBattle_Multiple_FireState::Enter()
{

	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_MULTIPLE_FIRE_B);
	
	m_fSoundStart = -1.f;

	
}

void CBattle_Multiple_FireState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}