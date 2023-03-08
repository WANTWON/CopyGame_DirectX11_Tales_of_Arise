#include "stdafx.h"
#include "..\Public\AstralDoubt_TeleportState.h"

#include "AstralDoubt_Battle_IdleState.h"

using namespace Astral_Doubt;

CAstralDoubt_TeleportState::CAstralDoubt_TeleportState(CAstralDoubt * pAstral_Doubt)
{
	m_pOwner = pAstral_Doubt;
	m_eStateId = STATE_TELEPORT_START;
}

CAstralDoubt_State * CAstralDoubt_TeleportState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CAstralDoubt_State * CAstralDoubt_TeleportState::Tick(_float fTimeDelta)
{
	if ((STATE_TELEPORT_UP == m_eStateId) || (STATE_TELEPORT_END == m_eStateId))
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone", 0.f);
	else
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		if ((STATE_TELEPORT_UP == m_eStateId) || (STATE_TELEPORT_DOWN == m_eStateId))
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.03f), fRotationRadian, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CAstralDoubt_State * CAstralDoubt_TeleportState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CAstralDoubt_State::STATE_TELEPORT_START:
			m_eStateId = STATE_TELEPORT_UP;
			break;
		case Client::CAstralDoubt_State::STATE_TELEPORT_END:
			return new CBattle_IdleState(m_pOwner, CAstralDoubt_State::STATE_ID::STATE_SPEARMULTI);
			break;
		}

		Enter();
	}

	if (STATE_TELEPORT_UP == m_eStateId)
	{
		m_fTime += fTimeDelta;

		if (5.f < m_fTime)
		{
			Find_Target();
			m_pOwner->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetY(m_pActiveTarget->Get_TransformState(CTransform::STATE_TRANSLATION), 20.f));
			m_eStateId = STATE_TELEPORT_DOWN;
			Enter();
		}
	}

	else if (m_pOwner->Check_Navigation_Jump() && (STATE_TELEPORT_DOWN == m_eStateId))
	{
		m_eStateId = STATE_TELEPORT_END;
		Enter();
	}

	return nullptr;
}

void CAstralDoubt_TeleportState::Enter()
{
	switch (m_eStateId)
	{
	case Client::CAstralDoubt_State::STATE_TELEPORT_START:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_START);
		break;
	case Client::CAstralDoubt_State::STATE_TELEPORT_UP:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_RISE_LOOP);
		break;
	case Client::CAstralDoubt_State::STATE_TELEPORT_DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_DESCEND_LOOP);
		break;
	case Client::CAstralDoubt_State::STATE_TELEPORT_END:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_END);
		break;
	}
}

void CAstralDoubt_TeleportState::Exit()
{
}
