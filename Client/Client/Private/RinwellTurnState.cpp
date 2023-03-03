#include "stdafx.h"
#include "..\Public\RinwellTurnState.h"

#include "RinwellAttackState.h"
#include "RinwellSkillState.h"

using namespace AiRinwell;

CRinwellTurnState::CRinwellTurnState(CAiRinwell * pRinwell, STATE_ID eStateID, STATETYPE eStateType)
{
	m_pOwner = pRinwell;
	m_eNextID = eStateID;
	m_eNextStateType = eStateType;
}

CRinwellState * CRinwellTurnState::Tick(_float fTimeDelta)
{
	Find_Target(0);

	_vector vCurScale, vCurRotQuat, vCurPos;
	_matrix OwnerWorld = m_pOwner->Get_Transform()->Get_WorldMatrix();
	XMMatrixDecompose(&vCurScale, &vCurRotQuat, &vCurPos, OwnerWorld);
	XMQuaternionNormalize(vCurRotQuat);

	_vector vOwnerPos = OwnerWorld.r[3];
	_vector vTargetExceptY = XMVectorSetY(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION), XMVectorGetY(vOwnerPos));
	_vector vLook = XMVector4Normalize(vTargetExceptY - vOwnerPos);
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

	return nullptr;
}

CRinwellState * CRinwellTurnState::LateTick(_float fTimeDelta)
{
	if (1.f <= m_fRatio)
	{
		switch (m_eNextID)
		{
		case Client::CRinwellState::STATE_MOVE:
			break;
		case Client::CRinwellState::STATE_ATTACK:
			return new CAttackState(m_pOwner, m_eNextStateType);
		case Client::CRinwellState::STATE_SKILL:
			return new CSkillState(m_pOwner, m_pOwner->Get_SkillIndex());
			break;
		}
	}

	return nullptr;
}

void CRinwellTurnState::Enter(void)
{
	m_eStateId = STATE_ID::STATE_TURN;
}

void CRinwellTurnState::Exit(void)
{
	m_fRatio = 0.f;
}
