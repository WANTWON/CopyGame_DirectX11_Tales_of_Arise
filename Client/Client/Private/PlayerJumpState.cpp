#include "stdafx.h"

#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerSkillState.h"

using namespace Player;

CJumpState::CJumpState(CPlayer* pPlayer, _float fStartHeight, STATETYPE eType, _float fTime)
{
	m_pOwner = pPlayer;
	m_fStartHeight = fStartHeight;
	m_eStateType = eType;
	m_fTime = fTime;
}

CPlayerState * CJumpState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (LEVEL_BATTLE == m_pOwner->Get_Level())
	{
		if (GetKeyState(VK_LBUTTON) < 0)
			return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1, m_fStartHeight, m_fTime);

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			if (pGameInstance->Key_Down(DIK_E))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1, m_fStartHeight, m_fTime);
			else if (pGameInstance->Key_Down(DIK_R))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2, m_fStartHeight, m_fTime);
			else if (pGameInstance->Key_Down(DIK_F))
				return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3, m_fStartHeight, m_fTime);
		}
	}

	if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
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
		m_eDirection = DIR_END;

	return nullptr;
}

CPlayerState * CJumpState::Tick(_float fTimeDelta)
{
	if (m_eStateType != STATETYPE_END)
		Move(fTimeDelta);

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	//m_pOwner->Check_Navigation_Jump();

	return nullptr;
}

CPlayerState * CJumpState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateType)
		{
		case STATETYPE_START:
			return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_MAIN, m_fTime);
		case STATETYPE_END:
			return new CIdleState(m_pOwner);
		}
	}

	if ((CAlphen::ANIM::ANIM_JUMP_LOOP == m_pOwner->Get_Model()->Get_CurrentAnimIndex()) && Check_JumpEnd())
	{
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
		m_eStateType = STATETYPE_END;
	}

	return nullptr;
}

void CJumpState::Enter()
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_JUMP;

	m_pOwner->On_IsFly();

	CPlayer::PLAYERID ePlayerID = m_pOwner->Get_PlayerID();

	switch (m_eStateType)
	{
	case STATETYPE_START:
		switch (ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (LEVEL_BATTLE == m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_JUMP);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_START);
			break;
		case CPlayer::SION:
			if (LEVEL_BATTLE == m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_JUMP);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_START);
			break;
		}
		break;
	case STATETYPE_MAIN:
		switch (ePlayerID)
		{
		case CPlayer::ALPHEN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LOOP);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LOOP);
			break;
		}
		break;
	case STATETYPE_END:
		switch (ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (LEVEL_BATTLE == m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_LAND);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
			break;
		case CPlayer::SION:
			if (LEVEL_BATTLE == m_pOwner->Get_Level())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_LAND);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::JUMP_LANDING);
			break;
		}
		break;
	}
}

void CJumpState::Exit()
{
	if (m_eStateType == STATETYPE_END)
	{
		m_fTime = 0.f;

		m_pOwner->Off_IsFly();
	}
}

_bool CJumpState::Check_JumpEnd()
{
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(vPosition);
	m_fEndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (m_fEndHeight + 1.f > XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, m_fEndHeight);
		m_pOwner->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		return true;
	}

	return false;
}

void CJumpState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix CameraMatrix = XMLoadFloat4x4(&pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW));

	RELEASE_INSTANCE(CGameInstance);

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

	m_fTime += 0.1f;
	m_pOwner->Get_Transform()->Jump(m_fTime, 3.f, 1.0f, m_fStartHeight, m_fEndHeight);

	if (m_eDirection != DIR_END)
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f);
}