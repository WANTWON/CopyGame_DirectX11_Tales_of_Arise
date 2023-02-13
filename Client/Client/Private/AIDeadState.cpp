#include "stdafx.h"

#include "AIDeadState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "AICheckState.h"

using namespace AIPlayer;

CDeadState::CDeadState(CPlayer* pPlayer)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CDeadState::Tick(_float fTimeDelta)
{

	

	//m_fTimer += fTimeDelta;

	if(!m_bIsAnimationFinished)
	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	

	return nullptr;
}

CAIState * CDeadState::LateTick(_float fTimeDelta)
{
	if (CUI_Manager::Get_Instance()->Get_CP() >= 0)
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_V))
		{
			CUI_Manager::Get_Instance()->MinusCP(10);
			return new CAICheckState(m_pOwner, m_eStateId);
		}
	}
	
		


	return nullptr;
}

void CDeadState::Enter()
{
	m_eStateId = STATE_ID::STATE_DEAD;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DEAD;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_DEAD;
		break;
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_DEAD;
		break;
	default:
		break;
	}

	m_pOwner->Set_Manarecover(false);
	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
}

void CDeadState::Exit()
{
	__super::Exit();
}