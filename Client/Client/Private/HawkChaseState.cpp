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

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();


	return nullptr;
}

CHawkState * CChaseState::LateTick(_float fTimeDelta)
{
	

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	if (m_fTarget_Distance >= 5.f)
		return new CIdleState(m_pOwner);
	

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