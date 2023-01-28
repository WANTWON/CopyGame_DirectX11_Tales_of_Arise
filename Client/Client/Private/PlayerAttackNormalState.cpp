#include "stdafx.h"

#include "PlayerAttackNormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"

using namespace Player;

CAttackNormalState::CAttackNormalState(CPlayer* pPlayer, STATE_ID eStateType)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
}

CPlayerState * CAttackNormalState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_float fTranslationLength;
		_matrix RotationMatrix;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &fTranslationLength, &RotationMatrix);

		fTranslation.push_back(fTranslationLength);

		m_pOwner->Get_Transform()->Sliding_Anim((fTranslationLength * 0.001f), m_pOwner->Get_Navigation());

		/*_matrix ChangeMatrix = RootMatrix * m_StartMatrix;
		ChangeMatrix.r[0] = XMVector4Normalize(ChangeMatrix.r[0]);
		ChangeMatrix.r[1] = XMVector4Normalize(ChangeMatrix.r[1]);
		ChangeMatrix.r[2] = XMVector4Normalize(ChangeMatrix.r[2]);

		*/

		m_pOwner->Check_Navigation();
	}
	else
	{
		int a = 10;
	}

	vector<EVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (EVENT_STATE == pEvent.iEventType)
					{
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						RELEASE_INSTANCE(CGameInstance);
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (EVENT_STATE == pEvent.iEventType)
					{
						CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						RELEASE_INSTANCE(CGameInstance);
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{

			}
		}
		else
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (EVENT_COLLIDER == pEvent.iEventType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					break;
				}
			}
			else if (CPlayer::SION == m_pOwner->Get_PlayerID())
			{

			}
		}
	}
	
	return nullptr;
}

CPlayerState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	if (m_bIsStateEvent)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3);
			break;
		}
	}

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner);

	return nullptr;
}

void CAttackNormalState::Enter()
{
	if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_8);
			break;
		}
	}
	else if (CPlayer::SION == m_pOwner->Get_PlayerID())
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			break;
		}
	}

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAttackNormalState::Exit()
{
	__super::Exit();
}