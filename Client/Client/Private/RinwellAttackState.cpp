#include "stdafx.h"
#include "RinwellAttackState.h"
#include "RinwellMoveState.h"
#include "RinwellSkills.h"
#include "Effect.h"

using namespace AiRinwell;

CAttackState::CAttackState(CAiRinwell* pRinwell, STATETYPE eType)
{
	m_pOwner = pRinwell;
	m_eStateType = eType;
}


CRinwellState * CAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		break;
	case Client::STATETYPE_MAIN:
		break;
	case Client::STATETYPE_END:
		break;
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CRinwellState * CAttackState::LateTick(_float fTimeDelta)
{
	
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateType)
		{
		case Client::STATETYPE_START:
			return new CAttackState(m_pOwner, STATETYPE_MAIN);
		case Client::STATETYPE_MAIN:
			return new CAttackState(m_pOwner, STATETYPE_END);
		case Client::STATETYPE_END:
			return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
		}
	}

	return nullptr;
}

void CAttackState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp*0.5f)
		m_bAirMove = true;
	else
		m_bAirMove = false;

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
	{
		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset;

		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;

		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBegin.dat"), mWorldMatrix);
		if (!m_bAirMove)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_0);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_0);
		break;
	}
	case Client::STATETYPE_MAIN:
	{
		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//Shot Ice Rock
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 10.f;
		BulletDesc.fDeadTime = 5.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);



		if (!m_bAirMove)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_1);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_1);
		break;
	}
	case Client::STATETYPE_END:
	{
		Find_Target(rand() % 4);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//Shot Ice Rock
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 10.f;
		BulletDesc.fDeadTime = 5.f;

		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + BulletDesc.vTargetDir;
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = vLocation;
	

		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("PhotonFlashBlast.dat"), mWorldMatrix);


		if (!m_bAirMove)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_2);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_AIR_2);
		break;
	}
	}

	
	

}


void CAttackState::Exit()
{
	

}

