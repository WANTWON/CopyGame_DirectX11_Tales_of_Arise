#include "stdafx.h"
#include "RinwellMoveState.h"


using namespace AiRinwell;

CMoveState::CMoveState(CAiRinwell* pRinwell)
{
	m_pOwner = pRinwell;
}


CRinwellState * CMoveState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	m_fTarget_Distance = Find_Target();

	Move(fTimeDelta);

	m_pOwner->Check_Navigation();

	return nullptr;
}

CRinwellState * CMoveState::LateTick(_float fTimeDelta)
{
	_float fDistance = XMVectorGetX(XMVector3Length(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vTargetPosition));

	if (fDistance < 2.f )
	{
		m_ChangeDirCount++;
		m_ChangedDir = true;

		Find_Target();
		_vector vOffsetPos = XMVectorSet((rand() % 10)*GET_PLUSMINUS, 0.f, (rand() % 10)*GET_PLUSMINUS, 0.f);
		_vector vRandomPos = XMVectorSet((rand() % 40) + 40, 0.f, (rand() % 40) + 40, 0.f);
		//m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffsetPos;
		m_vTargetPosition = vRandomPos;
	}

	if (fDistance > 4.f)
	{
		m_ChangedDir = false;
	}

	return nullptr;
}

void CMoveState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::DASH);

	m_fTarget_Distance = Find_Target();
	_vector vOffsetPos = XMVectorSet((rand() % 10)*GET_PLUSMINUS, 0.f, (rand() % 10)*GET_PLUSMINUS, 0.f);
	_vector vRandomPos = XMVectorSet((rand() % 40) + 40, 0.f, (rand() % 40) + 40, 0.f);
	//m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) + vOffsetPos;
	m_vTargetPosition = vRandomPos;
}

void CMoveState::Exit()
{
}

void CMoveState::Move(_float fTimeDelta)
{
	
	CTransform* pRinwellTransform = m_pOwner->Get_Transform();
	
	_vector vTargetDir = XMVector3Normalize(m_vTargetPosition - pRinwellTransform->Get_State(CTransform::STATE_TRANSLATION));
	_vector vLook = XMVector3Normalize(pRinwellTransform->Get_State(CTransform::STATE_LOOK));

	vLook = XMVectorSetY(vLook, 0.f);
	vTargetDir = XMVectorSetY(vTargetDir, 0.f);

	_float fDot = XMVectorGetX(XMVector3Dot(vTargetDir, vLook));

	if (fDot < 0.5f)
		pRinwellTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.03f);
	else
	{
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta*2.f, m_pOwner->Get_Navigation());
	}

}