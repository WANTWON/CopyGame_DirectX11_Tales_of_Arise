#include "stdafx.h"
#include "..\Public\IceWolfChaseState.h"
#include "IceWolfHowLingState.h"
#include "IceWolfBattle_IdleState.h"
#include "IceWolfIdleState.h"
#include "IceWolfTurnLeftState.h"
#include "IceWolfWalkState.h"
using namespace IceWolf;

CChaseState::CChaseState(CIce_Wolf * pIceWolf)
{
	m_pOwner = pIceWolf;
	m_fRandTime = ((rand() % 10000) *0.001f)*((rand() % 100) * 0.01f);
}

CIceWolfState * CChaseState::AI_Behaviour(_float fTimeDelta)
{
	

	return nullptr;
}

CIceWolfState * CChaseState::Tick(_float fTimeDelta)
{
	Find_Target_InField();
	

	m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()));

	if (!m_bIsAnimationFinished)
	{
		_vector vecTranslation;
		_float fRotationRadian;

		m_pOwner->Get_Model()->Get_MoveTransformationMatrix("ABone", &vecTranslation, &fRotationRadian);

		m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

		m_pOwner->Check_Navigation();
	}

  	/*m_pCurTarget = m_pOwner->Check_FiledTarget();

	if (m_pCurTarget == nullptr)
	{
		return new CIdleState(m_pOwner, FIELD_STATE_ID::FIELD_STATE_END);
	}

	else if (m_pCurTarget != nullptr)
	{
		m_vCurTargetPos = m_pCurTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_fTarget_Distance = m_pOwner->Target_Distance(m_pCurTarget);
	}*/
	
	
	
	return nullptr;
}

CIceWolfState * CChaseState::LateTick(_float fTimeDelta)
{
	//m_pOwner->Check_Navigation();


	//vTargetPosition = XMVectorSetY(vTargetPosition, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
	
	if (m_pTarget)
	{
		_vector vTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
		m_pOwner->Get_Transform()->LookAt(vTargetPosition);
		m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
	}

	if (m_fTarget_Distance >= 15.f || m_pTarget == nullptr)
		return new CIdleState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);
	


	//02.23 01:49 알펜 외에 다른 케릭터가 다가갈 때 쫓아오게 하는 코드 구현못했음. 

	/*_vector vPosition = XMVectorSetY(m_vCurTargetPos, XMVectorGetY(m_pOwner->Get_TransformState(CTransform::STATE_TRANSLATION)));
	m_pOwner->Get_Transform()->LookAt(vPosition);
	m_pOwner->Get_Transform()->Go_Straight(fTimeDelta, m_pOwner->Get_Navigation());

	if (m_fTarget_Distance > 10.f)
		return new CIdleState(m_pOwner, CIceWolfState::FIELD_STATE_ID::FIELD_STATE_IDLE);*/
	


	return nullptr;
}

//void CChaseState::Find_Target_InField(void)
//{
//	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
//	if (!pPlayer)
//		return;
//
//	_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
//	_vector vPosition = m_pOwner->Get_TransformState(CTransform::STATE::STATE_TRANSLATION);
//
//	_float fDistance = XMVectorGetX(XMVector3Length(vPlayerPosition - vPosition));
//	m_fTarget_Distance = fDistance;
//	if (fDistance < m_pOwner->Get_AggroRadius())
//	{
//		m_pTarget = pPlayer;
//
//		m_pOwner->Get_Transform()->Change_Speed(m_pOwner->Get_Stats().m_fRunSpeed);
//
//		if (5 > fDistance)
//			m_bBattleMode = true;
//	}
//}

void CChaseState::Enter()
{
	m_eStateId = STATE_ID::STATE_DISCOVER;


	m_pOwner->Get_Model()->Set_CurrentAnimIndex(CIce_Wolf::ANIM::ANIM_SYMBOL_RUN);

	CGameInstance::Get_Instance()->PlaySounds(TEXT("Wolf_Chase.wav"), SOUND_VOICE, 0.1f);
}

void CChaseState::Exit()
{
	CGameInstance::Get_Instance()->StopSound(SOUND_VOICE);
}
