#include "stdafx.h"
#include "CameraManager.h"
#include "UI_Dialogue_Caption.h"
#include "Monster_LawPoseState.h"
#include "Monster_LawIdleState.h"

using namespace MonsterLaw;

CPoseState::CPoseState(CMonsterLaw* pRinwell, STATE_ID eStateID)
{
	m_pOwner = pRinwell;
	m_eStateId = eStateID;
}


CMonsterLawState * CPoseState::Tick(_float fTimeDelta)
{
	if(!m_bFinised)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	


	return nullptr;
}

CMonsterLawState * CPoseState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished && CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		return new CMonster_LawIdleState(m_pOwner);
	}

	return nullptr;
}

void CPoseState::Enter()
{
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CMonsterLaw::BTL_ADVENT);


	/*if (m_eStateId == STATE_PHEROMONE)
	{

	}*/
	 
	
}

void CPoseState::Exit()
{
	
}

