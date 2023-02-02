#include "stdafx.h"

#include "PlayerRunState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerJumpState.h"
#include "PlayerSkillState.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir, _bool isDash)
	: m_eDirection(eDir)
	, m_bIsDash(isDash)
{
	m_pOwner = pPlayer;
}

CPlayerState * CRunState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
			return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1);

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (pGameInstance->Key_Down(DIK_E))//stopstopstop
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
			else if (pGameInstance->Key_Down(DIK_R))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
			else if (pGameInstance->Key_Down(DIK_F))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
		}
	}
	
	if (pGameInstance->Key_Down(DIK_LCONTROL) && !m_bIsFly)
		return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
		m_eDirection = DIR_STRAIGHT_LEFT;
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
		m_eDirection = DIR_STRAIGHT_RIGHT;
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
		m_eDirection = DIR_BACKWARD_LEFT;
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
		m_eDirection = DIR_BACKWARD_RIGHT;
	else if (pGameInstance->Key_Pressing(DIK_A))
		m_eDirection = DIR_LEFT;
	else if (pGameInstance->Key_Pressing(DIK_D))
		m_eDirection = DIR_RIGHT;
	else if (pGameInstance->Key_Pressing(DIK_S))
		m_eDirection = DIR_BACKWARD;
	else if (pGameInstance->Key_Pressing(DIK_W))
		m_eDirection = DIR_STRAIGHT;
	else
		return new CIdleState(m_pOwner);

	if (pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (!m_bIsDash)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
		
		m_bIsDash = true;
	}
	else
	{
		if (m_bIsDash)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);

		m_bIsDash = false;
	}

	return nullptr;
}

CPlayerState * CRunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	
	Move(fTimeDelta);

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CRunState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CRunState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_RUN;

	if (m_bIsDash)
	{
		switch (m_pOwner->Get_PlayerID())
		{
		case CPlayer::ALPHEN:
			if (LEVEL_BATTLE != m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);

			break;
		}
	}
	else
	{
		switch (m_pOwner->Get_PlayerID())
		{
		case CPlayer::ALPHEN:
			if (LEVEL_BATTLE == m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_RUN);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);

			break;
		}
	}
}

void CRunState::Exit()
{
}

void CRunState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix CameraMatrix = XMLoadFloat4x4(&pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW));

	switch (m_eDirection)
	{
	case DIR_STRAIGHT_LEFT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));
		break;
	case DIR_STRAIGHT_RIGHT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));
		break;
	case DIR_BACKWARD_LEFT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.f));
		break;
	case DIR_BACKWARD_RIGHT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(135.f));
		break;
	case DIR_STRAIGHT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f));
		break;
	case DIR_BACKWARD:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
		break;
	case DIR_LEFT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		break;
	case DIR_RIGHT:
		CameraMatrix *= XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		break;
	}

	CTransform* pPlayerTransform = m_pOwner->Get_Transform();
	
	_float4x4 CameraFloat;
	XMStoreFloat4x4(&CameraFloat, CameraMatrix);

	_float4x4 PlayerFloat = pPlayerTransform->Get_World4x4();

	_vector vLook = XMVectorLerp(XMVectorSet(PlayerFloat.m[2][0], 0.f, PlayerFloat.m[2][2], 0.f), XMVectorSet(CameraFloat.m[2][0], 0.f, CameraFloat.m[2][2], 0.f), 0.35f);
	_float4 LookFloat;
	XMStoreFloat4(&LookFloat, vLook);

	_vector vPlayerLook = XMVectorSet(LookFloat.x, PlayerFloat.m[2][1], LookFloat.z, 0.f);
	
	_vector vRight = XMVector4Normalize(XMVector3Cross(pPlayerTransform->Get_State(CTransform::STATE_UP), vPlayerLook)) * pPlayerTransform->Get_Scale(CTransform::STATE_RIGHT);

	pPlayerTransform->Set_State(CTransform::STATE_LOOK, XMVector4Normalize(vPlayerLook) * pPlayerTransform->Get_Scale(CTransform::STATE_LOOK));
	pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vRight);

	_float fCos = XMVectorGetX(XMVector4Dot(pPlayerTransform->Get_State(CTransform::STATE_LOOK), vPlayerLook));

	if (0.85f < fCos)
	{
		_float fTime = fTimeDelta;
		if (m_bIsDash)
			fTime *= 4.f;
		else
			fTime *= 2.f;
		
		m_pOwner->Get_Transform()->Sliding_Straight(fTime, m_pOwner->Get_Navigation());
	}

	RELEASE_INSTANCE(CGameInstance);
}