#include "stdafx.h"
#include "AICheckState.h"
#include "GameInstance.h"
#include "Alphen.h"
#include "AIAttackNormalState.h"
#include "AIRunawayfromTarget.h"

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


	m_pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();


	/*if (Get_Target_Distance() <= 5.f)
	{

	}*/
	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	m_fTimer += fTimeDelta;

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	m_pOwner->Check_Navigation();



	return nullptr;
}

CAIState * CAICheckState::LateTick(_float fTimeDelta)
{

	
//	return new CAICheckState(m_pOwner, m_eStateId);
	if (m_fTimer > 1.5f)
	{
		if (Get_Target_Distance() <= 2.f)
		{
			return new CAIRunawayfromTarget(m_pOwner, STATE_RUN);
		}
		else
			return new CAIAttackNormalState(m_pOwner, STATE_ATTACK, m_pTarget);

	}
	


	return nullptr;
}

void CAICheckState::Enter()
{
	m_eStateId = STATE_ID::STATE_IDLE;
	m_iCurrentAnimIndex = CSion::ANIM::IDLE;
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);

	//m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);



}

void CAICheckState::Exit()
{
	__super::Exit();
}