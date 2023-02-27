#include "stdafx.h"
#include "..\Public\Player_SionShooterState.h"

#include "Player_SionNormalAttack_State.h"

using namespace Player;

CPlayer_SionShooterState::CPlayer_SionShooterState(CPlayer * pPlayer)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CPlayer_SionShooterState::HandleInput(void)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);

	return nullptr;
}

CPlayerState * CPlayer_SionShooterState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);

	Move(fTimeDelta);

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CPlayer_SionShooterState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CPlayer_SionShooterState::Enter(void)
{
	__super::Enter();

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_RUN);
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
}

void CPlayer_SionShooterState::Exit(void)
{
}

void CPlayer_SionShooterState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_float4x4 CameraMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

	CTransform* pPlayerTransform = m_pOwner->Get_Transform();

	_vector vPlayerScale, vPlayerRotQuat, vPlayerPos;
	_vector vCamearScale, vCameraRot, vCameraPos;

	XMMatrixDecompose(&vPlayerScale, &vPlayerRotQuat, &vPlayerPos, pPlayerTransform->Get_WorldMatrix());
	XMMatrixDecompose(&vCamearScale, &vCameraRot, &vCameraPos, XMLoadFloat4x4(&CameraMatrix));

	vPlayerRotQuat = XMQuaternionSlerp(vPlayerRotQuat, XMQuaternionNormalize(vCameraRot), 0.5f);

	_matrix vPlayerWorld = XMMatrixScalingFromVector(vPlayerScale) * XMMatrixRotationQuaternion(vPlayerRotQuat) * XMMatrixTranslationFromVector(vPlayerPos);

	pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vPlayerWorld.r[0]);
	pPlayerTransform->Set_State(CTransform::STATE_UP, vPlayerWorld.r[1]);
	pPlayerTransform->Set_State(CTransform::STATE_LOOK, vPlayerWorld.r[2]);
	pPlayerTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerWorld.r[3]);
}
