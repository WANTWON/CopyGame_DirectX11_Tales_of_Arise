#include "stdafx.h"

#include "PlayerAttack2NormalState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "Weapon.h"

using namespace Player;

CAttack2NormalState::CAttack2NormalState(CPlayer * pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CAttack2NormalState::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CAttack2NormalState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.5f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	if (!m_bIsAnimationFinished)
	{
		_matrix RootMatrix = m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN");

		_matrix ChangeMatrix = RootMatrix * m_StartMatrix;
		ChangeMatrix.r[0] = XMVector4Normalize(ChangeMatrix.r[0]);
		ChangeMatrix.r[1] = XMVector4Normalize(ChangeMatrix.r[1]);
		ChangeMatrix.r[2] = XMVector4Normalize(ChangeMatrix.r[2]);

		m_pOwner->Get_Transform()->Sliding_Anim(ChangeMatrix, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

	vector<EVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (EVENT_COLLIDER == pEvent.iEventType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->On_Collider();
			
		}
		else
		{
			if (EVENT_COLLIDER == pEvent.iEventType)
				dynamic_cast<CWeapon*>(m_pOwner->Get_Parts(0))->Off_Collider();
		}
	}

	return nullptr;
}

CPlayerState * CAttack2NormalState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner);
	}

	return nullptr;
}

void CAttack2NormalState::Enter(void)
{
	m_eStateId = STATE_ID::STATE_ATTACK;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_ATTACK_NORMAL_1);

	m_StartMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
}

void CAttack2NormalState::Exit(void)
{
	__super::Exit();
}
