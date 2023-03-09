#include "stdafx.h"
#include "..\Public\BerserkerJumpState.h"

#include "BerserkerBattle_Damage_LargeB_State.h"

using namespace Berserker;

CBerserkerJumpState::CBerserkerJumpState(CBerserker * pBerserker, _float fTime)
{
	m_pOwner = pBerserker;
	m_fTime = fTime;
}

CBerserkerState * CBerserkerJumpState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CBerserkerState * CBerserkerJumpState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");

	Move(fTimeDelta);

	return nullptr;
}

CBerserkerState * CBerserkerJumpState::LateTick(_float fTimeDelta)
{
	if (m_pOwner->Check_Navigation_Jump())
		// return new CBattle_Damage_LargeB_State(m_pOwner, STATE_DOWN, HIT_DOWN, XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.f);

	return nullptr;
}

void CBerserkerJumpState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CBerserker::ANIM::BLOW_DOWN_B);

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((7.f * 7.f) * (5.f / 23.f)) * -0.5f);
		m_fTime = 7.f / 4.6f;
	}
}

void CBerserkerJumpState::Exit()
{
}

void CBerserkerJumpState::Move(_float fTimeDelta)
{
	m_fTime += fTimeDelta * 3.5f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 7.0f, 4.6f, m_fStartHeight);
}
