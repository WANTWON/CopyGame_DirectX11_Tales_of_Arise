#include "stdafx.h"

#include "IceWolfWalkState.h"
#include "GameInstance.h"
#include "IceWolfIdleState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfChaseState.h"


using namespace IceWolf;

CWalkFrontState::CWalkFrontState(CIce_Wolf* pIceWolf)	
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CWalkFrontState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CIceWolfState * CWalkFrontState::Tick(_float fTimeDelta)
{
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	Find_Target();



	return nullptr;
}

CIceWolfState * CWalkFrontState::LateTick(_float fTimeDelta)
{

		if (m_pTarget)
		{
			_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->LookAt(vTargetPosition);
			m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
			return new CChaseState(m_pOwner);

		}
		else
		{
			m_iRand = rand() % 3;
			if (m_fWalkMoveTimer > 3.f && m_iRand == 0)
				return new CIdleState(m_pOwner);

			else if (m_fWalkMoveTimer > 3.f && m_iRand == 1)
				return new CTurnLeftState(m_pOwner);

			else if (m_fWalkMoveTimer > 3.f && m_iRand == 2)
				return new CTurnRightState(m_pOwner);
			else
				m_fWalkMoveTimer += fTimeDelta;
		}
	

	return nullptr;
}

void CWalkFrontState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_MOVE_WALK_F);
}

void CWalkFrontState::Exit()
{
	m_fWalkMoveTimer = 0.f;
	m_fWalkAttackTimer = 0.f;
}

