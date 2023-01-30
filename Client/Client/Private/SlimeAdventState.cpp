#include "stdafx.h"

#include "SlimeAdventState.h"
#include "GameInstance.h"
#include "SlimeIdleState.h"
#include "SlimeChaseState.h"

using namespace Slime;

CAdventState::CAdventState(CSlime* pSlime)
{
	m_pOwner = pSlime;

}

CSlimeState * CAdventState::AI_Behaviour(_float fTimeDelta)
{


	return nullptr;
}

CSlimeState * CAdventState::Tick(_float fTimeDelta)
{
	Find_Target();

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	//m_pOwner->Check_Navigation();

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation, vecRotation;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix(&vecTranslation, &vecRotation);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), vecRotation, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	return nullptr;
}

CSlimeState * CAdventState::LateTick(_float fTimeDelta)
{
	if (m_pTarget)
		return new CChaseState(m_pOwner);

	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CSlimeState::FIELD_STATE_ID::STATE_ADVENT);
	}


	return nullptr;
}

void CAdventState::Enter()
{
	//m_eStateId = STATE_ID::STATE_MOVE;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSlime::ANIM::ADVENT);


}

void CAdventState::Exit()
{

}