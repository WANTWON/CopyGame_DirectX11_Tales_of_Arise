#include "stdafx.h"
#include "..\Public\IceWolfBattle_DeadState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "Ice_Wolf.h"

using namespace IceWolf;

CBattle_DeadState::CBattle_DeadState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	m_pOwner->Check_Navigation();
	return nullptr;
}

CIceWolfState * CBattle_DeadState::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	if (false == m_bDeadAnimFinish)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
		
	}
	return nullptr;
}

CIceWolfState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	


	if(m_bIsAnimationFinished && false == m_bDeadAnimFinish)
	{


		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DEAD);
		m_pOwner->Get_Model()->Play_Animation(2.55f, false);
		m_pOwner->Set_Dissolve();

		m_bDeadAnimFinish = true;
		
		
		//m_pOwner->Get_Model()->Play_Animation(2.5f, false);
	}

	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;
	
	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_DEAD);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_DeadState::Exit()
{
	m_fIdleAttackTimer = 0.f;
	
}



