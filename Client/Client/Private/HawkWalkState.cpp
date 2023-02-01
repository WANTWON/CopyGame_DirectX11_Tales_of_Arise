#include "stdafx.h"

#include "HawkWalkState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkTurnR_State.h"
#include "HawkTrigger_TurnState.h"
#include "HawkBraveState.h"
#include "HawkIdleState.h"
using namespace Hawk;

CWalkState::CWalkState(CHawk* pIceWolf, _bool bTriggerTurn)
{
	m_pOwner = pIceWolf;

	m_bTriggerTurn = bTriggerTurn;
	m_fTimeDletaAcc = 0;
	m_fMoveTime = ((rand() % 10000 + 6000) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CWalkState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CWalkState::Tick(_float fTimeDelta)
{

	Find_Target();

	m_pOwner->Check_Navigation();
	

	

	return nullptr;
}

CHawkState * CWalkState::LateTick(_float fTimeDelta)
{
	
	//나의 트리거 박스랑 충돌안했을떄
	CBaseObj* pTrigger = m_pOwner->Get_Trigger();

	if (pTrigger != nullptr && m_pOwner->Get_Collider()->Collision(pTrigger->Get_Collider()) == false)
	{
		//돌게하고

		if (false == m_bTriggerTurn)
			return new CTrigger_TurnState(m_pOwner);

		// 그 트리거 박스의 위치 방향으로 이동하는 상태를 세팅한다.

		else
		{
			_vector vPosition = pTrigger->Get_TransformState(CTransform::STATE_TRANSLATION);
			m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());
			m_pOwner->Get_Transform()->LookAt(vPosition);

		}
	}

	//수정 코드 
	else
	{
		m_bTriggerTurn = false;

		m_fTimeDletaAcc += fTimeDelta;

		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

		if (m_pTarget)
			return new CChaseState(m_pOwner);

		else if (m_fTimeDletaAcc > m_fMoveTime)
		{
			switch (rand() % 4)
			{
			case 0:
				return new CIdleState(m_pOwner);
			case 1:
				return new CTurnR_State(m_pOwner);
			case 2:
				return new CBraveState(m_pOwner);



			default:
				break;
			}
		}
	}

	return nullptr;
}

void CWalkState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::MOVE_WALK_F);
}

void CWalkState::Exit()
{

}