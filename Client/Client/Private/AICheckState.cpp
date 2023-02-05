#include "stdafx.h"
#include "AICheckState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"
#include "AIRunawayfromTarget.h"
#include "Rinwell.h"
#include "AI_ChaseState.h"
#include "AI_Sion_BoostAttackState.h"

using namespace AIPlayer;

CAICheckState::CAICheckState(CPlayer* pPlayer, STATE_ID eStateType)
{
	m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAICheckState::Tick(_float fTimeDelta)
{
	//if(m_ePreStateID =

	if (m_pTarget == nullptr)
		return nullptr;

	m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();


	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();

	

	return nullptr;
}

CAIState * CAICheckState::LateTick(_float fTimeDelta)
{
	if (m_pTarget == nullptr)
		return nullptr;

	m_fTimer += fTimeDelta;

	


	if (m_fTimer > 1.5f)
	{
		if (Get_Target_Distance() <= 10.f)
		{
			m_bRangerRunaway = true; // for ranger runaway
		}
		else
			m_bRangerRunaway = false;

		


		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:


			break;

		case CPlayer::SION:
			//return new CAI_Sion_BoostAttack(m_pOwner);
			if (Get_Target_Distance() >= 20.f)
			{
				return new CAI_ChaseState(m_pOwner, STATE_RUN, m_eCurrentPlayerID , m_pTarget);
			}
			if (m_bRangerRunaway)
				return new CAIRunawayfromTarget(m_pOwner, STATE_RUN , m_eCurrentPlayerID);
			else
				return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);
			break;

		case CPlayer::RINWELL:
			if (m_bRangerRunaway)
				return new CAIRunawayfromTarget(m_pOwner, STATE_RUN , m_eCurrentPlayerID);

			break;

		default:
			break;
		}
		

	}
	


	return nullptr;
}

void CAICheckState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;
	//m_iCurrentAnimIndex = CSion::ANIM::IDLE;
	

	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_BATTLE_MOVE_IDLE;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_IDLE;
		break;
	case CPlayer::RINWELL:
		m_iCurrentAnimIndex = CRinwell::ANIM::BTL_MAGIC_LOOP;
		break;
	default:
		break;
	}
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);


}

void CAICheckState::Exit()
{
	__super::Exit();
}