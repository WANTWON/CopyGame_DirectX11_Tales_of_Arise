#include "stdafx.h"
#include "RinwellMoveState.h"
#include "RinwellAttackState.h"
#include "RinwellSkillState.h"

using namespace AiRinwell;

CMoveState::CMoveState(CAiRinwell* pRinwell, STATETYPE eType, _int iCount)
{
	m_pOwner = pRinwell;
	m_eStateType = eType;
	m_iChangeCount = iCount;
}


CRinwellState * CMoveState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		Find_ActiveTarget();
		m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
		break;
	case Client::STATETYPE_MAIN:
		m_fTarget_Distance = Find_Target(4);
		Move(fTimeDelta, m_iDir);
		break;
	case Client::STATETYPE_END:
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;
			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

			LookAtPlayer(fTimeDelta);
			m_pOwner->Check_Navigation();
		}
		break;
	}

	return nullptr;
}

CRinwellState * CMoveState::LateTick(_float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		break;
	case Client::STATETYPE_MAIN:
	{
		_float fDistance = XMVectorGetX(XMVector3Length(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vTargetPosition));
		if (fDistance < 5.f)
		{
			m_iChangeCount++;
			m_ChangedDir = true;

			if (m_iChangeCount >= 1)
				return new CMoveState(m_pOwner, STATETYPE_END, m_iChangeCount);
			else
				return new CMoveState(m_pOwner, STATETYPE_MAIN, m_iChangeCount);
		}
		break;
	}
	case Client::STATETYPE_END:
	{
		if (m_bIsAnimationFinished)
		{
			_uint iSkill = rand() % 2;

			if (5.f > Find_ActiveTarget())
			{
				switch (iSkill)
				{
				case 0:
					m_pOwner->Set_SkillIndex(CRinwellState::GALEFORCE);
					break;
				case 1:
					m_pOwner->Set_SkillIndex(CRinwellState::THUNDERFIELD);
					break;
				}

				return new CSkillState(m_pOwner, m_pOwner->Get_SkillIndex());
			}
			else
			{
				switch (iSkill)
				{
				case 0:
					m_pOwner->Set_SkillIndex(CRinwellState::PHOTONFLASH);
					return new CAttackState(m_pOwner, STATETYPE_START);
					break;
				case 1:
					m_pOwner->Set_SkillIndex(CRinwellState::HOLY);
					return new CSkillState(m_pOwner, HOLY);
					break;
				}

				return new CSkillState(m_pOwner, m_pOwner->Get_SkillIndex());
			}
		}		
		break;
	}
	}

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	if (m_pOwner->Get_Stats().m_fCurrentHp <= 0)
	{
		HITLAGDESC m_HitLagDesc;
		m_HitLagDesc.bHitLag = false;
		m_HitLagDesc.bLockOnChange = false;
		m_HitLagDesc.bShaking = false;

		m_pOwner->Take_Damage(10, CPlayerManager::Get_Instance()->Get_ActivePlayer(), m_HitLagDesc);
	}
		

	return nullptr;
}

void CMoveState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	if (m_pOwner->Get_Stats().m_fCurrentHp < m_pOwner->Get_Stats().m_fMaxHp * 0.5f)
		m_bAirMove = true;
	else
		m_bAirMove = false;
	if (CGameInstance::Get_Instance()->Get_CurrentLevelIndex() == LEVEL_LAWBATTLE)
		m_bAirMove = true;

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH);
		break;
	case Client::STATETYPE_MAIN:
		if (m_bAirMove)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_STEP_AIR);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH);
		break;
	case Client::STATETYPE_END:
		if (m_bAirMove)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_MAGIC_EMIT_AIR);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH_BRAKE_000);
		break;
	default:
		break;
	}

	m_fTarget_Distance = Find_ActiveTarget();
	_vector vRandomPos = XMVectorSet((rand() % 30) + 50.f, 0.f, (rand() % 30) + 50.f, 0.f);
	m_vTargetPosition = vRandomPos;

	_vector vTargetDir = m_vTargetPosition - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	if (0.f > XMVectorGetY(XMVector3Cross(m_pOwner->Get_TransformState(CTransform::STATE_LOOK), vTargetDir)))
		m_iDir = 1;
	else
		m_iDir = 0;

	if (0.f > XMVectorGetX(XMVector4Dot(m_pOwner->Get_TransformState(CTransform::STATE_LOOK), vTargetDir)))
	{
		if (m_bAirMove)
			m_fTurnDegree = 0.2f;
		else
			m_fTurnDegree = 0.1f;
	}
	else
	{
		if (m_bAirMove)
			m_fTurnDegree = 0.1f;
		else
			m_fTurnDegree = 0.05f;
	}
}

void CMoveState::Exit()
{
}

void CMoveState::Move(_float fTimeDelta, _int iDir)
{
	CTransform* pRinwellTransform = m_pOwner->Get_Transform();

	_vector vTargetDir = XMVector3Normalize(m_vTargetPosition - pRinwellTransform->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(pRinwellTransform->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (m_bAirMove)
	{
		if (fDot < 0.95f)
		{
			switch (m_iDir)
			{
			case 0:
				pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnDegree);
				break;
			case 1:
				pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fTurnDegree);
				break;
			}
		}
		else
		{
			if (!m_bIsAnimationFinished)
				m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 5, m_pOwner->Get_Navigation()); 
		}
	}
	else
	{
		if (fDot < 0.7f)
		{
			switch (m_iDir)
			{
			case 0:
				pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fTurnDegree);
				break;
			case 1:
				pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_fTurnDegree);
				break;
			}
		}

		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();
}

void CMoveState::LookAtPlayer(_float fTimeDelta)
{
	CTransform* pRinwellTransform = m_pOwner->Get_Transform();

	_vector vTargetDir = XMVector3Normalize(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - pRinwellTransform->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(pRinwellTransform->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (fDot < 0.9f)
		pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.2f);
}
