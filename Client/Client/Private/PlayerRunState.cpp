#include "stdafx.h"

#include "PlayerRunState.h"
#include "GameInstance.h"
#include "PlayerIdleState.h"

using namespace Player;

CRunState::CRunState(CPlayer* pPlayer, DIRID eDir)
	: m_eDirection(eDir)
{
	m_pOwner = pPlayer;
}

CPlayerState * CRunState::HandleInput()
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	if (pGameInstance->Key_Pressing(DIK_UP) && pGameInstance->Key_Pressing(DIK_LEFT))
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
	else
		return new CIdleState(m_pOwner);

	return nullptr;
}

CPlayerState * CRunState::Tick(_float fTimeDelta)
{
	_matrix matRootNode;

	m_pOwner->Get_Model()->Play_Animation(fTimeDelta, &matRootNode/*, m_bIsAnimationFinished, pPlayer->Is_AnimationLoop(pPlayer->Get_Model()->Get_CurrentAnimIndex())*/);
	//pPlayer->Sync_WithNavigationHeight();

	Move(fTimeDelta);

	return nullptr;
}

CPlayerState * CRunState::LateTick(_float fTimeDelta)
{
	return nullptr;
}

void CRunState::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;

	m_pOwner->Get_Model()->Set_NextAnimIndex(CPlayer::ANIM::ANIM_RUN);
}

void CRunState::Exit()
{

}

void CRunState::Move(_float fTimeDelta)
{
	/*switch (m_eDirection)
	{
	case DIR_STRAIGHT_LEFT:
		pPlayer->Get_Transform()->Set_RotationY(305.f);
		break;
	case DIR_STRAIGHT_RIGHT:
		pPlayer->Get_Transform()->Set_RotationY(45.f);
		break;
	case DIR_BACKWARD_LEFT:
		pPlayer->Get_Transform()->Set_RotationY(225.f);
		break;
	case DIR_BACKWARD_RIGHT:
		pPlayer->Get_Transform()->Set_RotationY(135.f);
		break;
	case DIR_STRAIGHT:
		pPlayer->Get_Transform()->Set_RotationY(0.f);
		break;
	case DIR_BACKWARD:
		pPlayer->Get_Transform()->Set_RotationY(180.f);
		break;
	case DIR_LEFT:
		pPlayer->Get_Transform()->Set_RotationY(270.f);
		break;
	case DIR_RIGHT:
		pPlayer->Get_Transform()->Set_RotationY(90.f);
		break;
	}

	pPlayer->Get_Transform()->Move_Straight(fTimeDelta, pPlayer->Get_Navigation(), pPlayer->Get_Radius());*/
}