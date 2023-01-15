#include "stdafx.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "PlayerRunState.h"
#include "PlayerAttackNormalState.h"

using namespace Player;

CIdleState::CIdleState(CPlayer* pPlayer)
{
	m_pOwner = pPlayer;
}

CPlayerState * CIdleState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Mouse_Down(DIMK_LBUTTON))
		return new CAttackNormalState(m_pOwner);
	else if (pGameInstance->Key_Pressing(DIK_UP) && pGameInstance->Key_Pressing(DIK_LEFT))
		return new CRunState(m_pOwner, DIR_STRAIGHT_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_UP) && pGameInstance->Key_Pressing(DIK_RIGHT))
		return new CRunState(m_pOwner, DIR_STRAIGHT_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_DOWN) && pGameInstance->Key_Pressing(DIK_LEFT))
		return new CRunState(m_pOwner, DIR_BACKWARD_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_DOWN) && pGameInstance->Key_Pressing(DIK_RIGHT))
		return new CRunState(m_pOwner, DIR_BACKWARD_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_LEFT))
		return new CRunState(m_pOwner, DIR_LEFT);
	else if (pGameInstance->Key_Pressing(DIK_RIGHT))
		return new CRunState(m_pOwner, DIR_RIGHT);
	else if (pGameInstance->Key_Pressing(DIK_DOWN))
		return new CRunState(m_pOwner, DIR_BACKWARD);
	else if (pGameInstance->Key_Pressing(DIK_UP))
		return new CRunState(m_pOwner, DIR_STRAIGHT);

	return nullptr;
}

CPlayerState * CIdleState::Tick(_float fTimeDelta)
{
	_matrix matRootNode;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CIdleState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CIdleState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CPlayer::ANIM::ANIM_IDLE);
}

void CIdleState::Exit()
{

}