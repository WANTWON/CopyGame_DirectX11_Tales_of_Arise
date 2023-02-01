#include "stdafx.h"

#include "HawkBraveState.h"
#include "GameInstance.h"
#include "HawkChaseState.h"
#include "HawkIdleState.h"
#include "HawkWalkState.h"

using namespace Hawk;

CBraveState::CBraveState(CHawk* pIceWolf)
{
	m_pOwner = pIceWolf;

	
	m_fTimeDletaAcc = 0.f;
	m_fBraveTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CHawkState * CBraveState::AI_Behaviour(_float fTimeDelta)
{
	
	return nullptr;
}

CHawkState * CBraveState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	
	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CHawkState * CBraveState::LateTick(_float fTimeDelta)
{

	m_fTimeDletaAcc += fTimeDelta;

	if (
		m_fTimeDletaAcc > m_fBraveTime)
		m_iRand = rand() % 2;


	if (m_pTarget)
		return new CChaseState(m_pOwner);

	else if (m_bIsAnimationFinished)
		switch (m_iRand)
		{
		case 0:
			return new CWalkState(m_pOwner);
		case 1:
			return new CIdleState(m_pOwner);

		default:
			break;
		}
	return nullptr;
}

void CBraveState::Enter()
{

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CHawk::ANIM::ATTACK_BRAVE);
}

void CBraveState::Exit()
{
	
}