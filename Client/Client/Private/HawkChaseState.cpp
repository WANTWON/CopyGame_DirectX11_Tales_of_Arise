#include "stdafx.h"

#include "HawkChaseState.h"
#include "GameInstance.h"
#include "HawkIdleState.h"

using namespace Hawk;

CChaseState::CChaseState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CChaseState::Tick(_float fTimeDelta)
{
	m_fTarget_Distance = Find_BattleTarget();


	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CHawkState * CChaseState::LateTick(_float fTimeDelta)
{
	

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);


	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	if (m_fTarget_Distance >= 8.f)
		return new CIdleState(m_pOwner, CHawkState::FIELD_STATE_ID::STATE_TURN_R);
	

	return nullptr;
}

void CChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_RUN);
}

void CChaseState::Exit()
{

}