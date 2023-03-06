#include "stdafx.h"
#include "RinwellIdleState.h"
#include "RinwellSkillState.h"
#include "RinwellStepState.h"
#include "RinwellSkills.h"
#include "RinwellDownState.h"
#include "RinwellDamageState.h"
#include "Effect.h"

using namespace AiRinwell;

CSkillState::CSkillState(CAiRinwell* pRinwell, _uint eType, STATETYPE eStateType)
{
	m_pOwner = pRinwell;
	m_eSkillType = eType;
	m_eStateType = eStateType;
}

CRinwellState * CSkillState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	if (CAiRinwell::ANIM::BTL_ATTACK_HOUDEN)
	{
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;

			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);

			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.015f), fRotationRadian, m_pOwner->Get_Navigation());
		}
	}

	if (!m_pBlastEffects.empty())
	{
		for (auto& iter : m_pBlastEffects)
		{
			if (iter != nullptr && iter->Get_PreDead())
				iter = nullptr;
		}
	}

	if ((CRinwellState::METEOR == m_eSkillType) && (STATETYPE_MAIN == m_eStateType))
		m_fTime += fTimeDelta;

	m_pOwner->Check_Navigation();

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CRinwellState * CSkillState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		if (CRinwellState::METEOR == m_eSkillType)
		{
			switch (m_eStateType)
			{
			case Client::STATETYPE_START:
				m_eStateType = STATETYPE_MAIN;
				Enter();
				break;
			case Client::STATETYPE_END:
				return new CRinwellDownState(m_pOwner, 10.f);
				break;
			}
		}
		else
		{
			switch (m_eSkillType)
			{
			case CRinwellState::THUNDERFIELD:
			case CRinwellState::GALEFORCE:
			case CRinwellState::BANGJEON:
				return new CRinwellStepState(m_pOwner, STATETYPE_START);
				break;
			case CRinwellState::HOLY:
				return new CRinwellIdleState(m_pOwner, 0.5f);
				break;
			}
		}
	}

	if ((CRinwellState::METEOR == m_eSkillType) && (STATETYPE_MAIN == m_eStateType) && (10.f < m_fTime))
	{
		m_eStateType = STATETYPE_END;
		m_fTime = 0.f;
		Enter();
	}

	return nullptr;
}

void CSkillState::Enter()
{
	m_eStateId = STATE_ID::STATE_SKILL;

	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp*0.5f)
		m_bAirMove = true;
	else
		m_bAirMove = false;

	switch (m_eSkillType)
	{
	case Client::CRinwellState::PHOTONFLASH:
		break;
	case Client::CRinwellState::METEOR:
	{
		switch (m_eStateType)
		{
		case Client::STATETYPE_START:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MAGIC_START);
			break;
		case Client::STATETYPE_MAIN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MAGIC_LOOP);
			break;
		case Client::STATETYPE_END:
		{
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MAGIC_EMIT);

			Find_Target(0);
			m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

			//Bullet
			CBullet::BULLETDESC BulletDesc;
			BulletDesc.eCollisionGroup = MONSTER;
			BulletDesc.eBulletType = CRinwellSkills::METEOR;

			_int XRand = rand() % 2 == 0 ? 1.f : -1.f;
			_int ZRand = rand() % 2 == 0 ? 1.f : -1.f;
			BulletDesc.vTargetDir = { rand() % 6 * 0.1f *XRand, -1.f, rand() % 6 * 0.1f*ZRand, 0.f };

			BulletDesc.vTargetPosition = m_vTargetPosition;
			BulletDesc.pOwner = m_pOwner;

			for (int i = 0; i < 30; ++i)
			{
				BulletDesc.fVelocity = 4.f + ((_float)(rand() % 20 + 1)) * 0.1f;
				_vector pos = { (_float)(rand() % 40 + 40) , 12.f + i * 2.5f , (_float)(rand() % 40 + 40), 1.f };
				BulletDesc.vInitPositon = pos;

				if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
					return;
			}
			break;
		}
		}

		break;
	}
	case Client::CRinwellState::HOLY:
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MAGIC_START);

		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		//Bullet
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::HOLY_RANCE;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 5.f;
		BulletDesc.vInitPositon = XMVectorSetY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION), 15.f);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.fDeadTime = 10.f;
		BulletDesc.pOwner = m_pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		break;
	}
	case Client::CRinwellState::THUNDERFIELD:
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_DENGEKISYOUHEKI);

		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		//Bullet
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::THUNDER_FIELD;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 0.5f;
		BulletDesc.vInitPositon = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.fDeadTime = 6.f;
		BulletDesc.pOwner = m_pOwner;
		for (_uint i = 0; i < 12; ++i)
		{
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f, 0.f + i * 30.f, 0.f });
			BulletDesc.vTargetDir = m_pOwner->Get_TransformState(CTransform::STATE_LOOK);
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;
		}

		break;
	}
	case Client::CRinwellState::GALEFORCE:
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_GALEFORCE);

		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		//Bullet
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::GALE_FORCE;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 10.f;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f) + BulletDesc.vTargetDir*2.f;
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.fDeadTime = 2.f;
		BulletDesc.pOwner = m_pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		break;
	}
	case Client::CRinwellState::BANGJEON:
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_HOUDEN);

		Find_Target(0);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.fDeadTime = 2.f;
		BulletDesc.eBulletType = CRinwellSkills::BANGJEON;
		BulletDesc.pOwner = m_pOwner;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 5.f);
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		_vector vOffset = { 0.f,3.f,0.f,0.f };
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		m_pBlastEffects = CEffect::PlayEffectAtLocation(TEXT("ElecDischargeBegin.dat"), mWorldMatrix);
		CGameInstance::Get_Instance()->PlaySounds(TEXT("RinwellSkillSound_Ctrl_E.mp3"), SOUND_EFFECT, 1.0f);
		break;
	}
	}
}

void CSkillState::Exit()
{
}

