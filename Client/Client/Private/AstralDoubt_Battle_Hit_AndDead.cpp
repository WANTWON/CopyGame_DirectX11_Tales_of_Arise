#include "stdafx.h"
#include "..\Public\AstralDoubt_Battle_Hit_AndDead.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"

using namespace Astral_Doubt;

CBattle_Hit_AndDead::CBattle_Hit_AndDead(CAstralDoubt* pAstralDoubt,  STATE_ID StateId, _bool bThirdHit)
{
	m_pOwner = pAstralDoubt;
	m_fTimeDeltaAcc = 0;
	m_fCntChanceTime = ((rand() % 1000) *0.001f)*((rand() % 100) * 0.01f);
	m_bThirdHit = bThirdHit;
	m_eStateId = StateId;
}

CAstralDoubt_State * CBattle_Hit_AndDead::Tick(_float fTimeDelta)
{
	m_pOwner->Set_Done_HitAnimState();

	switch (m_eStateId)
	{
	case Client::CAstralDoubt_State::STATE_DEAD:
		if(!m_bThirdHit)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	case Client::CAstralDoubt_State::STATE_BE_DAMAGED:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta *1.2f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		break;
	default:
		break;
	}
		
	m_pOwner->Check_Navigation();
	return nullptr;
}

CAstralDoubt_State * CBattle_Hit_AndDead::LateTick(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	
	if(m_fTimeDeltaAcc > m_fCntChanceTime)
	m_iRand = rand() % 1;
	
	if (m_bIsAnimationFinished)
	{
		m_pOwner->Set_Finish_HitAnimState();
		
		switch (m_eStateId)
		{
		case Client::CAstralDoubt_State::STATE_DEAD:
			m_bDeadAnimFinish = true;
			if (m_bDeadAnimFinish)
				m_pOwner->Set_GlowUp();
			return new CBattle_Hit_AndDead(m_pOwner, STATE_DEAD, true);
			break;
		case Client::CAstralDoubt_State::STATE_BE_DAMAGED:
			m_pOwner->Set_Done_HitAnimState();
			if (m_bThirdHit)
				return new CBattle_720Spin_FirstState(m_pOwner);
			else
			{
				switch (m_iRand)
				{
				case 0:
					return new CBattleWalkState(m_pOwner, STATE_ID::STATE_END);

				default:
					break;
				}
			}
			break;
		}	
	}
	

	return nullptr;
}

void CBattle_Hit_AndDead::Enter()
{
	m_iRand = rand() % 9;

	switch (m_eStateId)
	{
	case Client::CAstralDoubt_State::STATE_DEAD:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::DEAD);
		
		//if (!m_bDeadSound && !m_bThirdHit)
		//{
		//	CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Dead.wav"), SOUND_VOICE, 0.4f);
		//	m_bDeadSound = true;
		//	break;
		//}
		break;

	case Client::CAstralDoubt_State::STATE_BE_DAMAGED:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::DOWN_UNIQUE);

		//if (!m_bAnimFinish)
		//{
		//	CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Hit.wav"), SOUND_VOICE, 0.4f);
		//	m_bAnimFinish = true;
		//	break;
		//}
		break;
	}

}

void CBattle_Hit_AndDead::Exit()
{
	//CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}
