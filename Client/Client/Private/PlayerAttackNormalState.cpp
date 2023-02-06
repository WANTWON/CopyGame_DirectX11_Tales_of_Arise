#include "stdafx.h"

#include "PlayerAttackNormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "PlayerSkillState.h"
#include "PlayerJumpState.h"
#include "BattleManager.h"

using namespace Player;

CAttackNormalState::CAttackNormalState(CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
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
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
			{
				switch (m_eStateId)
				{
				case Client::CPlayerState::STATE_NORMAL_ATTACK1:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						//EventInput();

						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
					if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
					{
						if (GetKeyState(VK_LBUTTON) < 0)
							m_bIsStateEvent = true;

						else if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						getchar();
					}
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();

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
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK2:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK3:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
					break;
				case Client::CPlayerState::STATE_NORMAL_ATTACK4:
					if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
						dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
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
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			if (!m_bIsFly)
				return new CAttackNormalState(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK4, m_fStartHeight, m_fTime);
			break;
		}
	}

	if (0 != m_iSkillEvent)
	{
		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_iSkillEvent)
			{
			case 1:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
				break;
			case 2:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
				break;
			case 3:
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
				break;
			}
		}
	}

	if (m_bIsAnimationFinished)
	{
		/*if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime);
		else*/
			return new CIdleState(m_pOwner);
	}

	return nullptr;
}

CPlayerState * CAttackNormalState::EventInput(void)
{
	return nullptr;
}

void CAttackNormalState::Enter()
{
	__super::Enter();

	if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
	{
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_2);
				break;
			}
		}
		else
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
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_6);
				break;
			}
		}
	}
	else if (CPlayer::SION == m_pOwner->Get_PlayerID())
	{
		if (m_bIsFly)
		{

		}
		else
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
	}

	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	_float4 fTargetPos;
	XMStoreFloat4(&fTargetPos, pBattleMgr->Get_LackonMonster()->Get_TransformState(CTransform::STATE_TRANSLATION));

	fTargetPos.y = m_pOwner->Get_Transform()->Get_World4x4().m[3][1];

	_vector		vLook = XMLoadFloat4(&fTargetPos) - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	
	m_pOwner->Get_Transform()->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_RIGHT));
	m_pOwner->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_LOOK));

	RELEASE_INSTANCE(CBattleManager);
}

void CAttackNormalState::Exit()
{
	__super::Exit();
}