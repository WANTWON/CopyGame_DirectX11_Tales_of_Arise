#include "stdafx.h"
#include "..\Public\PlayerOverlimit.h"

#include "PlayerIdleState.h"
#include "GameInstance.h"
#include "BattleManager.h"
#include "Monster.h"

using namespace Player;

CPlayerOverlimit::CPlayerOverlimit(CPlayer * pPlayer)
{
	m_pOwner = pPlayer;
	
}

CPlayerState * CPlayerOverlimit::HandleInput(void)
{
	return nullptr;
}

CPlayerState * CPlayerOverlimit::Tick(_float fTimeDelta)
{
	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return nullptr;

	if(!CBattleManager::Get_Instance()->Get_LackonMonster())
	CBaseObj* pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
     else
	{
		CBaseObj*	pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
	}



	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
	if (!m_bIsAnimationFinished)
	{
		

		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());
	}

	m_pOwner->Check_Navigation();

	return nullptr;
}

CPlayerState * CPlayerOverlimit::LateTick(_float fTimeDelta)
{

	if (m_bIsAnimationFinished)
	{
		return new CIdleState(m_pOwner, CIdleState::IDLE_MAIN);
		CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
		pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);
	}

	return nullptr;
}

void CPlayerOverlimit::Enter(void)
{
	__super::Enter();

	m_eStateId = STATE_ID::STATE_OVERLIMIT;

	m_pOwner->Get_Model()->Set_CurrentAnimIndex(0);



	if (CBattleManager::Get_Instance()->IsAllMonsterDead())
		return;

	if (nullptr != CBattleManager::Get_Instance()->Get_LackonMonster())
	{
		CBaseObj* pTarget = CBattleManager::Get_Instance()->Get_LackonMonster();
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}
		
	else
	{
		CBaseObj*	pTarget = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_MinDistance_Monster
		(m_pOwner->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION)));
		m_pOwner->Get_Transform()->LookAtExceptY(pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));
	}


	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	//	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
	pCamera->Set_Target(m_pOwner);

	m_pOwner->Set_Manarecover(false);

	m_pOwner->Set_Overlimit(true);
	
	
}

void CPlayerOverlimit::Exit(void)
{

	__super::Exit();
}
