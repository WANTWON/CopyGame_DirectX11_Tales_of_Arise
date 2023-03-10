#include "stdafx.h"
#include "..\Public\AI_Overlimit_State.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Law.h"
#include "Sion.h"
#include "Rinwell.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"

using namespace AIPlayer;

CAI_Overlimit_State::CAI_Overlimit_State(CPlayer* pPlayer, CBaseObj* pTarget, _bool FinalCameraMode)
{
	m_bFinalMode = FinalCameraMode;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_Overlimit_State::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr && iter->Get_PreDead() == true)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);

				iter = nullptr;
			}
		}
	

	if (nullptr == CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
	}
	else
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}

	if (m_pTarget == nullptr)
		return nullptr;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::ALPHEN:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{

					}
					break;
				}

				case CPlayer::LAW:
				{
					if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
						m_bIsStateEvent = true;
					else if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{


							m_fEventStart = pEvent.fStartTime;
						}
					}
				}
				}
			}
		}

		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAI_Overlimit_State::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		//CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	for (auto& pEffect : m_pEffects)
	{
		if (pEffect && pEffect->Get_PreDead())
			pEffect = nullptr;
	}

	if (m_bFinalMode)
	{
		if (m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
	}
	else if (m_bIsAnimationFinished)
	{
		return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
	}


	return nullptr;
}

void CAI_Overlimit_State::Enter()
{
	CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
	CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Camera"), 1.f);

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ADVENT;

		break;
	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_ADVENT;
		break;

	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ADVENT;
		break;

	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ADVENT;
		break;

	}

	/* Make Effect */
	_vector vOffset = { 0.f,3.f,0.f,0.f };
	_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
	m_pEffects = CEffect::PlayEffectAtLocation(TEXT("Overlimit.dat"), mWorldMatrix);

	m_pOwner->Set_IsActionMode(true);
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC && !m_bFinalMode)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		if (pCamera->Get_CamMode() == CCamera_Dynamic::CAM_BATTLEZONE)
		{
			pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
			pCamera->Set_Target(m_pOwner);
		}
		
	}

	m_pOwner->Set_Manarecover(false);

	m_pOwner->Set_Overlimit(true);
}

void CAI_Overlimit_State::Exit()
{
	m_pOwner->Set_IsActionMode(false);

	for (auto& pEffect : m_pEffects)
	{
		if (pEffect)
		{
			CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(pEffect);
			if (pParticleSystem != nullptr)
				pParticleSystem->Set_Stop(true);
		}
	}

	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC && !m_bFinalMode)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		if(pCamera->Get_CamMode() == CCamera_Dynamic::CAM_AIBOOSTON)
			pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	__super::Exit();
}