#include "stdafx.h"
#include "..\Public\AIRunawayfromTarget.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"
#include "AICheckState.h"

using namespace AIPlayer;

CAIRunawayfromTarget::CAIRunawayfromTarget(CPlayer* pPlayer, STATE_ID eStateType, _uint playerid)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = playerid;
}

CAIState * CAIRunawayfromTarget::Tick(_float fTimeDelta)
{



	m_fTimer += fTimeDelta;

	if (m_bStopRunning)
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");


		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;
			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
		}

	}
	else
	{
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));
		//m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
		m_pOwner->Get_Transform()->Sliding_Straight(fTimeDelta, m_pOwner->Get_Navigation());
	}


	m_pOwner->Check_Navigation();
	return nullptr;
}

CAIState * CAIRunawayfromTarget::LateTick(_float fTimeDelta)
{
	
	if (m_fTimer > 1.2f && !m_bStopRunning)
	{
		switch (m_eCurrentPlayerID)
		{
		case CPlayer::ALPHEN:
			break;

		case CPlayer::SION:
			__super::Exit();
			m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_BRAKE;
			m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
			m_bStopRunning = true;
			break;

		case CPlayer::RINWELL:
			break;



		}

		

	}
		
	if (m_bIsAnimationFinished && m_bStopRunning)
	{
		return new CAICheckState(m_pOwner, STATE_ID::STATE_RUN);
	}



	return nullptr;
}

void CAIRunawayfromTarget::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;
	
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		break;

	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_RUN;
		break;

	case CPlayer::RINWELL:
		break;

	
		
	}


	m_pOwner->Set_Manarecover(true);

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);
}

void CAIRunawayfromTarget::Exit()
{
	__super::Exit();
}