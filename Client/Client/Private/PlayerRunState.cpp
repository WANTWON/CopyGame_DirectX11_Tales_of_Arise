#include "stdafx.h"

#include "GameInstance.h"
#include "CameraManager.h"
#include "BattleManager.h"

#include "PlayerIdleState.h"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerCollectState.h"
#include "PlayerHitState.h"
#include "PlayerDodgeState.h"
#include "Player_SionNormalAttack_State.h"
#include "Player_SionSkillAttack.h"
#include "Player_RinwellNormalAttack_State.h"
#include "Player_RinwellSkillAttack_State.h"
#include "CloseChaseState.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir, _bool isDash)
	: m_eDirection(eDir)
	, m_bIsDash(isDash)
{
	m_pOwner = pPlayer;
	m_ePlayerID = m_pOwner->Get_PlayerID();
}

CPlayerState * CRunState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	CBattleManager* pBattleManager = CBattleManager::Get_Instance();

	if (pBattleManager->Get_IsBattleMode())
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
		case CPlayer::LAW:
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_NORMAL_ATTACK1);
			if (pGameInstance->Key_Down(DIK_E))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_E);
			if (pGameInstance->Key_Down(DIK_R))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_R);
			if (pGameInstance->Key_Down(DIK_F))
				return new CCloseChaseState(m_pOwner, STATE_CHASE, STATE_SKILL_ATTACK_F);
		case CPlayer::SION:
			//for Sion State//
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_SionNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
			break;
		case CPlayer::RINWELL:
			if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
				return new CPlayer_RinwellNormalAttack_State(m_pOwner, STATE_NORMAL_ATTACK1);
			break;
		}

		/* Skill */
		if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::SION:
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK5);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_SionSkillAttack(m_pOwner, STATE_SKILL_ATTACK_F);

				break;

			case CPlayer::RINWELL: //For Rinwell State
				if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK4);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK5);
				else if (CGameInstance::Get_Instance()->Key_Pressing(DIK_LCONTROL) && CGameInstance::Get_Instance()->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK6);
				else if (pGameInstance->Key_Down(DIK_E))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_E);
				else if (pGameInstance->Key_Down(DIK_R))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_R);
				else if (pGameInstance->Key_Down(DIK_F))
					return new CPlayer_RinwellSkillAttack_State(m_pOwner, STATE_SKILL_ATTACK_F);
				break;
			}
		}
		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_BATTLE);

		if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_A) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_LEFT);
		else if (pGameInstance->Key_Pressing(DIK_D) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_RIGHT);
		else if (pGameInstance->Key_Pressing(DIK_S) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_BACKWARD);
		else if (pGameInstance->Key_Pressing(DIK_W) && pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_STRAIGHT);
		else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			return new CDodgeState(m_pOwner, DIR_END);
	}
	else if (!pBattleManager->Get_IsBattleMode())
	{
		if (pGameInstance->Key_Down(DIK_E))
			return new CCollectState(m_pOwner);

		if (pGameInstance->Key_Down(DIK_SPACE) && !m_bIsFly)
			return new CJumpState(m_pOwner, STATETYPE_START, CJumpState::JUMP_RUN);
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
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);

	if ((!pBattleManager->Get_IsBattleMode()) && pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		if (!m_bIsDash)
		{
			if (CPlayer::ALPHEN == m_ePlayerID)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
			else if (CPlayer::SION == m_ePlayerID)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH);
			else if (CPlayer::RINWELL == m_ePlayerID)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::DASH);
			else if (CPlayer::LAW == m_ePlayerID)
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DASH);
		}

		m_bIsDash = true;

		if (!CBattleManager::Get_Instance()->Get_IsBattleMode())
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(true);
		}
	}
	else
	{
		if (m_bIsDash)
		{
			switch (m_ePlayerID)
			{
			case CPlayer::ALPHEN:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);
				break;
			case CPlayer::SION:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::SYS_RUN);
				break;
			case CPlayer::RINWELL:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN);
				break;
			case CPlayer::LAW:
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::RUN);
				break;
			}
		}

		m_bIsDash = false;

		if (!pBattleManager->Get_IsBattleMode())
		{
			CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
			pCamera->Set_Zoom(false);
		}
	}

	return nullptr;
}

CPlayerState * CRunState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN", 0.05f);

	Move(fTimeDelta);

	m_pOwner->Check_Navigation();

	vector<ANIMEVENT> pEvents = m_pOwner->Get_Model()->Get_Events();

	for (auto& pEvent : pEvents)
	{
		if (pEvent.isPlay)
		{
			if (ANIMEVENT::EVENTTYPE::EVENT_SOUND == pEvent.eType)
			{
				//CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
				if (!m_bSoundStart)
				{
					m_bSoundStart = true;
					CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_Run1.wav"), SOUND_EFFECT, 0.5f);
				}
			}
			else
				m_iRunSoundCount = 0;
		}
	}

	return nullptr;
}

CPlayerState * CRunState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CRunState::Enter()
{
	__super::Enter();

	CBattleManager* pBattleManager = CBattleManager::Get_Instance();
	m_eStateId = STATE_ID::STATE_RUN;

	if (m_bIsDash)
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (!pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_DASH);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::SION:
			if (!pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::DASH);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::RINWELL:
			if (!pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::DASH);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::LAW:
			if (!pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::DASH);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		}
	}
	else
	{
		switch (m_ePlayerID)
		{
		case CPlayer::ALPHEN:
			if (pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_BATTLE_MOVE_RUN);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_RUN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			//CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_RunSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::SION:
			if (pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::BTL_MOVE_RUN);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::SYS_RUN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::RINWELL:
			if (pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CRinwell::ANIM::RUN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
			break;
		case CPlayer::LAW:
			if (pBattleManager->Get_IsBattleMode())
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::BTL_MOVE_RUN);
			else
				m_pOwner->Get_Model()->Set_CurrentAnimIndex(CLaw::ANIM::RUN);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Player_DashSound.wav"), SOUND_FOOT, 0.4f);
				break;
		}
	}

	m_pOwner->Set_Manarecover(true);
}

void CRunState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_FOOT);
}

void CRunState::Move(_float fTimeDelta)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	_float4x4 CameraMatrix = pGameInstance->Get_TransformFloat4x4_Inverse(CPipeLine::D3DTS_VIEW);

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
	{
		if (CBattleManager::Get_Instance()->Get_IsBattleMode())
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 3.f, m_pOwner->Get_Navigation());
		else
			m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta * 2.f, m_pOwner->Get_Navigation());
	}
}