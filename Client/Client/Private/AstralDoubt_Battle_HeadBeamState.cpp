#include "stdafx.h"

#include "AstralDoubt_Battle_HeadBeamState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"
#include "AstralDoubt_Battle_720Spin_FirstState.h"
#include "AstralDoubt_Battle_IdleState.h"
#include "AstralDoubt_Battle_SpearMultiState.h"
#include "AstralDoubt_Battle_UpperState.h"
#include "Effect.h"
#include "BossSkills.h"

using namespace Astral_Doubt;

CBattle_HeadBeamState::CBattle_HeadBeamState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_HeadBeamState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_HeadBeamState::Tick(_float fTimeDelta)
{
  	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	Find_Target();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
			{
				
				if (!m_bAnimFinish)
				{
					CGameInstance::Get_Instance()->PlaySounds(TEXT("BossAsu_Attack_HeadBeam.wav"), SOUND_OBJECT, 0.2f);
					m_bAnimFinish = true;
				}
			}		
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				if (!m_bBeam)
				{
					if (!strcmp(pEvent.szName, "Beam"))
					{
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.fDeadTime = 8.3f;
						BulletDesc.fVelocity = 1.f;
						BulletDesc.eBulletType = CBossSkills::TYPE::BULLET_LASER;

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BossSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						m_bBeam = true;
					}
				}
			}
		}
	}

	return nullptr;
}

CAstralDoubt_State * CBattle_HeadBeamState::LateTick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc > m_fRandTime)
		m_iRand = rand() % 3;

	if (m_bIsAnimationFinished)
	{
		
		if (m_fActiveTarget_Distance <= 10.f)
		{
			switch (m_iRand)
			{
			case 0:
				return new CBattle_720Spin_FirstState(m_pOwner);
			case 1:
				return new CBattle_SpearMultiState(m_pOwner);
			case 2:
				return new CBattle_UpperState(m_pOwner);
			}
		}
		else
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_HEADBEAM);
	}

	return nullptr;
}

void CBattle_HeadBeamState::Enter()
{
	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 0.5f, 0.1f);

	m_eStateId = STATE_ID::STATE_IDLE;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_HEAD_BEAM);
}

void CBattle_HeadBeamState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}

void CBattle_HeadBeamState::AimTarget(_float fTimeDelta)
{
	m_fTimeDeltaAcc += fTimeDelta;

	if (m_fTimeDeltaAcc >= 0.5f)
	{
		m_bUpdatTargetPos = false;
		m_fTimeDeltaAcc = 0.f;
	}
}