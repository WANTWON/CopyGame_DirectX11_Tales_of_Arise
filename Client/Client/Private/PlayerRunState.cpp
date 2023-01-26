#include "stdafx.h"

#include "PlayerRunState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir)
	: m_eDirection(eDir)
{
	m_pOwner = pPlayer;
}

CPlayerState * CRunState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CAttackNormalState(m_pOwner);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_BACKWARD_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_A))
		return new CRunState(m_pOwner, DIR_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_D))
		return new CRunState(m_pOwner, DIR_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_S))
		return new CRunState(m_pOwner, DIR_BACKWARD);
	else if (pGameInstance->Key_Pressing(DIK_W))
		return new CRunState(m_pOwner, DIR_STRAIGHT);
	else
		return new CIdleState(m_pOwner);

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
	m_eStateId = STATE_ID::STATE_RUN;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CAlphen::ANIM::ANIM_RUN);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_NextAnimIndex(CSion::ANIM::ANIM_RUN);
		break;
	default:
		break;
	}
}

void CRunState::Exit()
{
	__super::Exit();
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
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2.f, m_pOwner->Get_Navigation());

	RELEASE_INSTANCE(CGameInstance);
}