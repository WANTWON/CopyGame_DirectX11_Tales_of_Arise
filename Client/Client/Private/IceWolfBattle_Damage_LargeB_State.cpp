#include "stdafx.h"
#include "..\Public\IceWolfBattle_Damage_LargeB_State.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfAttackBiteState.h"
#include "IceWolfBattle_RunState.h"
#include "IceWolfAttackBiteState.h"

using namespace IceWolf;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(class CIce_Wolf* pIceWolf,  STATE_ID StateId, _bool bThirdHit)
{
	m_pOwner = pIceWolf;
	m_fTimeDeltaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_bThirdHit = bThirdHit;
	m_eStateId = StateId;
}

CIceWolfState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{

	switch (m_eStateId)
	{
	case Client::CIceWolfState::STATE_DEAD:
		if(!m_bThirdHit)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	case Client::CIceWolfState::STATE_BE_DAMAGED:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	default:
		break;
	}
		
	m_pOwner->Check_Navigation();
	return nullptr;
}

CIceWolfState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;


	if(m_fTimeDeltaAcc > m_fCntChanceTime)
	m_iRand = rand() % 3;
	
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CIceWolfState::STATE_DEAD:
			m_bDeadAnimFinish = true;
			if (m_bDeadAnimFinish)
				m_pOwner->Set_GlowUp();
			return new CBattle_Damage_LargeB_State(m_pOwner, STATE_DEAD, true);
			break;
		case Client::CIceWolfState::STATE_BE_DAMAGED:
			m_pOwner->Set_Done_HitAnimState();
			if (m_bThirdHit)
				return new CBattle_SomerSaultState(m_pOwner);
			else
			{
				switch (m_iRand)
				{
				case 0:
					return new CBattle_RunState(m_pOwner, STATE_ID::STATE_END);
					break;
				case 1:
					return new CBattle_RunState(m_pOwner, STATE_ID::STATE_END);
					break;
				case 2:
					return new CBattle_RunState(m_pOwner, STATE_ID::STATE_BE_DAMAGED);
					break;

				default:
					break;
				}
			}
			break;
		}	
	}
	

	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	m_iRand = rand() % 9;

	switch (m_eStateId)
	{
	case Client::CIceWolfState::STATE_DEAD:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DEAD);
		
		if (!m_bDeadSound && !m_bThirdHit)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Dead.wav"), SOUND_VOICE, 0.4f);
			m_bDeadSound = true;
			break;
		}
		break;

	case Client::CIceWolfState::STATE_BE_DAMAGED:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DAMAGE_SMALL_B);

		if (!m_bAnimFinish)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Hit.wav"), SOUND_VOICE, 0.4f);
			m_bAnimFinish = true;
			break;
		}
		break;
	}

}

void CBattle_Damage_LargeB_State::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}
