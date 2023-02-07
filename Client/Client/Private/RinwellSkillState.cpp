#include "stdafx.h"
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
	{
		return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
	}

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
	case Client::CRinwellState::GALEFORCE:
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_GALEFORCE);

		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

		//Bullet
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::GALE_FORCE;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 10.f;
		BulletDesc.fDeadTime = 2.f;
		BulletDesc.pOwner = m_pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		break;
	}
	case Client::CRinwellState::SKILL_END:
		break;
	default:
		break;
	}
	
	

}


void CSkillState::Exit()
{
	

}

