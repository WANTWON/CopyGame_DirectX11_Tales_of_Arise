#include "stdafx.h"
#include "..\Public\AI_AlphenSion_Smash.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"
#include "AiRinwell.h"
#include "PlayerIdleState.h"
#include "UI_Skillmessage.h"

using namespace AIPlayer;
using namespace Player;
CAI_AlphenSion_Smash::CAI_AlphenSion_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
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

CAIState * CAI_AlphenSion_Smash::Tick(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;


	if (m_pOwner->Get_PlayerID() == CPlayer::ALPHEN)
	{
		if (m_fTimer > 4.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_ALPHENSIONSTRIKE);
			m_fTimer = -100.f;
		}
	}


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

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, false);
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
					if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType || !m_bBullet)
					{
						/* Make Effect */
						_vector vOffset = { 0.f,0.f,0.f,0.f };
						CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
						_matrix mWorldMatrix = pLockOn->Get_Transform()->Get_WorldMatrix();
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("AlphenSionStrike.dat"), mWorldMatrix);

						m_bBullet = true;
					}
					break;
				}

				case CPlayer::SION:
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

	
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CAIState * CAI_AlphenSion_Smash::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());

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
			m_pOwner->Set_IsActionMode(false);

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

	m_pOwner->Check_Navigation();

	if (m_bBullet &&!m_bScreen)
	{
		m_fScreenTimer += fTimeDelta;
		if (m_fScreenTimer > 1.5f)
		{
			if (m_eCurrentPlayerID == CPlayer::ALPHEN)
			{
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"));
				m_bScreen = true;
			}
		}
	}

	return nullptr;
}

void CAI_AlphenSion_Smash::Enter()
{
	
	m_pOwner->Set_StrikeAttack(true);
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_SIONALPHEN_SMASH;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::ANIM_ALPHENSION_STRIKE;

		break;

	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Model()->Reset();

	if (!CBattleManager::Get_Instance()->Get_IsBossBattle())
	{
		if (nullptr == m_pTarget)
		{
			m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
			(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		}
		else
			m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
	/*else
	{
		if (nullptr == m_pTarget)
		{
		}
	}*/
	

	m_pOwner->Set_Manarecover(false);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("AlphenSion_Smash.wav"), SOUND_VOICE, 0.5f);
}

void CAI_AlphenSion_Smash::Exit()
{
	__super::Exit();

	
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

	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->fadeout();


	m_pOwner->Set_StrikeAttack(false);
	m_pOwner->Set_IsActionMode(false);
	CBattleManager::Get_Instance()->Set_IsStrike(false);
	CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
	if (pLockOn != nullptr)
	{
		_vector vLastPosition = dynamic_cast<CMonster*>(pLockOn)->Get_LastPosition();
		if (!dynamic_cast<CMonster*>(pLockOn)->Get_LastStrikeAttack())
		{
			dynamic_cast<CMonster*>(pLockOn)->Set_LastStrikeAttack(true);
			dynamic_cast<CMonster*>(pLockOn)->Set_State(CTransform::STATE_TRANSLATION, vLastPosition);
			dynamic_cast<CMonster*>(pLockOn)->Take_Damage(10000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
		}
		else
		{
			dynamic_cast<CMonster*>(pLockOn)->Set_State(CTransform::STATE_TRANSLATION, vLastPosition);
			dynamic_cast<CMonster*>(pLockOn)->Take_Damage(10000, CPlayerManager::Get_Instance()->Get_ActivePlayer());
		}
	}
	
		

	
	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
	

}
