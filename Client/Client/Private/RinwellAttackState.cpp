#include "stdafx.h"
#include "RinwellIdleState.h"
#include "RinwellAttackState.h"
#include "RinwellMoveState.h"
#include "RinwellSkills.h"
#include "RinwellTurnState.h"
#include "Effect.h"

using namespace AiRinwell;

CAttackState::CAttackState(CAiRinwell* pRinwell, STATETYPE eType)
{
	m_pOwner = pRinwell;
	m_eStateType = eType;
}


CRinwellState * CAttackState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 4.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	
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
			return new CRinwellTurnState(m_pOwner, STATE_ATTACK, STATETYPE_MAIN);
		case Client::STATETYPE_MAIN:
			return new CRinwellTurnState(m_pOwner, STATE_ATTACK, STATETYPE_END);
		case Client::STATETYPE_END:
			return new CRinwellIdleState(m_pOwner, 0.5f);
		}
	}

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

void CAttackState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		m_bAirMove = true;
	else
		m_bAirMove = false;

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
	{
		Find_Target(0);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + (m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * 1.f); 

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
		Find_Target(0);
		m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		//Shot Ice Rock
		CBullet::BULLETDESC BulletDesc;
		BulletDesc.eCollisionGroup = MONSTER;
		BulletDesc.eBulletType = CRinwellSkills::PHOTON_FLASH;
		BulletDesc.vInitPositon = XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 3.f);
		BulletDesc.vTargetPosition = m_vTargetPosition;
		BulletDesc.vTargetDir = XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		BulletDesc.fVelocity = 8.f;
		BulletDesc.fDeadTime = 5.f;
		BulletDesc.pOwner = m_pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		{
			BulletDesc.vTargetDir = XMVector3TransformNormal(BulletDesc.vTargetDir, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(15.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;

			BulletDesc.vTargetDir = XMVector3TransformNormal(XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(345.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;
		}

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + (m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * 1.f);
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
		BulletDesc.fVelocity = 8.f;
		BulletDesc.fDeadTime = 5.f;
		BulletDesc.pOwner = m_pOwner;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
			return;

		if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.75f)
		{
			BulletDesc.vTargetDir = XMVector3TransformNormal(BulletDesc.vTargetDir, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(15.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;

			BulletDesc.vTargetDir = XMVector3TransformNormal(XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(345.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;
		}

		if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		{
			BulletDesc.vTargetDir = XMVector3TransformNormal(BulletDesc.vTargetDir, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-15.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;

			BulletDesc.vTargetDir = XMVector3TransformNormal(XMVector3Normalize(m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(30.f)));

			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_RinwellSkills"), LEVEL_BATTLE, TEXT("Layer_Bullet"), &BulletDesc)))
				return;
		}

		_vector vOffset = XMVectorSet(0.f, 3.f, 0.f, 0.f);
		_vector vLocation = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION) + vOffset + (m_pOwner->Get_TransformState(CTransform::STATE_LOOK) * 1.f);
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

void CAttackState::Move(_float fTimeDelta)
{
	CTransform* pRinwellTransform = m_pOwner->Get_Transform();

	_vector vTargetDir = XMVector3Normalize(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pRinwellTransform->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(pRinwellTransform->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (m_bAirMove)
	{
		if (fDot < 0.95f)
			pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.5f);
	}
	else
	{
		if (fDot < 0.7f)
			pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 1.f);
	}

	m_pOwner->Check_Navigation();
}

