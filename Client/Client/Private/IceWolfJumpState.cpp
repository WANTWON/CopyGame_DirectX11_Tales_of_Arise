#include "stdafx.h"
#include "..\Public\IceWolfJumpState.h"

#include "IceWolfBattle_Damage_LargeB_State.h"

using namespace IceWolf;

CIceWolfJumpState::CIceWolfJumpState(CIce_Wolf * pIceWolf, _float fTime)
{
	m_pOwner = pIceWolf;
	m_fTime = fTime;
}

CIceWolfState * CIceWolfJumpState::AI_Behaviour(_float fTimeDelta)
{
	return nullptr;
}

CIceWolfState * CIceWolfJumpState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "ABone");
	
	Move(fTimeDelta);

	return nullptr;
}

CIceWolfState * CIceWolfJumpState::LateTick(_float fTimeDelta)
{
	if (m_pOwner->Check_Navigation_Jump())
		return new CBattle_Damage_LargeB_State(m_pOwner, STATE_DOWN, HIT_DOWN, XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.f);

	return nullptr;
}

void CIceWolfJumpState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_DAMAGE_AIR_LOOP);

	if (0.f != m_fTime)
	{
		m_fStartHeight = XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)) + (((7.f * 7.f) * (5.f / 23.f)) * -0.5f);
		m_fTime = 7.f / 4.6f;
	}
}

void CIceWolfJumpState::Exit()
{
}

void CIceWolfJumpState::Move(_float fTimeDelta)
{
	m_fTime += fTimeDelta * 3.5f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 7.0f, 4.6f, m_fStartHeight);
}
