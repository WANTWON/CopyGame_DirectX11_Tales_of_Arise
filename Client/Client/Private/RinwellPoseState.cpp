#include "stdafx.h"
#include "RinwellPoseState.h"
#include "RinwellIdleState.h"
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
	
	switch (m_eStateId)
	{
	case Client::CRinwellState::STATE_IDLE:
		if (!m_bFinised)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");


		m_fTarget_Distance =  Find_ActiveTarget();
		if (m_fTarget_Distance < 10.f && m_pOwner->Get_NpcMode() == false)
			return new CPoseState(m_pOwner, STATE_AGGRO);
		break;
	case Client::CRinwellState::STATE_HP50DOWN:
		m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f,false);
		break;
	default:
		if (!m_bFinised)
			m_bIsAnimationFinished = m_pOwner->Get_Model()->Play_Animation(fTimeDelta * 1.f, m_pOwner->Is_AnimationLoop(m_pOwner->Get_Model()->Get_CurrentAnimIndex()), "TransN");
		break;
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
			if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			{
				m_pOwner->Set_IsActionMode(false);
				return new CRinwellIdleState(m_pOwner, 0.5f);
			}
			else
				return nullptr;
			break;
		case Client::CRinwellState::STATE_BATTLESTART:
			return new CRinwellIdleState(m_pOwner, 0.5f);
		}
	}

	m_pOwner->Get_Collider()->Update(m_pOwner->Get_Transform()->Get_WorldMatrix());

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
	{
		_vector vPosition = XMVectorSet(60.f, 0.2f, 80.f, 1.f);
		m_pOwner->Get_Transform()->LookDir(XMVectorSet(0.f, 0.f, -1.f, 0.f));
		m_pOwner->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Set_CamState(CCameraManager::CAM_ACTION);
		pCameraManager->Play_ActionCamera(TEXT("Rinwell50Down.dat"), m_pOwner->Get_Transform()->Get_WorldMatrix());
		m_pOwner->Set_AirMode(true);
		m_pOwner->Get_Model()->Set_CurrentAnimIndex(CAiRinwell::BTL_ATTACK_DENZIHOU);
		m_pOwner->Get_Model()->Reset();
		break;
	}
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

