#include "stdafx.h"
#include "..\Public\IceWolfTurnLeftState.h"
#include "IceWolfIdleState.h"
#include "IceWolfWalkState.h"
#include "IceWolfTurnRightState.h"
#include "IceWolfChaseState.h"
#include "IceWolfBattle_SomerSaultState.h"
#include "IceWolfBattle_RunState.h"

using namespace IceWolf;

CTurnLeftState::CTurnLeftState(class CIce_Wolf* pIceWolf)
{
	m_pOwner = pIceWolf;
}

CIceWolfState * CTurnLeftState::AI_Behaviour(_float fTimeDelta)
{
	Find_Target();
	return nullptr;
}

CIceWolfState * CTurnLeftState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();
	Find_Target();

	
	

	return nullptr;
}

CIceWolfState * CTurnLeftState::LateTick(_float fTimeDelta)
{
	//m_iRadian = rand() % 10;
	//

	//if (m_pTarget)
	//{
	//	_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	//	m_pOwner->Get_Transform()->LookAt(vTargetPosition);
	//	m_pOwner->Get_Transform()->Go_PosTarget(fTimeDelta, vTargetPosition);
	//	return new CChaseState(m_pOwner);


	//}

	//else if (m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex())))
	//{
	//	m_iRand = rand() % 2;
	//	if (m_iRand == 0)
	//	{
	//		return new CWalkFrontState(m_pOwner);
	//	}

	//	else if (m_iRand == 1)
	//	{
	//		return new CIdleState(m_pOwner);
	//	}
	//}
	if (m_bIsAnimationFinished)
	{

		return new CBattle_RunState(m_pOwner);
	}

	//else
	//{
	//	_matrix RootMatrix = XMLoadFloat4x4(&m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone"));

	//	m_pOwner->Get_Transform()->Sliding_Anim(RootMatrix * m_StartMatrix, m_pOwner->Get_Navigation());

	//	m_pOwner->Check_Navigation();
	//}
	

	
	
	return nullptr;
}

void CTurnLeftState::Enter()
{
	m_eStateId = STATE_ID::STATE_MOVE;

	
	m_pOwner->Get_Model()->Set_NextAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_TURN_LEFT);
}

void CTurnLeftState::Exit()
{
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.5f);
}



