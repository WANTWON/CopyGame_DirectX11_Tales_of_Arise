#include "stdafx.h"

#include "BerserkerBattle_FireBallState.h"
#include "GameInstance.h"
#include "BerserkerBattle_IdleState.h"
#include "BerserkerBattle_Shock_WaveState.h"
#include "BerserkerBattle_DeadState.h"
#include "BerserkerBattle_BackStepState.h"
#include "BerserkerBattle_RunState.h"

using namespace Berserker;

CBattle_FireBallState::CBattle_FireBallState(CBerserker* pBerserker)
{
	
	m_pOwner = pBerserker;
	
}

CBerserkerState * CBattle_FireBallState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CBattle_FireBallState::Tick(_float fTimeDelta)
{
	//m_fTarget_Distance = Find_BattleTarget();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.6f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())/*, "ABone"*/);


	m_pOwner->Check_Navigation();
	

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay && m_bFisrtSound == false)
		{

			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Berserker_FireBall.wav"), SOUND_VOICE, 0.4f);
				m_bFisrtSound = true;
			}
		}
	}

	return nullptr;
}

CBerserkerState * CBattle_FireBallState::LateTick(_float fTimeDelta)
{
	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	//if (false == m_bTargetSetting)
	//{
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	
	//	m_bTargetSetting = true;
	//}

	if (m_bIsAnimationFinished)
	{
		
		return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BATTLE);

	}



	return nullptr;
}

void CBattle_FireBallState::Enter()
{

	m_eStateId = STATE_ID::STATE_ANGRY;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_FIRE_BALL);
		

}

void CBattle_FireBallState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}