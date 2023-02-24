#include "stdafx.h"
#include "..\Public\AlphenAttackState.h"

#include "GameInstance.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "PlayerRunState.h"
#include "AlphenSkillState.h"
#include "Effect.h"

using namespace Player;

CAlphenAttackState::CAlphenAttackState(CPlayer * pPlayer, STATE_ID eStateType, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fTime = fTime;
}

CPlayerState * CAlphenAttackState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAlphenAttackState::Tick(_float fTimeDelta)
{
	if ((Client::CPlayerState::STATE_NORMAL_ATTACK3 == m_eStateId) && !m_bIsFly)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.2f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.1f);

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
		else
			m_pOwner->Check_Navigation_Jump();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();
	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
			{
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
			}
			if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				return EventInput();
			if (ANIMEVENT::EVENTTYPE::EVENT_EFFECT == pEvent.eType)
			{
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				_tchar wcEffectName[MAX_PATH] = TEXT("");
			
				if (m_bIsFly)
				{
					switch (m_eStateId)
					{
						case Client::CPlayerState::STATE_NORMAL_ATTACK1:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_1.dat"));
							break;
						case Client::CPlayerState::STATE_NORMAL_ATTACK2:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_2.dat"));
							break;
						case Client::CPlayerState::STATE_NORMAL_ATTACK3:
						{
							if (!strcmp(pEvent.szName, "Jump_Normal_Attack_3"))
								wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_3.dat"));
							if (!strcmp(pEvent.szName, "Jump_Normal_Attack_4"))
							{
								if (!m_bEffectKickSpawned)
								{
									CEffect::PlayEffectAtLocation(TEXT("Jump_Normal_Attack_4.dat"), mWorldMatrix);
									m_bEffectKickSpawned = true;
								}
							}
							break;
						}
						case Client::CPlayerState::STATE_NORMAL_ATTACK4:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Jump_Normal_Attack_5.dat"));
							break;
					}

					if (!m_bEffectSlashSpawned && !m_bEffectKickSpawned)
					{
						CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);

						m_bEffectSlashSpawned = true;
					}
				}
				else
				{
					switch (m_eStateId)
					{
						case Client::CPlayerState::STATE_NORMAL_ATTACK1:
						case Client::CPlayerState::STATE_NORMAL_ATTACK3:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_1.dat"));
							break;
						case Client::CPlayerState::STATE_NORMAL_ATTACK2:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_2.dat"));
							break;
						case Client::CPlayerState::STATE_NORMAL_ATTACK4:
							wcscpy_s(wcEffectName, MAX_PATH, TEXT("Normal_Attack_3.dat"));
							break;
					}

					if (!m_bEffectSlashSpawned)
					{
						if (!wcscmp(wcEffectName, TEXT("Normal_Attack_2.dat")))
							m_SlashEffect = CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);
						else
							CEffect::PlayEffectAtLocation(wcEffectName, mWorldMatrix);

						m_bEffectSlashSpawned = true;
					}	
				}
			}
		}
		else
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
		}
	}

	return nullptr;
}

CPlayerState * CAlphenAttackState::LateTick(_float fTimeDelta)
{
	for (auto& pEffect : m_SlashEffect)
	{
		if (pEffect)
		{
			if (pEffect->Get_PreDead())
				pEffect = nullptr;
			else
			{
				_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + XMVectorSet(0.f, 2.5f, 0.f, 0.f);
				pEffect->Set_State(CTransform::STATE::STATE_TRANSLATION, vPosition);
			}
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CAlphenAttackState::EventInput(void)
{
	if (GetKeyState(VK_LBUTTON) < 0)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_eStateId = STATE_NORMAL_ATTACK2;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_eStateId = STATE_NORMAL_ATTACK3;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_eStateId = STATE_NORMAL_ATTACK4;
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			return new CAlphenAttackState(m_pOwner, STATE_NORMAL_ATTACK1, m_fTime);
			break;
		}

		Enter();
	}

	if (m_bIsFly)
	{
		if (GetKeyState('E') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E, m_fTime);
		}
		else if (GetKeyState('R') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R, m_fTime);
		}
		else if (GetKeyState('F') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F, m_fTime);
		}
	}
	else
	{
		if (GetKeyState('E') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_E);
		}
		else if (GetKeyState('R') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_R);
		}
		else if (GetKeyState('F') < 0)
		{
			if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
				return new CAlphenSkillState(m_pOwner, STATE_SKILL_ATTACK_F);
		}

		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_BACKWARD_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_A))
			return new CRunState(m_pOwner, DIR_LEFT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_D))
			return new CRunState(m_pOwner, DIR_RIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_S))
			return new CRunState(m_pOwner, DIR_BACKWARD, pGameInstance->Key_Pressing(DIK_LSHIFT));
		else if (pGameInstance->Key_Pressing(DIK_W))
			return new CRunState(m_pOwner, DIR_STRAIGHT, pGameInstance->Key_Pressing(DIK_LSHIFT));
	}

	return nullptr;
}

void CAlphenAttackState::Enter()
{
	__super::Enter();

	m_bEffectSlashSpawned = false;
	m_bEffectKickSpawned = false;

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
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_3);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack1.wav"), SOUND_EFFECT, 0.8f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack2.wav"), SOUND_EFFECT, 0.8f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_8);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack3.wav"), SOUND_EFFECT, 0.8f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_6);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack4.wav"), SOUND_EFFECT, 0.8f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("swing_sword_01.wav"), SOUND_EFFECT, 0.8f);
			break;
		}
	}

	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_pOwner->Set_Manarecover(false);
}

void CAlphenAttackState::Exit()
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);

	Safe_Release(m_pAttackCollider);
}

CCollider * CAlphenAttackState::Get_Collider(CCollider::TYPE eType, _float3 vScale, _float3 vRotation, _float3 vPosition)
{
	return nullptr;
}
