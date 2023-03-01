#include "stdafx.h"
#include "..\Public\AI_SionLaw_Smash.h"
#include "GameInstance.h"
#include "Law.h"
#include "Sion.h"
#include "AICheckState.h"
#include "CameraManager.h"
#include "Effect.h"
#include "Bullet.h"
#include "SionSkills.h"
#include "AlphenSkills.h"
#include "ParticleSystem.h"
#include "PlayerIdleState.h"
#include "UI_Skillmessage.h"

using namespace AIPlayer;
using namespace Player;
CAI_SionLaw_Smash::CAI_SionLaw_Smash(CPlayer* pPlayer, CBaseObj* pTarget)
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

CAIState * CAI_SionLaw_Smash::Tick(_float fTimeDelta)
{

	m_fTimer += fTimeDelta;


	if (m_pOwner->Get_PlayerID() == CPlayer::LAW)
	{
		if (m_fTimer > 4.f)
		{
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_SIONLAWSTRIKE);
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
				case CPlayer::SION:
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
					else if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
					{
						if ((m_fEventStart != pEvent.fStartTime) && !m_bBullet)
						{

							/* Make Effect */
							_vector vOffset = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] +=  vOffset*10.f + XMVectorSet(-10.f, 0.f, 0.f, 0.f);
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("BunStrikeBurst.dat"), mWorldMatrix);
							m_fFadeTime = 0.f;
							m_bBullet = true;
							m_fEventStart = pEvent.fStartTime;
						}
					}
				}
				}
			}
		}
	
	}

	m_pOwner->Check_Navigation();




	return nullptr;
}

CAIState * CAI_SionLaw_Smash::LateTick(_float fTimeDelta)
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

	if (m_bBullet)
	{
		m_fFadeTime += fTimeDelta;

		if (m_fFadeTime > 4.f)
		{
			if (m_eCurrentPlayerID == CPlayer::LAW)
			{
				CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"));
			}

		}
	}

	return nullptr;
}

void CAI_SionLaw_Smash::Enter()
{
	m_pOwner->Set_StrikeAttack(true);
	
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


	switch (m_eCurrentPlayerID)
	{
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::ANIM_LAWSION_STRIKE;

		break;
	case CPlayer::LAW:
		m_iCurrentAnimIndex = CLaw::ANIM::ANIM_SIONLAW_STRIKE;


		break;

	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	if (!CBattleManager::Get_Instance()->IsAllMonsterDead())
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
	

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	//	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
	pCamera->Set_Target(m_pOwner);

	m_pOwner->Set_Manarecover(false);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("LawSion_Smash.wav"), SOUND_VOICE, 0.7f);
}

void CAI_SionLaw_Smash::Exit()
{
	if (m_eCurrentPlayerID == CPlayer::SION)
	{
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_StrikeFinish"), LEVEL_STATIC, TEXT("dddd"))))
			return;

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
	for (auto& iter : m_pEffects)
	{
		if (iter != nullptr && iter->Get_PreDead())
			iter = nullptr;
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
	__super::Exit();
}