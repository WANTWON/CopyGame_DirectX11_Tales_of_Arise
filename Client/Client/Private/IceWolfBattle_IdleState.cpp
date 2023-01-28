#include "stdafx.h"
#include "..\Public\IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CBattle_IdleState::CBattle_IdleState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_IdleState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CBattle_IdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	
	//Find_BattleTarget();
	//m_pOwner->Check_Navigation();


	return nullptr;
}

CIceWolfState * CBattle_IdleState::LateTick(_float fTimeDelta)
{
	srand((_uint)time(NULL));
	m_iRand = rand() % 1;

	//_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	
	m_fRedayAttackTimer += fTimeDelta;

	if (m_fRedayAttackTimer >= 2.5f )
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_RunState(m_pOwner);
			break;
		/*case 1:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);
			break;*/
			/*case 2:
			return new CBattle_HowLingState(m_pOwner);
			break;*/
		default:
			break;
		}
	}

	return nullptr;
}

void CBattle_IdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_IDLE);
}

void CBattle_IdleState::Exit()
{
	
	
}



