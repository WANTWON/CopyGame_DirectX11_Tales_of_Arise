#include "stdafx.h"
#include "..\Public\IceWolfBattle_Damage_LargeB_State.h"
#include "IceWolfBattle_WalkState.h"
#include "IceWolfAttackNormalState.h"
#include "IceWolfAttack_Elemental_Charge.h"
#include "IceWolfBattle_BackStepState.h"
#include "IceWolfBattle_SomerSaultState.h"

using namespace IceWolf;

CBattle_Damage_LargeB_State::CBattle_Damage_LargeB_State(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CBattle_Damage_LargeB_State::AI_Behaviour(_float fTimeDelta)
{
	Find_BattleTarget();
	m_pOwner->Check_Navigation();
	return nullptr;
}

CIceWolfState * CBattle_Damage_LargeB_State::Tick(_float fTimeDelta)
{
	AI_Behaviour(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	return nullptr;
}

CIceWolfState * CBattle_Damage_LargeB_State::LateTick(_float fTimeDelta)
{
	m_iRand = rand() % 2;
	
	
	if (m_bIsAnimationFinished)
	{
		switch (m_iRand)
		{
		case 0:
			return new CBattle_BackStepState(m_pOwner);
			break;

		case 1:
			return new CBattle_SomerSaultState(m_pOwner);
			break;

		default:
			break;
		}
	}
	//switch (m_eDamageAnim)
	//{
	//case 0:
	//	m_eDamageAnim = ANIM_DAMAGE_LARGE_B;
	//	break;

	//case 1:
	//	m_eDamageAnim = ANIM_DAMAGE_LARGE_F;
	//	break;

	//case 2:
	//	m_eDamageAnim = ANIM_DAMAGE_LARGE_L;
	//	break;

	//case 3:
	//	m_eDamageAnim = ANIM_DAMAGE_LARGE_R;
	//	break;

	//case 4:
	//	m_eDamageAnim = ANIM_DAMAGE_LOOP;
	//	break;

	//case 5:
	//	m_eDamageAnim = ANIM_DAMAGE_SMALL_B;
	//	break;

	//case 6:
	//	m_eDamageAnim = ANIM_DAMAGE_SMALL_F;
	//	break;

	//case 7:
	//	m_eDamageAnim = ANIM_DAMAGE_SMALL_L;
	//	break;

	//case 8:
	//	m_eDamageAnim = ANIM_DAMAGE_SMALL_R;
	//	break;

	//default:
	//	break;
	//}
	/*if (m_fIdleAttackTimer > 3.f && m_iRand == 0)
		return new CBattle_WalkState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 1)
		return new CBattle_Damage_LargeB_State(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 2)
		return new CAttackNormalState(m_pOwner);

	else if (m_fIdleAttackTimer > 3.f && m_iRand == 3)
		return new CAttack_Elemental_Charge(m_pOwner, STATE_ID::STATE_CHARGE_START);

	else m_fIdleAttackTimer += fTimeDelta;*/

	return nullptr;
}

void CBattle_Damage_LargeB_State::Enter()
{
	m_iRand = rand() % 9;

	m_eStateId = STATE_ID::STATE_HIT;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DAMAGE_SMALL_B);
	//switch (m_iRand)
	//{
	//case 0:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_LARGE_B);
	//	break;
	//case 1:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_LARGE_F);
	//	break;
	//case 2:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_LARGE_L);
	//	break;
	//case 3:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_LARGE_R);
	//	break;
	//case 4:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_LOOP);
	//	break;
	//case 5:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_SMALL_B);
	//	break;
	//case 6:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_SMALL_F);
	//	break;
	//case 7:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_SMALL_L);
	//	break;
	//case 8:
	//	m_pOwner->Get_Model()->Set_CurrentAnimIndex(Client::CIce_Wolf::ANIM_DAMAGE_SMALL_R);
	//	break;
	//default:
	//	break;
	//}
	
}

void CBattle_Damage_LargeB_State::Exit()
{
	m_fIdleAttackTimer = 0.f;
	m_iRand = false;
	//m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 1.f), 2.f);
}



