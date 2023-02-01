#include "stdafx.h"
#include "RinwellMoveState.h"
#include "RinwellAttackState.h"

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
	case Client::STATETYPE_MAIN:
		m_fTarget_Distance = Find_Target(4);
		Move(fTimeDelta);
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
	case Client::STATETYPE_MAIN:
	{
		_float fDistance = XMVectorGetX(XMVector3Length(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vTargetPosition));
		if (fDistance < 5.f)
		{
			m_iChangeCount++;
			m_ChangedDir = true;
			return new CMoveState(m_pOwner, STATETYPE_END, m_iChangeCount);
		}
		break;
	}
	case Client::STATETYPE_END:
	{
		if (m_bIsAnimationFinished)
		{
			if(m_iChangeCount >= 5)
				return new CAttackState(m_pOwner, STATETYPE_MAIN);

			else
				return new CMoveState(m_pOwner, STATETYPE_MAIN, m_iChangeCount);
		}
		break;
	}	
	default:
		break;
	}

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

void CMoveState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	switch (m_eStateType)
	{
	case Client::STATETYPE_MAIN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH);
		break;
	case Client::STATETYPE_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH_BRAKE_000);
		break;
	default:
		break;
	}

	m_fTarget_Distance = Find_ActiveTarget();
	_vector vRandomPos = XMVectorSet((rand() % 30) + 50, 0.f, (rand() % 30) + 50, 0.f);
	m_vTargetPosition = vRandomPos;
}

void CMoveState::Exit()
{
	if (m_eStateType == STATETYPE_END)
	{
		
	}
}

void CMoveState::Move(_float fTimeDelta)
{
	
	CTransform* pRinwellTransform = m_pOwner->Get_Transform();
	
	_vector vTargetDir = XMVector3Normalize(m_vTargetPosition - pRinwellTransform->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(pRinwellTransform->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (fDot < 0.7f)
		pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);

	m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta, m_pOwner->Get_Navigation());
	

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
