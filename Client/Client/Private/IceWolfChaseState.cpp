#include "stdafx.h"
#include "..\Public\IceWolfChaseState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfBattle_WalkState.h"

using namespace IceWolf;

CChaseState::CChaseState(CIce_Wolf * pIceWolf)
{
	m_pOwner = pIceWolf;
}

CHawkState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CChaseState::Tick(_float fTimeDelta)
{
	m_pOwner->Check_Navigation();
	Find_Target();


	if (m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())))

	return nullptr;
}

CHawkState * CChaseState::LateTick(_float fTimeDelta)
{
	if (false == m_bBattleMode)
	{
		if (m_pTarget)
		{
			_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->LookAt(vTargetPosition);
			m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);



			//if (m_fIdleAttackTimer > 1.5f)
			//{
			//	/*if (!m_bHasSpottedTarget)
			//	return new CAggroState();
			//	else
			//	return new CAttackState();*/
			//}aaaa
			//else
			//	m_fIdleAttackTimer += fTimeDelta;
		}
	}

	else if (true == m_bBattleMode)
	{
		return new CBattle_WalkState(m_pOwner);
	}
	return nullptr;
}

void CChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_RUN);
}

void CChaseState::Exit()
{
}
