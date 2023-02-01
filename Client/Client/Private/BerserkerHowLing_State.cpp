#include "stdafx.h"

#include "BerserkerHowLing_State.h"
#include "BerserkerIdleState.h"
#include "GameInstance.h"
#include "BerserkerChaseState.h"
#include "BerserkerTurnR_State.h"
#include "BerserkerWalkState.h"
using namespace Berserker;

CHowLing_State::CHowLing_State(CBerserker* pIceWolf)
{
	m_pOwner = pIceWolf;

	m_fTimeDletaAcc = 0.f;
	m_fHowlingTime = ((rand() % 10000 + 5000) *0.001f)*((rand() % 100) * 0.01f);
}

CBerserkerState * CHowLing_State::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CBerserkerState * CHowLing_State::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");


	return nullptr;
}

CBerserkerState * CHowLing_State::LateTick(_float fTimeDelta)
{
	//수정코드 

	m_fTimeDletaAcc += fTimeDelta;

	
	if (m_fTimeDletaAcc > m_fHowlingTime)
		m_iRand = rand() % 2;


	if (m_pTarget)
		return new CChaseState(m_pOwner);

	else if (m_bIsAnimationFinished)
		switch (m_iRand)
		{
		case 0:
			return new CWalkState(m_pOwner, FIELD_STATE_END);
		case 1:
			return new CTurnR_State(m_pOwner);
		/*case 2:
			return new CIdle_State(m_pOwner);*/

		default:
			break;
		}






	////이전 코드 
	/*if (m_pTarget)
		return new CChaseState(m_pOwner);

	m_fWalkMoveTimer += fTimeDelta;

	if (m_bIsAnimationFinished)
		return new CIdleState(m_pOwner, CBerserkerState::FIELD_STATE_ID::STATE_HOWLING);
*/
	return nullptr;
}

void CHowLing_State::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::ATTACK_HOWLING);
}

void CHowLing_State::Exit()
{
	
}