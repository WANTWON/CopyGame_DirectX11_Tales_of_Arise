#include "stdafx.h"
#include "..\Public\RinwellStepState.h"

#include "RinwellIdleState.h"

using namespace AiRinwell;

CRinwellStepState::CRinwellStepState(CAiRinwell * pRinwell, STATETYPE eStateType)
{
	m_pOwner = pRinwell;
	m_eStateType = eStateType;
}

CRinwellState * CRinwellStepState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	Move(fTimeDelta);

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

	return nullptr;
}

CRinwellState * CRinwellStepState::LateTick(_float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		if (1.f <= m_fRatio)
		{		
			m_eStateType = STATETYPE_END;
			m_fRatio = 0.f;
			Enter();
		}
		break;
	case Client::STATETYPE_END:
		if (1.f <= m_fRatio)
			return new CRinwellIdleState(m_pOwner, 1.f);
		break;
	}

	return nullptr;
}

void CRinwellStepState::Enter(void)
{
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
	{
		if (m_pOwner->Get_AirMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_FALL);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_MOVE_IDLE);

		m_iDistance = (rand() % 15) + 5;

		m_vTargetPos = XMVectorSet(64.f + m_iDistance * cosf(XMConvertToRadians(_float(rand() % 361))), XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)),
			64.f + m_iDistance * sinf(XMConvertToRadians(_float(rand() % 361))), 1.f);
		break;
	}
	case Client::STATETYPE_END:
	{
		if (m_pOwner->Get_AirMode())
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_STEP_AIR);
		else
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_STEP_LAND);

		m_vCurrentPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
		break;
	}
	}
}

void CRinwellStepState::Exit(void)
{
}

void CRinwellStepState::Move(_float fTimeDelta)
{
	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
	{
		_vector vCurScale, vCurRotQuat, vCurPos;
		_matrix OwnerWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
		XMMatrixDecompose(&vCurScale, &vCurRotQuat, &vCurPos, OwnerWorld);
		XMQuaternionNormalize(vCurRotQuat);

		_vector vOwnerPos = OwnerWorld.r[3];
		_vector vLook = XMVector4Normalize(m_vTargetPos - vOwnerPos);
		_vector vRight = XMVector4Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));

		_matrix ToTargetMatrix;
		ToTargetMatrix.r[0] = vRight;
		ToTargetMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		ToTargetMatrix.r[2] = vLook;

		_vector vTargetRotQuat = XMQuaternionNormalize(XMQuaternionRotationMatrix(ToTargetMatrix));

		_vector vRotQuat = XMQuaternionSlerp(vCurRotQuat, vTargetRotQuat, m_fRatio);

		_matrix InterpolationMatrix = XMMatrixScalingFromVector(vCurScale) * XMMatrixRotationQuaternion(vRotQuat) *
			XMMatrixTranslationFromVector(vCurPos);

		m_pOwner->Get_Transform()->Set_WorldMatrix(InterpolationMatrix);

		m_fRatio += 0.25f;
		break;
	}
	case Client::STATETYPE_END:
		_vector vCurrentPos = XMVectorLerp(m_vCurrentPos, m_vTargetPos, m_fRatio);
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCurrentPos);
		m_fRatio += fTimeDelta;
		break;
	}
}
