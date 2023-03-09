#include "stdafx.h"
#include "..\Public\Player_RinwellNormalAttack_State.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"
//#include "Effect.h"
#include "EffectMesh.h"
#include "PlayerJumpState.h"
#include "BattleManager.h"
#include "RinwellSkills.h"
#include "Monster.h"
#include "RinwellSkillState.h"

using namespace Player;

CPlayer_RinwellNormalAttack_State::CPlayer_RinwellNormalAttack_State(CPlayer* pPlayer, STATE_ID eStateType, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fTime = fTime;
}

CPlayerState * CPlayer_RinwellNormalAttack_State::HandleInput()
{
	return nullptr;
}

CPlayerState * CPlayer_RinwellNormalAttack_State::Tick(_float fTimeDelta)
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

						_vector m_vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						//Shot Ice Rock
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						BulletDesc.vTargetPosition = m_vTargetPosition;
						BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
						BulletDesc.fVelocity = 5.f;
						BulletDesc.fDeadTime = 5.f;
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);
						m_fEventStart = pEvent.fStartTime;
					

						
					}
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					//EventInput();

					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
					{
						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
							m_iSkillEvent = 4;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
							m_iSkillEvent = 5;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
							m_iSkillEvent = 6;
					}

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

						_vector m_vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						//Shot Ice Rock
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						BulletDesc.vTargetPosition = m_vTargetPosition;
						BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
						BulletDesc.fVelocity = 5.f;
						BulletDesc.fDeadTime = 5.f;
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);
						m_fEventStart = pEvent.fStartTime;
					}
				}
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
					{
						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
							m_iSkillEvent = 4;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
							m_iSkillEvent = 5;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
							m_iSkillEvent = 6;
					}

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

						_vector m_vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
						//Shot Ice Rock
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
						BulletDesc.vTargetPosition = m_vTargetPosition;
						BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
						BulletDesc.fVelocity = 5.f;
						BulletDesc.fDeadTime = 5.f;
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
						_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
						_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
						mWorldMatrix.r[3] = vLocation;
						m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);
						m_fEventStart = pEvent.fStartTime;
					}


				}


				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;
					if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
					{

						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
							m_iSkillEvent = 4;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
							m_iSkillEvent = 5;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
							m_iSkillEvent = 6;
					}

					getchar();
				}
				break;
			case Client::CPlayerState::STATE_NORMAL_ATTACK4:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							CBaseObj * pTarget = nullptr;
							CBullet::BULLETDESC BulletDesc;
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

							_vector m_vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							//Shot Ice Rock
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
							BulletDesc.vTargetPosition = m_vTargetPosition;
							BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
							BulletDesc.fVelocity = 5.f;
							BulletDesc.fDeadTime = 5.f;
							BulletDesc.pOwner = m_pOwner;
							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;

							_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
							_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
							_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
							mWorldMatrix.r[3] = vLocation;
							m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);
							m_fEventStart = pEvent.fStartTime;
						}
					}


				}


				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{
					if (GetKeyState(VK_LBUTTON) < 0)
						m_bIsStateEvent = true;

					if (floor(m_pOwner->Get_Info().fCurrentMp) >= 1)
					{
						if (GetKeyState('E') < 0)
							m_iSkillEvent = 1;

						else if (GetKeyState('R') < 0)
							m_iSkillEvent = 2;

						else if (GetKeyState('F') < 0)
							m_iSkillEvent = 3;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
							m_iSkillEvent = 4;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
							m_iSkillEvent = 5;

						else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
							m_iSkillEvent = 6;
					}

					getchar();
				}


				if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
				{

					/*CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT_SION);
					if (!m_bSoundStart)
					{
						m_bSoundStart = true;
						CGameInstance::Get_Instance()->PlaySounds(TEXT("SionReload.wav"), SOUND_EFFECT, 0.1f);


					}*/
				}
				break;

			

			}
		}
	}



	return nullptr;
}

CPlayerState * CPlayer_RinwellNormalAttack_State::LateTick(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (m_bIsStateEvent)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK2, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK3, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			if (!m_bIsFly)
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK4, m_fTime);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			if (!m_bIsFly)
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK5, m_fTime);
			break;
		}
	}

	if (0 != m_iSkillEvent)
	{
		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			/*switch (m_iSkillEvent)
			{
			case 1:
				return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);
				break;
			case 2:
				return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);
				break;
			case 3:
				return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			case 4:
				return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4);
				break;
			case 5:
				return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK5);
				break;*/
		}
	}

	

	if (m_bIsAnimationFinished)
	{
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(false, -4.f, 0.5f, 5.f, 8.f);
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
	}

	return nullptr;
}

CPlayerState * CPlayer_RinwellNormalAttack_State::EventInput(void)
{
	return nullptr;
}

void CPlayer_RinwellNormalAttack_State::Enter()
{
	__super::Enter();

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack1.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack2.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_2);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack3.wav"), SOUND_EFFECT, 1.0f);
			break;

		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_NORMAL_ATTACK1:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_0);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack1.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK2:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_1);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack2.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK3:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_2);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack3.wav"), SOUND_EFFECT, 1.0f);
			break;
		case Client::CPlayerState::STATE_NORMAL_ATTACK4:
			pCamera->Set_Zoom(true, -6.f, 0.5f, 5.f, 8.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_NORMAL_3);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Normal_Attack4.wav"), SOUND_EFFECT, 1.0f);
			break;
		}
	}


	CBattleManager* pBattleMgr = CBattleManager::Get_Instance();

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

	m_pOwner->Set_Manarecover(false);
}

void CPlayer_RinwellNormalAttack_State::Exit()
{
	__super::Exit();

}

