#include "stdafx.h"
#include "RinwellAttackState.h"
#include "RinwellMoveState.h"

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

	switch (m_eStateType)
	{
	case Client::STATETYPE_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_0);
		break;
	case Client::STATETYPE_MAIN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_1);
		break;
	case Client::STATETYPE_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::ANIM::BTL_ATTACK_NORMAL_2);
		break;
	}

	Find_Target(rand()%4);
	m_vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//Shot Ice Rock

}

void CAttackState::Exit()
{
	if (m_eStateType == STATETYPE_END)
	{
		
	}
}

