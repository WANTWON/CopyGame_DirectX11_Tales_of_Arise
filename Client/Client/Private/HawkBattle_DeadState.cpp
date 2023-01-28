#include "stdafx.h"
#include "HawkBattle_DeadState.h"
#include "HawkIdleState.h"
#include "GameInstance.h"
#include "HawkBattle_BombingState.h"
#include "HawkBattle_ChargeState.h"
#include "HawkBattle_GrabState.h"
#include "HawkBattle_GrabStartState.h"
#include "HawkBattle_RevolveState.h"
#include "HawkBattle_RunState.h"
#include "HawkBattle_Flying_BackState.h"

using namespace Hawk;

CBattle_DeadState::CBattle_DeadState(CHawk* pHawk)
{
	m_pOwner = pHawk;
	
}

CHawkState * CBattle_DeadState::AI_Behaviour(_float fTimeDelta)
{

	return nullptr;


}

CHawkState * CBattle_DeadState::Tick(_float fTimeDelta)
{


	if (false == m_bDeadAnimFinish)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	}

			
	else
	{
	//	_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}


	return nullptr;
}

CHawkState * CBattle_DeadState::LateTick(_float fTimeDelta)
{
	m_fTimeDletaAcc += fTimeDelta;
	

		if (m_bIsAnimationFinished && false == m_bDeadAnimFinish)
		{


//			_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone");

			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::DEAD);
			m_pOwner->Get_Model()->Play_Animation(2.55f, false);


			m_pOwner->Get_Transform()->Go_Up(fTimeDelta * -1.f);

			_vector vHawkPos = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
			_float4 fHawkPos;

			XMStoreFloat4(&fHawkPos, vHawkPos);



			if (m_fTimeDletaAcc >= 5 || fHawkPos.y <= 0)
			{
			m_bDeadAnimFinish = true;
			m_pOwner->Set_Dissolve();
			m_fTimeDletaAcc = 0.f;
			}

		}

	return nullptr;
}

void CBattle_DeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_TAKE_DAMAGE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::DEAD);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CBattle_DeadState::Exit()
{
	m_fIdleMoveTimer = 0.f;
	m_fIdleAttackTimer = 0.f;
	
}