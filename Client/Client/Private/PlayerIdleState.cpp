#include "stdafx.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "PlayerRunState.h"
#include "PlayerAttackNormalState.h"

#include "Effect.h"
#include "EffectTexture.h"
#include "PlayerJumpState.h"
#include "PlayerSkillState.h"

using namespace Player;

CIdleState::CIdleState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CIdleState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CAttackNormalState(m_pOwner, STATE_NORMAL_ATTACK1);
	else if (pGameInstance->Key_Down(DIK_LCONTROL) && !m_bIsFly)
		return new CJumpState(m_pOwner, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)), STATETYPE_START, 0.f);
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

	/* Skill */
	if (floor(m_pOwner->Get_Info().fCurrentMp) > 0)
	{
	//	if (pGameInstance->Key_Down(DIK_E)) //change
	//		return new CSkillState(m_pOwner, STATE_SKILL_ATTACK1);
		if (pGameInstance->Key_Down(DIK_R))
			return new CSkillState(m_pOwner, STATE_SKILL_ATTACK2);
		else if (pGameInstance->Key_Down(DIK_F))
			return new CSkillState(m_pOwner, STATE_SKILL_ATTACK3);
	}

	return nullptr;
}

CPlayerState * CIdleState::Tick(_float fTimeDelta)
{
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	__super::Enter();

	/*CWeapon::WEAPONDESC WeaponDesc;
	ZeroMemory(&WeaponDesc, sizeof(CWeapon::WEAPONDESC));

	CHierarchyNode* pSocket = m_pOwner->Get_Model()->Get_BonePtr("SWG_CHR_ARI_HUM_003_COLOAR00_00_L_end");
	if (nullptr == pSocket)
		return;

	WeaponDesc.pSocket = pSocket;
	WeaponDesc.SocketPivotMatrix = m_pOwner->Get_Model()->Get_PivotFloat4x4();
	WeaponDesc.pParentWorldMatrix = m_pOwner->Get_Transform()->Get_World4x4Ptr();
	strcpy(WeaponDesc.pModeltag, "SWO1(R00)");

	((CWeapon*)m_pOwner->Get_Parts(0))->Set_WeaponDesc(WeaponDesc);*/

	m_eStateId = STATE_ID::STATE_IDLE;

	switch (m_pOwner->Get_PlayerID())
	{
	case CPlayer::ALPHEN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAlphen::ANIM::ANIM_IDLE);
		break;
	case CPlayer::SION:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CSion::ANIM::ANIM_IDLE);
		break;
	default:
		break;
	}
}

void CIdleState::Exit()
{
	__super::Exit();
}