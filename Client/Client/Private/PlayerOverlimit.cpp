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

	if (CPlayer::ALPHEN == m_ePlayerID)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

		CCollision_Manager* pCollisionMgr = CCollision_Manager::Get_Instance();

		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Set_Rotation(true, 10.f);
			}
			else
			{
				if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				{
					dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Set_Rotation(false, 0.f);
					//dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Reset_RotateTime();
				}
			}
		}
	}

	m_pOwner->Check_Navigation();

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

	m_eStateId = STATE_ID::STATE_OVERLIMIT;
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