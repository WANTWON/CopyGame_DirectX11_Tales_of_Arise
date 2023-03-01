#include "stdafx.h"
#include "RinwellIdleState.h"
#include "RinwellSkillState.h"
#include "RinwellMoveState.h"
#include "RinwellSkills.h"
#include "Effect.h"

using namespace AiRinwell;

CSkillState::CSkillState(CAiRinwell* pRinwell, _uint eType)
{
	m_pOwner = pRinwell;
	m_eSkillType = eType;
}

CRinwellState * CSkillState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	m_pOwner->Check_Navigation();

	return nullptr;
}

CRinwellState * CSkillState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
		return new CRinwellIdleState(m_pOwner, 3.5f);

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

void CSkillState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

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
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MAGIC_START);

		Find_Target(rand() % 4);
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
			BulletDesc.fVelocity = 4.f + ((_float)(rand() % 20 + 1))*0.1f;
			_vector pos = { (_float)(rand() % 40 + 40) , 12.f + i*2.5f , (_float)(rand() % 40 + 40), 1.f };
			BulletDesc.vInitPositon = pos;

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;
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
			m_pOwner->Get_Transform()->Set_Rotation({ 0.f,0.f + i * 30.f , 0.f });
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
	}
}

void CSkillState::Exit()
{
}

