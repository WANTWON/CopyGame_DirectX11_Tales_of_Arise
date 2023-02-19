#include "stdafx.h"
#include "RinwellPoseState.h"
#include "RinwellMoveState.h"
#include "CameraManager.h"
#include "UI_Dialogue_Caption.h"

using namespace AiRinwell;

CPoseState::CPoseState(CAiRinwell* pRinwell, STATE_ID eStateID)
{
	m_pOwner = pRinwell;
	m_eStateId = eStateID;
}


CRinwellState * CPoseState::Tick(_float fTimeDelta)
{
	if(!m_bFinised)
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");

	switch (m_eStateId)
	{
	case Client::CRinwellState::STATE_IDLE:
		m_fTarget_Distance =  Find_ActiveTarget();
		if (m_fTarget_Distance < 10.f && m_pOwner->Get_NpcMode() == false)
			return new CPoseState(m_pOwner, STATE_AGGRO);
		break;
	case Client::CRinwellState::STATE_HP50DOWN:
		if (!m_bIsAnimationFinished)
		{
			_vector vecTranslation;
			_float fRotationRadian;
			m_pOwner->Get_Model()->Get_MoveTransformationMatrix("TransN", &vecTranslation, &fRotationRadian);
			m_pOwner->Get_Transform()->Sliding_Anim((vecTranslation * 0.01f), fRotationRadian, m_pOwner->Get_Navigation());

			m_pOwner->Check_Navigation();

		}
	}

	return nullptr;
}

CRinwellState * CPoseState::LateTick(_float fTimeDelta)
{
	if (m_bIsAnimationFinished)
	{
		switch (m_eStateId)
		{
		case Client::CRinwellState::STATE_AGGRO:
		{
			m_bFinised = true;
			if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			{
				m_pOwner->Set_IsActionMode(false);
				return new CMoveState(m_pOwner, STATETYPE_START, 0);
				
			}	
			else
				return nullptr;
		}	
		case Client::CRinwellState::STATE_MOVE:
			break;
		case Client::CRinwellState::STATE_HP50DOWN:
			return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
			break;
		case Client::CRinwellState::STATE_BATTLESTART:
			return new CMoveState(m_pOwner, STATETYPE_MAIN, 0);
		}
	}

	m_pOwner->Check_Navigation();
	return nullptr;
}

void CPoseState::Enter()
{

	switch (m_eStateId)
	{
	case Client::CRinwellState::STATE_IDLE:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::IDLE_CHARA);
		break;
	case Client::CRinwellState::STATE_AGGRO:
	{
		CCameraManager* pCameraManager = CCameraManager::Get_Instance();

		dynamic_cast<CUI_Dialogue_Caption*>(CUI_Manager::Get_Instance()->Get_DialogueCaption())->Open_Dialogue(0);

		pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
		pCameraManager->Play_ActionCamera(TEXT("RinwellAction.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_ATTACK_BRAVE);
		m_pOwner->Set_IsActionMode(true);
		break;
	}		
	case Client::CRinwellState::STATE_HP50DOWN:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_ATTACK_DENZIHOU);
		break;
	case Client::CRinwellState::STATE_BATTLESTART:
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_ATTACK_BRAVE);
		break;
	default:
		break;
	}


		
}

void CPoseState::Exit()
{
	
}

