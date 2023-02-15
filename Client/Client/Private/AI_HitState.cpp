#include "stdafx.h"
#include "..\Public\AI_HitState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "Rinwell.h"
#include "Sion.h"
#include "AICheckState.h"

using namespace AIPlayer;

CAI_HitState::CAI_HitState(CPlayer* pPlayer)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAI_HitState::Tick(_float fTimeDelta)
{



	//m_fTimer += fTimeDelta;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();



	return nullptr;
}

CAIState * CAI_HitState::LateTick(_float fTimeDelta)
{

	if (m_bIsAnimationFinished)
	return new CAICheckState(m_pOwner, m_eStateId);


	return nullptr;
}

void CAI_HitState::Enter()
{
	m_eStateId = STATE_ID::STATE_HIT;

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		
		switch (rand() % 8)
		{
		case 0:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_B;
			break;
		case 1:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_F;
			break;
		case 2:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_L;
			break;
		case 3:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_LARGE_R;
			break;
		case 4:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_B;
			break;
		case 5:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_F;
			break;
		case 6:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_L;
			break;
		case 7:
			m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_DAMAGE_SMALL_R;
			break;
		}
		break;
	case CPlayer::SION:
		switch (rand() % 8)
		{
		case 0:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_B;
			break;
		case 1:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_F;
			break;
		case 2:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_L;
			break;
		case 3:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_R;
			break;
		case 4:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_B;
			break;
		case 5:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_F;
			break;
		case 6:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_L;
			break;
		case 7:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_R;
			break;
		}
		break;
	case CPlayer::RINWELL:
		switch (rand() % 8)
		{
		case 0:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_B;
			break;
		case 1:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_F;
			break;
		case 2:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_L;
			break;
		case 3:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_LARGE_R;
			break;
		case 4:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_B;
			break;
		case 5:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_F;
			break;
		case 6:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_L;
			break;
		case 7:
			m_iCurrentAnimIndex = CSion::ANIM::BTL_DAMAGE_SMALL_R;
			break;
		}
		break;

	default:
		break;
	}

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

	m_pOwner->Set_Manarecover(false);
}

void CAI_HitState::Exit()
{
	__super::Exit();
}