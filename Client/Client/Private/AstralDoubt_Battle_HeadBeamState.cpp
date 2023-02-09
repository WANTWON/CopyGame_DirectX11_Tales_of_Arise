#include "stdafx.h"

#include "AstralDoubt_Battle_HeadBeamState.h"
#include "GameInstance.h"


using namespace Astral_Doubt;

CBattle_HeadBeamState::CBattle_HeadBeamState(CAstralDoubt* pAstralDoubt, FIELD_STATE_ID ePreState)
{
	m_pOwner = pAstralDoubt;
	m_ePreState_Id = ePreState;

	m_fTimeDeltaAcc = 0;
	m_fIdleTime = ((rand() % 4000 + 1000) *0.001f)*((rand() % 100) * 0.01f);
}

CAstralDoubt_State * CBattle_HeadBeamState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CBattle_HeadBeamState::Tick(_float fTimeDelta)
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

CAstralDoubt_State * CBattle_HeadBeamState::LateTick(_float fTimeDelta)
{

	m_pOwner->Check_Navigation();

	m_fTimeDeltaAcc += fTimeDelta;



	return nullptr;
}

void CBattle_HeadBeamState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_HEAD_BEAM);
}

void CBattle_HeadBeamState::Exit()
{

}