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

CPlayer_RinwellSkillAttack_State::CPlayer_RinwellSkillAttack_State(CPlayer * pPlayer, STATE_ID eStateType, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;

	m_fStartHeight = fStartHeight;
	m_fTime = fTime;
}

CPlayerState * CPlayer_RinwellSkillAttack_State::HandleInput(void)
{
	return nullptr;
}


CPlayerState * CPlayer_RinwellSkillAttack_State::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (CSion::ANIM::BTL_ATTACK_TRESVENTOS)
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		else
			m_pOwner->Check_Navigation_Jump();
	}
	else
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
		else
			m_pOwner->Check_Navigation();
	}


	if (!m_pBlastEffect.empty())
	{
		for (auto& iter : m_pBlastEffect)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;

			if (iter != nullptr)
			{
				CParticleSystem* pParticleSystem = dynamic_cast<CParticleSystem*>(iter);
				if (pParticleSystem != nullptr)
					pParticleSystem->Set_Stop(true);
			}
		}
	}


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
						if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == (CRinwell::ANIM::BTL_ATTACK_FUATU))
						{
							CBullet::BULLETDESC BulletDesc;
							CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
							if (pTarget == nullptr)
								pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
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
						
						CBullet::BULLETDESC BulletDesc;


						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::METEOR;

						BulletDesc.vTargetDir = { -0.3f, -1.f, -0.1f, 0.f };


						for (int i = 0; i < 14; ++i)
						{
							BulletDesc.fVelocity = 2.f + ((_float)(rand() % 20 + 1))*0.1f;
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


						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fDeadTime = 3.f;
						BulletDesc.eBulletType = CRinwellSkills::BANGJEON;
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 5.f);
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
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType )
				{

					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
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
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));


						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fVelocity = 5.f;
						BulletDesc.fDeadTime = 10.f;
						BulletDesc.eBulletType = CRinwellSkills::HOlY_RANCE;
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



	return nullptr;
}

CPlayerState * CPlayer_RinwellSkillAttack_State::LateTick(_float fTimeDelta)
{
	if (m_pOwner->Get_Model()->Get_CurrentAnimIndex() == CRinwell::ANIM::BTL_MAGIC_START && m_bIsAnimationFinished)
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_EMIT);
		return nullptr;
	}

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
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime, CJumpState::JUMP_BATTLE);
		else
			return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);

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
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_TRESVENTOS);

			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Jump_E_Fix.wav"), SOUND_EFFECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_Jump_E.wav"), SOUND_EFFECT, 0.5f);

			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_TRESVENTUS);
			CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("SionTresShot.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());

			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
			//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET);
			break;
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			//				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_THUNDER_BOLT);
			break;
		}
	}
	else
	{
		switch (m_eStateId)
		{
		case Client::CPlayerState::STATE_SKILL_ATTACK_E:
		{
			/* Make Effect */
			/*_vector vOffset = { 0.f,3.f,0.f,0.f };
			_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			_vector vLook = XMVector3Normalize(m_pOwner->Get_TransformState(CTransform::STATE::STATE_LOOK));
			_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();*/
			//mWorldMatrix.r[3] = vLocation + vOffset + vLook*2.f;
			//m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("MagnaRayStart.dat"), mWorldMatrix);

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_FUATU);//마그나
			/*CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_E.wav"), SOUND_EFFECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_E.wav"), SOUND_EFFECT, 0.5f);*/
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GALEFORCE);


			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK_R:
		{
			///* Make Effect */
			//_vector vOffset = { 0.f,3.f,0.f,0.f };
			//_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
			//_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
			//mWorldMatrix.r[3] = vLocation + vOffset;
			//m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("GravitasField.dat"), mWorldMatrix);


			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI);
			/*CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_R.wav"), SOUND_EFFECT, 0.5f);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_R.wav"), SOUND_EFFECT, 0.5f);*/
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_THUNDERFIELD);
			break;
		}
		case Client::CPlayerState::STATE_SKILL_ATTACK_F:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START); // 메테오
			break;
			/*dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_METEOR);
			break;*/

		case Client::CPlayerState::STATE_SKILL_ATTACK4:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_ATTACK_HOUDEN); //얼음떨구기
			dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);

			break;

		case Client::CPlayerState::STATE_SKILL_ATTACK5:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START); // 비가우수수
		//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_ATTACK_CRESCENT_BULLET); // 비가우수수
			break;
		//	dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
		//	CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Ctrl_R.wav"), SOUND_EFFECT, 0.5f);
		//	CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_Ctrl_R.wav"), SOUND_EFFECT, 0.5f);
		//	break;

		case Client::CPlayerState::STATE_SKILL_ATTACK6:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::BTL_MAGIC_START); // 비가우수수

			//dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_AQUARUINA);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillSound_Ctrl_R.wav"), SOUND_EFFECT, 0.5f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("SionSkillVoice_Ctrl_R.wav"), SOUND_EFFECT, 0.5f);
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

void CPlayer_RinwellSkillAttack_State::Exit(void)
{
	__super::Exit();

	CGameInstance::Get_Instance()->StopSound(SOUND_EFFECT);
}
