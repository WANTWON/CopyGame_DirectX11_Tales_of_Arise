#include "stdafx.h"
#include "..\Public\AI_RinwellLaw_Smash.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"
#include "PlayerIdleState.h"
#include "UI_Dialogue_Caption.h"
#include "UI_Skillmessage.h"

using namespace AIPlayer;
using namespace Player;

CAI_RinwellLaw_Smash::CAI_RinwellLaw_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
}

CAIState * CAI_RinwellLaw_Smash::Tick(_float fTimeDelta)
{
	if (m_bStrikeBlur)
		StrikeBlur(fTimeDelta);

	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
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

	m_fTimer += fTimeDelta;

	if (m_pOwner->Get_PlayerID() == CPlayer::RINWELL)
	{
		if (m_fTimer > 2.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();
			m_fTimer = -100.f;
		}

	}

	if (m_pOwner->Get_PlayerID() == CPlayer::LAW)
	{
		if (m_fTimer > 4.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_RINWELLLAWSTRIKE);
			m_fTimer = -100.f;
		}

	}


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta,false);
	if (!m_bIsAnimationFinished)
	{
		vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
		for (auto& pEvent : pEvents)
		{
			if (pEvent.isPlay)
			{
				switch (m_eCurrentPlayerID)
				{
				case CPlayer::RINWELL:
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

	}

	m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CAIState * CAI_RinwellLaw_Smash::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
	}

	if (m_bIsAnimationFinished)
	{

		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
		{

			PLAYER_MODE eMode = CPlayerManager::Get_Instance()->Check_ActiveMode(m_pOwner);
			switch (eMode)
			{
			case Client::ACTIVE:
			{
				CPlayerState* pState = nullptr;

				pState = new CIdleState(m_pOwner, CIdleState::IDLE_SIDE);
				m_pOwner->Set_PlayerState(m_pOwner->Get_PlayerState()->ChangeState(m_pOwner->Get_PlayerState(), pState));
			}



			case Client::AI_MODE:
			{
				return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);
			}
			}

		}


	}

	return nullptr;
}

void CAI_RinwellLaw_Smash::Enter()
{
	m_pOwner->Set_StrikeAttack(true);
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		break;
	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::BTL_MYSTIC_GURENTENSYOU;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_RAINOUI);
		dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(1);

		break;

	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	else
		m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_Target(m_pOwner);

	m_pOwner->Set_Manarecover(false);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("LawRinwell_Smash.wav"), SOUND_VOICE, 0.85f);
}

void CAI_RinwellLaw_Smash::Exit()
{
	if (m_bStrikeBlur)
	{
		m_pOwner->Set_ResetStrikeBlur(true);
		m_bStrikeBlur = false;
	}

	CBattleManager::Get_Instance()->Set_IsStrike(false);
	m_pOwner->Set_StrikeAttack(false);
	m_pOwner->Set_IsActionMode(false);
	if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
	{
		if (!dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_LastStrikeAttack())
		{
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Set_LastStrikeAttack(true);
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Take_Damage(10000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
		}
		else
		{
			dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Take_Damage(10000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
		}
	}

	if (!m_pEffects.empty())
	{
		for (auto& iter : m_pEffects)
		{
			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	__super::Exit();
}

void CAI_RinwellLaw_Smash::StrikeBlur(_float fTimeDelta)
{
	_float fDuration = .45f;
	m_fResetTimer += fTimeDelta;

	/* Zoom Blur Lerp */
	_float fFocusPower = 10.f;

	_float fBlurInterpFactor = m_fResetTimer / fDuration;
	if (fBlurInterpFactor > 1.f)
		fBlurInterpFactor = 1.f;

	_int iDetailStart = 1;
	_int iDetailEnd = 10;
	_int iFocusDetailLerp = iDetailStart + fBlurInterpFactor * (iDetailEnd - iDetailStart);
	m_pOwner->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
}