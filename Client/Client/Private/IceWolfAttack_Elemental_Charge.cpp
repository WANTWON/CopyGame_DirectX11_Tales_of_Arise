#include "stdafx.h"
#include "..\Public\IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfAttackNormalState.h"

using namespace IceWolf;

CAttack_Elemental_Charge::CAttack_Elemental_Charge(class CIce_Wolf* pIceWolf, STATE_ID eStateType)
{
	m_eStateId_Charge = eStateType;
	m_pOwner = pIceWolf;
	
}

CIceWolfState * CAttack_Elemental_Charge::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::Tick(_float fTimeDelta)
{
	
	m_fTarget_Distance = Find_BattleTarget();
	
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	return nullptr;
}

CIceWolfState * CAttack_Elemental_Charge::LateTick(_float fTimeDelta)
{

	m_iRand = rand() % 3;

	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId_Charge)
		{
		case Client::CIceWolfState::STATE_CHARGE_START:
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_LOOP);
			break;

		case Client::CIceWolfState::STATE_CHARGE_LOOP:
			if (3 < m_fTarget_Distance)
				return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_LOOP);
			else if (3 >= m_fTarget_Distance)
				return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
			break;

		case Client::CIceWolfState::STATE_CHARGE_END:
			return new CBattle_IdleState(m_pOwner);
			break;
		}
	}

	else
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

		m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}







	
		/*if (STATE_CHARGE_START == m_eStateId_Charge && m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())))
		{
			return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_LOOP);
		}

		else if (STATE_CHARGE_LOOP == m_eStateId_Charge)
		{		
			m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

				if (10 < m_fTarget_Distance)
				m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
				

				if(10 >= m_fTarget_Distance)
				return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_END);
		
		}

		else if (STATE_CHARGE_END == m_eStateId_Charge)
		{
			m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
			
			if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
				return new CBattle_WalkState(m_pOwner);

			else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
				return new CBattle_IdleState(m_pOwner);

			else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
				return new CAttackNormalState(m_pOwner);

			else m_fIdleAttackTimer += fTimeDelta;

		}*/
	
	return nullptr;
}



void CAttack_Elemental_Charge::Enter()
{
	switch (m_eStateId_Charge)
	{
	case STATE_CHARGE_START:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_START);
		break;

	case STATE_CHARGE_LOOP:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_LOOP);
		break;

	case STATE_CHARGE_END:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_ATTACK_ELEMENTAL_CHARGE_END);
		break;
	}


	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAttack_Elemental_Charge::Exit()
{
	m_fIdleAttackTimer = 0.f;
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
}



