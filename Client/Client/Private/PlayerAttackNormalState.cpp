#include "stdafx.h"

#include "PlayerAttackNormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"

using namespace Player;

CAttackNormalState::CAttackNormalState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CAttackNormalState::HandleInput()
{
	return nullptr;
}

CPlayerState * CAttackNormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), &matRootNode);
	
	if (!m_bIsAnimationFinished)
	{
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, (matRootNode * m_pOwner->Get_AnimTransform()->Get_WorldMatrix()).r[3]);

		m_pOwner->Check_Navigation();
	}
	
	return nullptr;
}

CPlayerState * CAttackNormalState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		//m_pOwner->Get_AnimTransform()->Set_State(CTransform::STATE_TRANSLATION, m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		return new CIdleState(m_pOwner);
	}

	return nullptr;
}

void CAttackNormalState::Enter()
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CPlayer::ANIM::ANIM_ATTACK_NORMAL_0);
}

void CAttackNormalState::Exit()
{
	_vector AnimTransAddValue = (m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION) - XMVectorSetW(matRootNode.r[3], 0.f));
	_vector AnimTransform = m_pOwner->Get_AnimTransform()->Get_State(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_AnimTransform()->Set_State(CTransform::STATE_TRANSLATION, AnimTransAddValue);
	m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, AnimTransAddValue);
}