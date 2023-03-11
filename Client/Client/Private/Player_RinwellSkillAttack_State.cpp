#include "stdafx.h"
#include "..\Public\Player_RinwellSkillAttack_State.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"
#include "UI_Skillmessage.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "BattleManager.h"
#include "Bullet.h"
#include "Monster.h"
#include "ParticleSystem.h"
#include "Player_RinwellNormalAttack_State.h"
#include "RinwellSkills.h"



using namespace Player;

CPlayer_RinwellSkillAttack_State::CPlayer_RinwellSkillAttack_State(CPlayer * pPlayer, STATE_ID eStateType, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fTime = fTime;
}

CPlayerState * CPlayer_RinwellSkillAttack_State::HandleInput(void)
{
	return nullptr;
}


CPlayerState * CPlayer_RinwellSkillAttack_State::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation_Jump();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			switch (m_eStateId)
			{
			case Client::CPlayerState::STATE_SKILL_ATTACK_E:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_E_SkillSound.wav"), SOUND_SKILL, 0.5f);
						if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CRinwell::ANIM::BTL_ATTACK_FUATU))
						{
							CBaseObj * pTarget = nullptr;

							CBullet::BULLETDESC BulletDesc;
							if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
								pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							else
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

							CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
							pCamera->Set_Zoom(false, 1.f, 0.1f, 5.f, 7.f);

							BulletDesc.pTarget = pTarget;
							_vector vTargetPosition = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.eBulletType = CRinwellSkills::GALE_FORCE;
							BulletDesc.vTargetDir = XMVector3Normalize(vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
							BulletDesc.fVelocity = 10.f;
							BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + BulletDesc.vTargetDir*2.f;
							BulletDesc.vTargetPosition = vTargetPosition;
							BulletDesc.fDeadTime = 2.f;
							BulletDesc.pOwner = m_pOwner;
							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
							//m_bCollideFinsh = true;
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

				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK_R:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

					if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI))
					{
						if ((m_fEventStart != pEvent.fStartTime))
						{
							CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_R_SkillSound.wav"), SOUND_SKILL, 0.4f);
							CBullet::BULLETDESC BulletDesc;
							BulletDesc.eCollisionGroup = PLAYER;
							BulletDesc.eBulletType = CRinwellSkills::THUNDER_FIELD;


							BulletDesc.fVelocity = 0.5f;
							BulletDesc.vInitPositon = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
							BulletDesc.fDeadTime = 6.f;
							BulletDesc.pOwner = m_pOwner;
							for (_uint i = 0; i < 12; ++i)
							{
								m_pOwner->Get_Transform()->Set_Rotation({ 0.f,0.f + i * 30.f , 0.f });
								BulletDesc.vTargetDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
								if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
									return nullptr;
							}
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

				break;
			case Client::CPlayerState::STATE_SKILL_ATTACK_F:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_F_SkillSound.wav"), SOUND_SKILL, 0.4f);
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_SkillSound_Begin.wav"), SOUND_EFFECT, 0.7f);
						CBullet::BULLETDESC BulletDesc;


						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::METEOR;


						for (int i = 0; i < 7; ++i)
						{
							_int XRand = rand() % 2 == 0 ? 1.f : -1.f;
							_int ZRand = rand() % 2 == 0 ? 1.f : -1.f;
							BulletDesc.vTargetDir = { rand() % 6 * 0.1f *XRand, -1.f, rand() % 6 * 0.1f*ZRand, 0.f };
							BulletDesc.pOwner = m_pOwner;
							BulletDesc.fVelocity = 4.f + ((_float)(rand() % 20 + 1))*0.1f;
							_vector pos = { (_float)(rand() % 40 + 40) , 12.f + i*2.5f , (_float)(rand() % 40 + 40), 1.f };
							BulletDesc.vInitPositon = pos;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
						}
						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_METEORSWARM);
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

			case Client::CPlayerState::STATE_SKILL_ATTACK4:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellCtrl_E_SkillSound.wav"), SOUND_SKILL, 0.5f);

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fDeadTime = 2.f;
						BulletDesc.eBulletType = CRinwellSkills::BANGJEON;
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 0.5f);
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;


						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);
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

			case Client::CPlayerState::STATE_SKILL_ATTACK5:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

					if ((m_fEventStart != pEvent.fStartTime))
					{


						if (CBattleManager::Get_Instance()->IsAllMonsterDead())
							return nullptr;
						CBaseObj * pTarget = nullptr;
						if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillSound.wav"), SOUND_SKILL, 0.4f);
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_SkillSound_Begin.wav"), SOUND_EFFECT, 0.7f);



						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::DIVINE_SABER;



						if (pTarget != nullptr)
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_DIVINESABER);


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

			case Client::CPlayerState::STATE_SKILL_ATTACK6:
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{

					if ((m_fEventStart != pEvent.fStartTime))
					{

						if (CBattleManager::Get_Instance()->IsAllMonsterDead())
							return nullptr;
						CBaseObj * pTarget = nullptr;
						if (CBattleManager::Get_Instance()->Get_LackonMonster() != nullptr)
							pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();

						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillSound.wav"), SOUND_SKILL, 0.5f);

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 5.f;
						BulletDesc.fDeadTime = 10.f;
						BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE;
						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = (pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
							BulletDesc.vInitPositon = XMVectorSetY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION), 15.f);
						}
						else if (pTarget == nullptr)
							BulletDesc.vTargetPosition = CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_HOLYRANCE);
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
			}
		}

	}


	if (!m_pSmokeEffect.empty())
	{
		for (auto& iter : m_pSmokeEffect)
		{
			if (iter != nullptr)
			{
				_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
				iter->Set_State(CTransform::STATE_TRANSLATION, m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
			}
		}
	}

	if (!m_pBlastEffect.empty())
	{
		for (auto& iter : m_pBlastEffect)
		{
			if (iter != nullptr)
			{
				_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
				iter->Set_State(CTransform::STATE_TRANSLATION, m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffset);
			}


		}
	}


	return nullptr;
}

CPlayerState * CPlayer_RinwellSkillAttack_State::LateTick(_float fTimeDelta)
{
	if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == CRinwell::ANIM::BTL_MAGIC_START && m_bIsAnimationFinished)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_EMIT);
		return nullptr;
	}

	//	if(m_pOwner->Get_IsFly() && m_bIsAnimationFinished)


	if (m_bIsStateEvent)
		return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_ID::STATE_NORMAL_ATTACK1);

	if ((0 != m_iSkillEvent) && (floor(m_pOwner->Get_Info().fCurrentMp) > 0))
	{
		switch (m_iSkillEvent)
		{
		case 1:
			return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_E);
			break;
		case 2:
			return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_R);
			break;
		case 3:
			return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_ID::STATE_SKILL_ATTACK_F);
			break;
		case 4:
			return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_ID::STATE_SKILL_ATTACK4);
			break;
		case 5:
			return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_ID::STATE_SKILL_ATTACK5);
			break;
		}
	}

	if (m_bIsAnimationFinished)
	{
		if (m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_MAIN, CJumpState::JUMP_BATTLE, m_fTime);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);

	}



	if (!m_pSmokeEffect.empty())
	{
		for (auto& iter : m_pSmokeEffect)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;

		}
	}

	if (!m_pBlastEffect.empty())
	{
		for (auto& iter : m_pBlastEffect)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;

		}
	}


	return nullptr;
}

void CPlayer_RinwellSkillAttack_State::Enter(void)
{
	__super::Enter();
	m_pOwner->Use_Mana(1.f);

	if (m_bIsFly)
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK4:
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_HOUDEN);

			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true, 0.5f, 0.1f, 6.f, 10.f);

			_vector vOffset = { 0.f,3.f,0.f,0.f };
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeBegin.dat"), mWorldMatrix);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellCtrl_E_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellElecDischarge.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());
			break;
		}

		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
		{

			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true, 1.f, 0.1f, 6.f, 7.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_FUATU);//마그나
			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellGaleForce.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GALEFORCE);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_E_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
		{

			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true, -5.f, 1.f, 3.f, 7.f);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_THUNDERFIELD);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_R_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK_F://Metoeor
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START); // 메테오

			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_F_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellDevineSabor2.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartFlash.dat"), mWorldMatrix);
			m_pSmokeEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartRing.dat"), mWorldMatrix);
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK4: //elecdischarge
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_HOUDEN);


			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true, 1.f, 0.1f, 5.f, 10.f);


			_vector vOffset = { 0.f,3.f,0.f,0.f };
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeBegin.dat"), mWorldMatrix);
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);
			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellElecDischarge.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellCtrl_E_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK5: //devine
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START);
			//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET); 
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_R_SkillVoice.wav"), SOUND_VOICE, 0.7f);

			_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("RinwellDevineSabor.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartFlash.dat"), mWorldMatrix);
			m_pSmokeEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartRing.dat"), mWorldMatrix);
			break;
		}

		case Client::CPlayerState::STATE_SKILL_ATTACK6: //HolyRance
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellSkillSound_Ctrl_F.mp3"), SOUND_EFFECT, 1.0f);
			//dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Rinwell_Ctrl_F_SkillVoice.wav"), SOUND_VOICE, 0.5f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("Magic_StartSound.wav"), SOUND_EFFECT, 1.0f);
			_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE_LOOK)) * 2;
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			mWorldMatrix.r[3] = vLocation;

			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true, -10.f, 0.5f, 3.f, 7.f);

			m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartFlash.dat"), mWorldMatrix);
			m_pSmokeEffect = CEffect::PlayEffectAtLocation(TEXT("RinwellMagicStartRing.dat"), mWorldMatrix);
			break;
		}
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

void CPlayer_RinwellSkillAttack_State::Exit(void)
{
	__super::Exit();

	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_Zoom(false, 3.f, 1.f, 6.f, 10.f);

}
