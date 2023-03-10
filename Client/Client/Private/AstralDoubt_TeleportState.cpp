#include "stdafx.h"
#include "..\Public\AstralDoubt_TeleportState.h"

#include "AstralDoubt_Battle_IdleState.h"
#include "Effect.h"

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

		m_pActiveTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		_vector vDir = XMVector3Normalize(m_pActiveTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION));
		vDir = XMVectorSetY(vDir, 0.f);
		if(XMVectorGetX(XMVectorEqual(vDir, XMVectorSet(0.f,0.f,0.f,0.f))) != 1)
			m_pOwner->Get_Transform()->Go_PosDir(fTimeDelta, vDir);
		m_pOwner->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION), 20.f));

		if (5.f < m_fTime)
		{
			m_eStateId = STATE_TELEPORT_DOWN;
			Enter();
		}

		if (!m_pEffects.empty())
		{
			for (auto& iter : m_pEffects)
			{
				if (iter == nullptr)
					continue;
				_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
				mWorldMatrix.r[3] = XMVectorSetY(mWorldMatrix.r[3], 0.5f);
				iter->Set_State(CTransform::STATE_TRANSLATION, mWorldMatrix.r[3]);
			}
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
	{
		/* Make Effect */
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = XMVectorSetY(mWorldMatrix.r[3], 0.5f);
		m_pEffects = CEffect::PlayEffectAtLocation(TEXT("BosstelePort.dat"), mWorldMatrix);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_RISE_LOOP);
		break;
	}
	case Client::CAstralDoubt_State::STATE_TELEPORT_DOWN:
	{
		/* Make Effect */
		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = XMVectorSetY(mWorldMatrix.r[3], 0.5f);
		CEffect::PlayEffectAtLocation(TEXT("BosstelePort.dat"), mWorldMatrix);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_DESCEND_LOOP);
		break;
	}
	case Client::CAstralDoubt_State::STATE_TELEPORT_END:
	{
		if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_ShakingMode(true, 2.f, 0.2f);

		_matrix mWorldMatrix = m_pOwner->Get_Transform()->Get_WorldMatrix();
		mWorldMatrix.r[3] = XMVectorSetY(mWorldMatrix.r[3], 0.5f);
		CEffect::PlayEffectAtLocation(TEXT("BosstelePort.dat"), mWorldMatrix);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAstralDoubt::ANIM::ATTACK_JUMP_SHOOT_DROP_END);
		break;
	}
		
	}
}

void CAstralDoubt_TeleportState::Exit()
{
}
