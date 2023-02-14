#include "stdafx.h"

#include "AstralDoubt_Battle_RushState.h"
#include "GameInstance.h"
#include "AstralDoubt_Battle_WalkState.h"


using namespace Astral_Doubt;

CBattle_RushState::CBattle_RushState(CAstralDoubt* pAstralDoubt, STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fRandTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_RushState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_RushState::Tick(_float fTimeDelta)
{
	/*m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	CBaseObj*	pDamageCauser = m_pOwner->Get_DamageCauser();

	if (pDamageCauser == nullptr)
	{
		if (m_pCurTarget == nullptr)
		{
			m_pCurTarget = m_pOwner->Find_MinDistance_Target();
			if (m_pCurTarget == nullptr)
				return nullptr;

			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}
		else
		{
			m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
		}

	}
	else
	{
		m_pCurTarget = pDamageCauser;
		m_vCurTargetPos = pDamageCauser->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(pDamageCauser);
	}*/

	return nullptr;
}

CAstralDoubt_State * CBattle_RushState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;

	if (m_bIsAnimationFinished)
	{
		if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_START)
			return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_RUSH_LOOP);

		else if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_LOOP)
		{
			return new CBattleWalkState(m_pOwner);
		}

		else
			return new CBattle_RushState(m_pOwner, CAstralDoubt_State::STATE_RUSH_START);
	}



	return nullptr;
}

void CBattle_RushState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	if (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_START)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_LOOP);

	else if  (m_ePreState_Id == CAstralDoubt_State::STATE_RUSH_LOOP)
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_END);

	else
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_SPEAR_RUSH_START);

	
}

void CBattle_RushState::Exit()
{

}