#include "stdafx.h"
#include "..\Public\Player_SionNormalAttack_State.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"
//#include "Effect.h"
#include "EffectMesh.h"
#include "PlayerJumpState.h"
#include "BattleManager.h"
#include "SionSkills.h"
#include "Monster.h"
#include "Player_SionSkillAttack.h"

using namespace Player;

CPlayer_SionNormalAttack_State::CPlayer_SionNormalAttack_State(CPlayer* pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;

	if (m_fStartHeight != 0.f)
		m_pOwner->On_IsFly();
}

CPlayerState * CPlayer_SionNormalAttack_State::HandleInput()
{
	return nullptr;
}

CPlayerState * CPlayer_SionNormalAttack_State::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;
	if (CBattleManager::Get_Instance()->Get_LackonMonster() == nullptr)
		return nullptr;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		if (!m_bIsFly)
			m_pOwner->Check_Navigation();
	}

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{

						CBaseObj * pTarget = nullptr;
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 40.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
						{
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						}
						else
						{
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						}
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						else if (pTarget == nullptr )
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//รั น฿ป็ 
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.13f);
						m_fEventStart = pEvent.fStartTime;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 3;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
					}
				}
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

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = nullptr;
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 40.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
						{
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						}
						else
						{
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						}
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						else if (pTarget == nullptr)
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//รั น฿ป็ 
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.13f);
						m_fEventStart = pEvent.fStartTime;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 2;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
					}
				}
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

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = nullptr;
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 40.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
						{
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						}
						else
						{
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						}
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						else if (pTarget == nullptr)
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//รั น฿ป็ 
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.13f);
						m_fEventStart = pEvent.fStartTime;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 2;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
					}


				}


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

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = nullptr;
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 40.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
						{
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						}
						else
						{
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						}
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						else if (pTarget == nullptr)
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//รั น฿ป็ 
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.13f);
						m_fEventStart = pEvent.fStartTime;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 2;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
					}


				}


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

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}


				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{

					CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT_SION);
					if (!m_bSoundStart)
					{
						m_bSoundStart = true;
						CGameInstance::Get_Instance()->PlaySounds(TEXT("SionReload.wav"), SOUND_EFFECT, 0.1f);


					}
				}
				break;

			case Client::CPlayerState::STATE_NORMAL_ATTACK5:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = nullptr;
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 40.f;
						BulletDesc.eBulletType = CSionSkills::NORMALATTACK;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
						{
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						}
						else
						{
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						}
						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						else if (pTarget == nullptr)
							BulletDesc.vTargetPosition = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
							(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_SionSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//รั น฿ป็ 
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Sion_Shot.wav"), SOUND_EFFECT, 0.07f);
						m_fEventStart = pEvent.fStartTime;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir * 2;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("SionNormalBulletBlast.dat"), mWorldMatrix);
					}


				}


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

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
						m_iSkillEvent = 4;

					else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
						m_iSkillEvent = 5;

					getchar();
				}

				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{
					
					CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT_SION);
					if (!m_bSoundStart)
					{
						m_bSoundStart = true;
						CGameInstance::Get_Instance()->PlaySounds(TEXT("SionReload.wav"), SOUND_EFFECT, 0.3f);


					}
				}
				break;

			}
		}
	}
		
	

	return nullptr;
}

CPlayerState * CPlayer_SionNormalAttack_State::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			if (!m_bIsFly)
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK4, m_fStartHeight, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			if (!m_bIsFly)
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK5, m_fStartHeight, m_fTime);
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
				return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				break;
			case 2:
				return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				break;
			case 3:
				return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			case 4:
				return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				break;
			case 5:
				return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				break;
			}
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CPlayer_SionNormalAttack_State::EventInput(void)
{
	return nullptr;
}

void CPlayer_SionNormalAttack_State::Enter()
{
	__super::Enter();

	/*if (CPlayer::ALPHEN == m_pOwner->Get_PlayerID())
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
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_3);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK5:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_AIR_4);
				break;


			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_0);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack1.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack2.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_8);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack3.wav"), SOUND_EFFECT, 1.0f);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_6);
				CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Normal_Attack4.wav"), SOUND_EFFECT, 1.0f);
				break;
			}
		}
	}*/
	/*else if (CPlayer::SION == m_pOwner->Get_PlayerID())
	{*/
		if (m_bIsFly)
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_2);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_3);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK5:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_AIR_4);
				break;
			}
		}
		else
		{
			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_NORMAL_ATTACK1:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_0);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK2:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_1);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK3:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_2);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_3);
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK5:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_NORMAL_4);
				break;
			}
		}
	//}

	CBattleManager* pBattleMgr = GET_INSTANCE(CBattleManager);

	CBaseObj* pTarget = pBattleMgr->Get_LackonMonster();

	if (nullptr != pTarget)
	{
		_float4 fTargetPos;
		XMStoreFloat4(&fTargetPos, pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

		fTargetPos.y = m_pOwner->Get_Transform()->Get_World4x4().m[3][1];

		_vector		vLook = XMLoadFloat4(&fTargetPos) - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		_vector		vRight = XMVector3Cross(vAxisY, vLook);

		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_RIGHT));
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) * m_pOwner->Get_Transform()->Get_Scale(CTransform::STATE_LOOK));
	}

	RELEASE_INSTANCE(CBattleManager);
}

void CPlayer_SionNormalAttack_State::Exit()
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}

