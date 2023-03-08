#include "stdafx.h"
#include "..\Public\PlayerDeadState.h"
#include "Level_LawBattle.h"
#include "MonsterLaw.h"
using namespace Player;

CPlayerDeadState::CPlayerDeadState(CPlayer * pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CPlayerDeadState::HandleInput()
{
	return nullptr;
}

CPlayerState * CPlayerDeadState::Tick(_float fTimeDelta)
{
	if (!m_bIsStop)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_INPUT == pEvent.eType)
				m_bIsStop = true;
		}
	}

	return nullptr;
}

CPlayerState * CPlayerDeadState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (CBattleManager::Get_Instance()->Get_IsOneonOneMode() == true)
		{
			CPlayer::PLAYERID ePlayerID = m_pOwner->Get_PlayerID();
			
			switch (ePlayerID)
			{
			case Client::CPlayer::ALPHEN:
				CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::SION);
				CBattleManager::Get_Instance()->Set_LawBattlePhase(1);
				break;
			case Client::CPlayer::SION:
			/*{
				CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::ALPHEN);
				CBattleManager::Get_Instance()->Set_OneonOneMode(false);
				CBattleManager::Get_Instance()->Set_LawBattlePhase(0);
				vector<CPlayer*> pAIList = CPlayerManager::Get_Instance()->Get_AIPlayers();
				for (auto& iter : pAIList)
				{
					iter->Set_HP(iter->Get_Info().fMaxHp);
				}

				CLevel_LawBattle* pLavel = dynamic_cast<CLevel_LawBattle*>(CGameInstance::Get_Instance()->Get_CurrentLevel());
				pLavel->Ready_Layer_Player(nullptr);


				CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
				CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
				pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLEZONE);
				break;
			}*/
				CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::RINWELL);
				CBattleManager::Get_Instance()->Set_LawBattlePhase(2);
				break;
			case Client::CPlayer::RINWELL:
			{
				CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::ALPHEN);
				CBattleManager::Get_Instance()->Set_OneonOneMode(false);
				CBattleManager::Get_Instance()->Set_LawBattlePhase(0);
				dynamic_cast<CMonsterLaw*>(CBattleManager::Get_Instance()->Get_BossMonster())->Set_Daguri(true);
				vector<CPlayer*> pAIList = CPlayerManager::Get_Instance()->Get_AIPlayers();
				for (auto& iter : pAIList)
				{
					iter->Set_HP(iter->Get_Info().fMaxHp);
				}
				
				CLevel_LawBattle* pLavel = dynamic_cast<CLevel_LawBattle*>(CGameInstance::Get_Instance()->Get_CurrentLevel());
				pLavel->Ready_Layer_Player(nullptr);


				CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
				CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
				pCamera->Set_CamMode(CCamera_Dynamic::CAM_BATTLEZONE);
				break;
			}
			default:
				break;
			}
		}
		else
			m_bIsStop = true;


	}
		

	return nullptr;
}

CPlayerState * CPlayerDeadState::EventInput(void)
{
	return nullptr;
}

void CPlayerDeadState::Enter()
{
	__super::Enter();

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DEAD);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_DEAD);
		break;
	case CPlayer::RINWELL:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_DEAD);
		break;
	case CPlayer::LAW:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_DEAD);
		break;
	}
}

void CPlayerDeadState::Exit()
{
	__super::Exit();
}
