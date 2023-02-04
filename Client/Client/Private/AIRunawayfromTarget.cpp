#include "stdafx.h"
#include "..\Public\AIRunawayfromTarget.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"
#include "AICheckState.h"

using namespace AIPlayer;

CAIRunawayfromTarget::CAIRunawayfromTarget(CPlayer* pPlayer, STATE_ID eStateType)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
}

CAIState * CAIRunawayfromTarget::Tick(_float fTimeDelta)
{



	m_fTimer += fTimeDelta;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();


	

	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

	return nullptr;
}

CAIState * CAIRunawayfromTarget::LateTick(_float fTimeDelta)
{

	if (m_fTimer > 1.2f)
		return new CAICheckState(m_pOwner, STATE_ID::STATE_RUN);


	return nullptr;
}

void CAIRunawayfromTarget::Enter()
{
	m_eStateId = STATE_ID::STATE_RUN;
	
	switch (m_eCurrentPlayerID)
	{
	case 0:
		break;
	case 1:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_MOVE_RUN;
		break;
		
	}


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 2.f);
}

void CAIRunawayfromTarget::Exit()
{
	__super::Exit();
}