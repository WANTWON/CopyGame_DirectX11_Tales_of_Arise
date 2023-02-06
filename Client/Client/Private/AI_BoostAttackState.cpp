#include "stdafx.h"
#include "..\Public\AI_BoostAttackState.h"

#include "GameInstance.h"
#include "Alphen.h"
#include "Sion.h"
#include "Rinwell.h"
#include "Alphen.h"
#include "AICheckState.h"
#include "CameraManager.h"

using namespace AIPlayer;

CAI_BoostAttack::CAI_BoostAttack(CPlayer* pPlayer , CBaseObj* pTarget)
{
	//m_ePreStateID = eStateType;
	m_pOwner = pPlayer;
	m_eCurrentPlayerID = m_pOwner->Get_PlayerID();
	m_pTarget = pTarget;
}

CAIState * CAI_BoostAttack::Tick(_float fTimeDelta)
{



	//m_fTimer += fTimeDelta;
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

CAIState * CAI_BoostAttack::LateTick(_float fTimeDelta)
{


	if (m_bIsAnimationFinished)
	return new CAICheckState(m_pOwner, STATE_ID::STATE_BOOSTATTACK);

	return nullptr;
}

void CAI_BoostAttack::Enter()
{
	//m_eStateId = STATE_ID::STATE_DEAD;
	switch (m_eCurrentPlayerID)
	{
	case CPlayer::ALPHEN:
		m_iCurrentAnimIndex = CAlphen::ANIM::ANIM_ATTACK_STRIKE;
		break;
	case CPlayer::SION:
		m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE_AIR;
	}
	//m_iCurrentAnimIndex = CSion::ANIM::BTL_ATTACK_STRIKE;
	
	m_pOwner->Get_Model()->Set_CurrentAnimIndex(m_iCurrentAnimIndex);
	m_pOwner->Get_Transform()->LookAt(m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	CCamera_Dynamic* pCamera =  dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTON);
	pCamera->Set_Target(m_pOwner);

}

void CAI_BoostAttack::Exit()
{
	CCamera_Dynamic* pCamera = dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera());
	pCamera->Set_CamMode(CCamera_Dynamic::CAM_AIBOOSTOFF);

	__super::Exit();
}