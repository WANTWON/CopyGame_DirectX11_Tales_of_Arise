#include "stdafx.h"

#include "PlayerRunState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"
#include "PlayerAttackNormalState.h"
#include "PlayerJumpState.h"
#include "PlayerSkillState.h"
#include "PlayerCollectState.h"
#include "CameraManager.h"

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
	else if (LEVEL_SNOWFIELD == m_pOwner->Get_Level())
	{
		if (pGameInstance->Key_Down(DIK_E))
			return new CCollectState(m_pOwner);
	}
	
	if (pGameInstance->Key_Down(DIK_LCONTROL) && !m_bIsFly)
		return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f, CJumpState::JUMP_RUN);
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

	if ((LEVEL_BATTLE != m_pOwner->Get_Level()) && pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (!m_bIsDash)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
		
		m_bIsDash = true;


		//CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>( CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_Zoom(true);

		/*CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>( CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(true);*/

	}
	else
	{
		if (m_bIsDash)
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);

		m_bIsDash = false;


		//CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		//pCamera->Set_Zoom(false);

		/*CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_Zoom(false);*/

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
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH);
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
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_RUN);
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

	_float4x4 CameraMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

	RELEASE_INSTANCE(CGameInstance);

	_vector vCameraLook = XMVectorSet(CameraMatrix.m[2][0], 0.f, CameraMatrix.m[2][2], CameraMatrix.m[2][3]);

	switch (m_eDirection)
	{
	case DIR_STRAIGHT_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)));
		break;
	case DIR_STRAIGHT_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f))); 
		break;
	case DIR_BACKWARD_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-135.f)));
		break;
	case DIR_BACKWARD_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(135.f)));
		break;
	case DIR_STRAIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(0.f)));
		break;
	case DIR_BACKWARD:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f)));
		break;
	case DIR_LEFT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f)));
		break;
	case DIR_RIGHT:
		vCameraLook = XMVector3TransformNormal(vCameraLook, XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f)));
		break;
	}

	_matrix NewCameraMatrix = XMMatrixIdentity();
	NewCameraMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	NewCameraMatrix.r[2] = vCameraLook;
	NewCameraMatrix.r[0] = XMVector3Cross(NewCameraMatrix.r[1], NewCameraMatrix.r[2]);

	CTransform* pPlayerTransform = m_pOwner->Get_Transform();
	
	_vector vPlayerScale, vPlayerRotQuat, vPlayerPos;
	_vector vNewCamearScale, vNewCameraRot, vNewCameraPos;

	XMMatrixDecompose(&vPlayerScale, &vPlayerRotQuat, &vPlayerPos, pPlayerTransform->Get_WorldMatrix());
	XMMatrixDecompose(&vNewCamearScale, &vNewCameraRot, &vNewCameraPos, NewCameraMatrix);

	vPlayerRotQuat = XMQuaternionSlerp(vPlayerRotQuat, XMQuaternionNormalize(vNewCameraRot), 0.5f);

	_matrix vPlayerWorld = XMMatrixScalingFromVector(vPlayerScale) * XMMatrixRotationQuaternion(vPlayerRotQuat) * XMMatrixTranslationFromVector(vPlayerPos);

	pPlayerTransform->Set_State(CTransform::STATE_RIGHT, vPlayerWorld.r[0]);
	pPlayerTransform->Set_State(CTransform::STATE_UP, vPlayerWorld.r[1]);
	pPlayerTransform->Set_State(CTransform::STATE_LOOK, vPlayerWorld.r[2]);
	pPlayerTransform->Set_State(CTransform::STATE_TRANSLATION, vPlayerWorld.r[3]);

	if (m_bIsDash)
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 4.f, m_pOwner->Get_Navigation());
	else
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2.f, m_pOwner->Get_Navigation());
}