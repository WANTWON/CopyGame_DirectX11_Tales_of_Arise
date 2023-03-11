#include "stdafx.h"
#include "..\Public\PlayerOverlimit.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Effect.h"
#include "ParticleSystem.h"

using namespace Player;

CPlayerOverlimit::CPlayerOverlimit(CPlayer * pPlayer, _bool FinalCameraMode)
{
	m_bFinalMode = FinalCameraMode;

	m_pOwner = pPlayer;
}

CPlayerState * CPlayerOverlimit::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CPlayerOverlimit::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	if(!CBattleManager::Get_Instance()->Get_LackonMonster())
		CBaseObj* pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
    else
	{
		CBaseObj* pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	switch (m_ePlayerID)
	{
	case CPlayer::ALPHEN:
		if (!m_bAlphenSound)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("AlphenOverLimit.wav"), SOUND_NATURE, 0.85f);
			m_bAlphenSound = true;
		}
		break;

	case CPlayer::SION:
		if (!m_bSionSound)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionOverLimit.wav"), SOUND_NATURE, 0.85f);
			m_bSionSound = true;
		}
		break;

	case CPlayer::RINWELL:
		if (!m_bRinwellSound)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellOverLimit.wav"), SOUND_NATURE, 0.85f);
			m_bRinwellSound = true;
		}
		break;

	case CPlayer::LAW:
		if (!m_bLawSound)
		{
			CGameInstance::Get_Instance()->PlaySounds(TEXT("LawOverLimit.wav"), SOUND_NATURE, 0.65f);
			m_bLawSound = true;
		}
		break;
		

	default:
		break;
	}

	return nullptr;
}

CPlayerState * CPlayerOverlimit::LateTick(_float fTimeDelta)
{
	if (m_bFinalMode)
	{
		if(m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}
	else if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

void CPlayerOverlimit::Enter(void)
{
	__super::Enter();
	m_ePlayerID = m_pOwner->Get_PlayerID();
	m_eStateId = STATE_ID::STATE_OVERLIMIT;

	if (CPlayer::ALPHEN == m_ePlayerID)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_HADOUMEPPU_START);
	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(0);

	CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
	CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Camera"), 1.f);

	if(!m_bFinalMode)
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("OverLimit.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());
	else
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("FinalOverLimit.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());


	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return;

	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		CBaseObj* pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
	{
		CBaseObj* pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}

	if (!m_bFinalMode)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Target(m_pOwner);
	}

	m_pOwner->Set_IsActionMode(true);

	m_pOwner->Set_Manarecover(false);
	m_pOwner->Set_Overlimit(true);
}

void CPlayerOverlimit::Exit(void)
{
	m_pOwner->Set_IsActionMode(false);

	__super::Exit();
}