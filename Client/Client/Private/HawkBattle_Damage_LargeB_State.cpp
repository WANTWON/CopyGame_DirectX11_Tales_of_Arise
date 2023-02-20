#include "stdafx.h"
#include "HawkBattle_Damage_LargeB_State.h"
#include "HawkIdleState.h"
#include "GameInstance.h"

#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_Flying_BackState.h"
#include "HawkBattle_IdleState.h"

using namespace Hawk;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(CHawk* pHawk, STATE_ID eStateId)
{
	m_pOwner = pHawk;
	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	m_eStateId = eStateId;
}

CHawkState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	
	
	return nullptr;


}

CHawkState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	m_pOwner->Check_Navigation();
	
	return nullptr;
}

CHawkState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 1;


	if (m_bIsAnimationFinished)
	{
		if (m_eStateId == STATE_DOWN)
			return new CBattle_IdleState(m_pOwner, STATE_DOWN);

		else
			return new CBattle_RunState(m_pOwner, STATE_END);
	}
		//switch (m_iRand)
		//{
		////case 0:
		////	return new CBattle_ChargeState(m_pOwner);
		////	break;

		////case 0:
		////	return new CBattle_Flying_BackState(m_pOwner);
		////	break;

		////case 2:
		////	return new CBattle_BombingState(m_pOwner);
		////	break;
		//	return new CBattle_RunState(m_pOwner, STATE_END);
		//default:
		//	break;
		//}
	

	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	

	


	switch (m_eStateId)
	{
	case Client::CHawkState::STATE_TAKE_DAMAGE:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::DAMAGE_LARGE_F);
		m_pOwner->SetOff_BedamagedCount();
		m_pOwner->Set_BedamageCount_Delay();
		CGameInstance::Get_Instance()->PlaySounds(TEXT("Hawk_Hit.wav"), SOUND_VOICE, 0.07f);
		break;

	case Client::CHawkState::STATE_DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::DOWN_F);
		m_pOwner->Set_OnGoingDown();
		break;

	default:
		break;
	}
}

void CBattle_Damage_LargeB_State::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
	if (m_eStateId == Client::CHawkState::STATE_TAKE_DAMAGE)
		m_pOwner->SetOff_BedamageCount_Delay();
}