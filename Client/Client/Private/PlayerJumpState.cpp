#include "stdafx.h"

#include "PlayerJumpState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"

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
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 2.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), nullptr);
	

	if (m_eStateType != STATETYPE_END)
	{
		Move(fTimeDelta);
	}
		

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

	if(Check_JumpEnd())
		return new CJumpState(m_pOwner, m_fStartHeight, STATETYPE_END, m_fTime);

	return nullptr;
}

void CJumpState::Enter()
{
	m_eStateId = STATE_ID::STATE_JUMP;

	CPlayer::PLAYERID ePlayerID = m_pOwner->Get_PlayerID();

	switch (m_eStateType)
	{
	case STATETYPE_START:
		switch (ePlayerID)
		{
		case CPlayer::ALPHEN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_START);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
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
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
			break;
		}
		break;
	case STATETYPE_END:
		switch (ePlayerID)
		{
		case CPlayer::ALPHEN:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_JUMP_LAND);
			break;
		case CPlayer::SION:
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_ATTACK_KAGEROU_END);
			break;
		}
		break;
	}
}

void CJumpState::Exit()
{
	if(m_eStateType == STATETYPE_END)
		m_fTime = 0.f;
}

_bool CJumpState::Check_JumpEnd()
{
	
	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pOwner->Get_Navigation()->Compute_CurrentIndex_byXZ(vPosition);
	m_fEndHeight = m_pOwner->Get_Navigation()->Compute_Height(vPosition, 0.f);

	if (m_fEndHeight > XMVectorGetY(vPosition))
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
		m_fTime += 0.1f;
		m_pOwner->Get_Transform()->Jump(m_fTime, 3.f, 1.0f, m_fStartHeight, m_fEndHeight);

		if(m_eDirection != DIR_END)
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f);
		
	}
		

	

	RELEASE_INSTANCE(CGameInstance);
}