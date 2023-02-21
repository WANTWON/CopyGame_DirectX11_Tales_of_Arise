#include "stdafx.h"
#include "..\Public\AI_Rinwell_SkillState.h"
#include "GameInstance.h"
#include "Weapon.h"
#include "Effect.h"
#include "EffectMesh.h"
#include "AICheckState.h"
#include "Rinwell.h"
#include "UI_Skillmessage.h"
#include "Bullet.h"
#include "RinwellSkills.h"
#include "AIAttackNormalState.h"
#include "AI_DodgeState.h"
#include "AI_JumpState.h"


using namespace AIPlayer;

CAI_Rinwell_SkillState::CAI_Rinwell_SkillState(CPlayer* pPlayer, STATE_ID eStateType, CBaseObj* pTarget ,_float fTime)//, _float fStartHeight, _float fTime)
{
	m_eStateId = eStateType;
	m_pOwner = pPlayer;
	m_fTime = fTime;
	if (nullptr == pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	m_pTarget = pTarget;
	//m_iCurrentAnimIndex = skillindex;
	//m_fStartHeight = fStartHeight;
	//m_fTime = fTime;
}

CAIState * CAI_Rinwell_SkillState::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

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



	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}





	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (m_eStateId == STATE_BANGJEON && m_pOwner->Get_IsFly())
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

	//////////////////////////////////////////////////////////////////////////////////

	if (m_iCurrentAnimIndex == CRinwell::ANIM::BTL_MAGIC_START && m_bIsAnimationFinished)
	{
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_EMIT;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	}


	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{

			switch (m_eStateId)
			{
			case STATE_GALEFORCE:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

						//Bullet
						CBullet::BULLETDESC BulletDesc;
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


				break;
			case STATE_THUNDERFIELD:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

						//Bullet
						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::THUNDER_FIELD;

						BulletDesc.fVelocity = 0.5f;
						BulletDesc.vInitPositon = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
						//BulletDesc.vTargetDir = { -0.3f, -1.f, -0.1f, 0.f };
						//BulletDesc.vTargetPosition = vTargetPosition;
						BulletDesc.fDeadTime = 5.5f;
						BulletDesc.pOwner = m_pOwner;
						for (_uint i = 0; i < 12; ++i)
						{
							m_pOwner->Get_Transform()->Set_Rotation({ 0.f,0.f + i * 30.f , 0.f });
							BulletDesc.vTargetDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
						}
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
						m_bBulletMake = true;
						//m_bCollideFinsh = true;
						m_fEventStart = pEvent.fStartTime;
					}

					//m_fEventStart = pEvent.fStartTime;
				}


				break;
			case STATE_METEOR:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
						CBullet::BULLETDESC BulletDesc;


						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::METEOR;
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vTargetDir = { -0.3f, -1.f, -0.1f, 0.f };


						for (int i = 0; i < 14; ++i)
						{
							BulletDesc.fVelocity = 2.f + ((_float)(rand() % 20 + 1))*0.1f;
							_vector pos = { (_float)(rand() % 40 + 40) , 12.f + i*2.5f , (_float)(rand() % 40 + 40), 1.f };
							BulletDesc.vInitPositon = pos;

							if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
								return nullptr;
						}

						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_METEORSWARM);
						m_fEventStart = pEvent.fStartTime;
					}
				}

				break;

			case STATE_DIVINE_SABER:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime) && !m_bBulletMake)
					{
						CBaseObj * pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
						if (pTarget == nullptr)
							pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.eBulletType = CRinwellSkills::DIVINE_SABER;
						if (pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = pTarget->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
							if (dynamic_cast<CMonster*>(pTarget)->Get_MonsterID() == HAWK)
								BulletDesc.vTargetPosition = XMVectorSetY(BulletDesc.vTargetPosition, XMVectorGetY(BulletDesc.vTargetPosition) - 2.f);

						}
							
						BulletDesc.pOwner = m_pOwner;





						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//m_bCollideFinsh = true;
						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_DIVINESABER);

					}
				}

				break;

			case STATE_HOLYRANCE:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
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
						if (m_pTarget != nullptr)
						{
							BulletDesc.vTargetPosition = (m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
							BulletDesc.vInitPositon = XMVectorSetY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION), 15.f);
						}

							
						else if (m_pTarget == nullptr)
							BulletDesc.vTargetPosition = CBattleManager::Get_Instance()->Get_MinDistance_Monster(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION))->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
						BulletDesc.pOwner = m_pOwner;
						//BulletDesc.vTargetDir = XMVector3Normalize(BulletDesc.vTargetPosition - m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));







						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;
						//m_bCollideFinsh = true;
					 
						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_HOLYRANCE);
					}
				}

				break;

			case STATE_BANGJEON:
				if (ANIMEVENT::EVENTTYPE::EVENT_STATE == pEvent.eType)
				{


					m_bIsStateEvent = true;
					m_bStateFinish = true;

				}
				if (ANIMEVENT::EVENTTYPE::EVENT_COLLIDER == pEvent.eType)
				{
					if ((m_fEventStart != pEvent.fStartTime))
					{
					

						CBullet::BULLETDESC BulletDesc;
						BulletDesc.eCollisionGroup = PLAYER;
						BulletDesc.fDeadTime = 2.f;

						BulletDesc.eBulletType = CRinwellSkills::BANGJEON;
						BulletDesc.pOwner = m_pOwner;
						BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 5.f);
						if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
							return nullptr;

						m_fEventStart = pEvent.fStartTime;
						dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);
					}
				}

				break;
			}

		}

	}


	return nullptr;

}
	


CAIState * CAI_Rinwell_SkillState::LateTick(_float fTimeDelta)
{
	//CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

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



	if (nullptr == m_pTarget)
	{
		m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}
	

	if (m_bIsStateEvent)
	{
		m_fEventStart = -1.f;
		//m_bIsStateEvent = false;
	}
		
	if (m_bIsStateEvent || m_bIsAnimationFinished)
	{
		__super::Exit();
		/*++m_iCurrentAnimIndex;
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);*/
		//
		m_bBulletMake = false;
		m_bIsStateEvent = false;
		//	m_fEventStart = -1.f;
		if (m_bStateFinish)
		{
			if (m_pOwner->Get_IsFly())
				return new CAI_JumpState(m_pOwner, STATETYPE_START, false, m_fTime);


			m_bStateFinish = false;
			if (m_pOwner->Get_Info().fCurrentMp < 1)
			{
				switch (rand() % 3)
				{
				case 0:
					return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
				case 1:
					return new CAICheckState(m_pOwner, m_eStateId);
				case 2:
					return new CAI_DodgeState(m_pOwner, m_pTarget);

				}
			}

			else
			{


				switch (rand() % 8)
				{

				case 0: //Client::CAIState::STATE_NORMAL_ATTACK1:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_GALEFORCE;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_FUATU;
					dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GALEFORCE);
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

					break;
				case 1:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_THUNDERFIELD;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI;
					dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_THUNDERFIELD);
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

					break;
				case 2:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_METEOR;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

					break;
				case 3:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_DIVINE_SABER;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					else
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

					break;
				case 4:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_HOLYRANCE;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
					break;
				case 5:
					__super::Exit();
					m_pOwner->Use_Mana(1.f);
					m_eStateId = STATE_BANGJEON;
					m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_HOUDEN;
					if (nullptr == m_pTarget)
					{
						m_pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
						(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
						m_pOwner->Get_Transform()->LookAtExceptY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
					}
					break;

				case 6:
					return new CAI_JumpState(m_pOwner, STATETYPE_START, true);

				default:
					return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);


				}
			}
		}
	}
			/*else
			{
				switch (rand() % 2)
				{
				case 0:
					return new CAICheckState(m_pOwner, m_eStateId);
					break;

				case 1:
					return new CAI_DodgeState(m_pOwner, m_pTarget);
					break;
				}
			}*/
				
				
		
		
	

	return nullptr;
}

void CAI_Rinwell_SkillState::Enter()
{
	//__super::Enter();
	m_bBulletMake = false;
	//m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_NORMAL_0;
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
	
	switch (m_eStateId)
	{
	case STATE_GALEFORCE:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_FUATU;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_GALEFORCE);
		break;
	case STATE_THUNDERFIELD:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_THUNDERFIELD);
		break;
	case STATE_METEOR:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;

		break;
	case STATE_DIVINE_SABER:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;
		
		break;
	case STATE_HOLYRANCE:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_START;

		break;
	case STATE_BANGJEON:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
		/* Make Effect */
		_vector vOffset = { 0.f,0.f,0.f,0.f };
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		m_pBlastEffect = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeBegin.dat"), mWorldMatrix);
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_ATTACK_HOUDEN;
		dynamic_cast<CUI_Skillmessage*>(CUI_Manager::Get_Instance()->Get_Skill_msg())->Skillmsg_on(CUI_Skillmessage::SKILLNAME::SKILLNAME_BANGJEON);
		break;
	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Set_Manarecover(false);
	m_pOwner->Use_Mana(1.f);

}

void CAI_Rinwell_SkillState::Exit()
{
	__super::Exit();
}