#include "stdafx.h"
#include "..\Public\Camera_Action.h"
#include "GameInstance.h"
#include "Camera_Manager.h"
#include "Imgui_Manager.h"

CCamera_Action::CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Action::CCamera_Action(const CCamera_Action & rhs)
	: CCamera(rhs)
{
	m_CamDatas.reserve(0);
}

void CCamera_Action::Add_CamData(TOOLDESC CamDesc)
{
	if (CImgui_Manager::Get_Instance()->Get_TargetMode())
	{
		TOOLDESC ToolCamDesc;
		memcpy(&ToolCamDesc, &CamDesc, sizeof(TOOLDESC));
		_vector vLocalDistance = XMLoadFloat4(&ToolCamDesc.vAtPosition) - XMLoadFloat4(&ToolCamDesc.vEyePosition);
		vLocalDistance = XMVectorSetW(vLocalDistance, 1.f);
		_vector vLocalAtPos = { 0.f,0.f,0.f,1.f };
		m_CamDatas.push_back(ToolCamDesc);
	}
	else
		m_CamDatas.push_back(CamDesc);
}

void CCamera_Action::Set_PositionToCurrentData(_int iIndex)
{
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&m_CamDatas[iIndex].vEyePosition));
	m_pTransform->LookAt(XMLoadFloat4(&m_CamDatas[iIndex].vAtPosition));
}



void CCamera_Action::Set_Play(_bool type)
{
	 m_bPlay = type; 
	 m_bFinished = false;
	 if (m_bPlay)
	 {
		 m_vInitPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION); 
		 m_vInitAt = m_vInitPosition + m_pTransform->Get_State(CTransform::STATE_LOOK);
	}
}

int CCamera_Action::PlayCamera(_float fTimeDelta)
{
	if (m_CamDatas[m_iIndex].bNewSection == true)
	{
		m_vInitPosition = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
		m_vInitAt = XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition);
		m_pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vInitPosition);
		m_pTransform->LookAt(m_vInitAt);
		m_iIndex++;
		m_fTime = m_CamDatas[m_iIndex].fStartTime*m_fPlayTime;

	}


	m_fTime += fTimeDelta;
	
	_float fStartTime = m_CamDatas[m_iIndex].fStartTime* m_fPlayTime;
	_float fEndTime = m_CamDatas[m_iIndex].fEndTime* m_fPlayTime;
	_float fValue = (m_fTime - fStartTime) / (fEndTime - fStartTime);

	
	_vector vPositionStart = m_vInitPosition;
	_vector vPositionEnd = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
	_vector vCatRom1 = vPositionStart;
	_vector vCatRom2 = vPositionEnd;
	
	
	_vector vAtStart = m_vInitAt;
	_vector vAtEnd = XMVectorSetW(XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition), 1.f);
	_vector vPosition;

	if(m_CamDatas[m_iIndex].bLerp == true)
		vPosition = XMVectorLerp(vPositionStart, vPositionEnd, fValue);
	else
		vPosition = XMVectorCatmullRom(vCatRom1, vPositionStart, vPositionEnd, vCatRom2, fValue);
	
	
	
	_vector vAt = XMVectorLerp(vAtStart, vAtEnd, fValue);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_pTransform->LookAt(vAt);


	if (m_fTime >= fEndTime)
	{
		m_vInitPosition = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
		m_vInitAt = XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition);

		m_iIndex++;

		if (m_iIndex >= m_CamDatas.size())
		{
			m_iIndex = 0;
			m_bPlay = false;
			CImgui_Manager::Get_Instance()->Set_Play(false);
			Set_CamMode(CAM_DEBUG);
			m_bFinished = true;
		}
	}

	return OBJ_NOEVENT;
}

int CCamera_Action::DebugVCamera(_float fTimeDelta)
{
	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		_float fSpeedOffSet = CImgui_Manager::Get_Instance()->Get_CameraDebugSpeed();
		/* Camera Inputs */
		if (pGameInstance->Key_Pressing(DIK_W))
			m_pTransform->Go_Straight(fTimeDelta * fSpeedOffSet);
		if (pGameInstance->Key_Pressing(DIK_S))
			m_pTransform->Go_Backward(fTimeDelta * fSpeedOffSet);
		if (pGameInstance->Key_Pressing(DIK_A))
			m_pTransform->Go_Left(fTimeDelta * fSpeedOffSet);
		if (pGameInstance->Key_Pressing(DIK_D))
			m_pTransform->Go_Right(fTimeDelta * fSpeedOffSet);

		_long MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

HRESULT CCamera_Action::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Action::Initialize(void* pArg)
{
	

	if (FAILED(__super::Initialize(&((ACTIONCAMDESC*)pArg)->CameraDesc)))
		return E_FAIL;
	
	CImgui_Manager::Get_Instance()->Set_ActionCamera(this);
	return S_OK;
}

int CCamera_Action::Tick(_float fTimeDelta)
{
	if (CCamera_Manager::Get_Instance()->Get_CamMode() != CCamera_Manager::ACTION)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch (m_eCamMode)
	{
	case CAM_DEBUG:
		DebugVCamera(fTimeDelta);
		break;
	case CAM_ACTION:
		if (m_bPlay)
		{
			PlayCamera(fTimeDelta);
		}
		break;
	default:
		break;
	}

	



	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;

	return OBJ_NOEVENT;
}

void CCamera_Action::Late_Tick(_float fTimeDelta)
{
	if (CCamera_Manager::Get_Instance()->Get_CamMode() != CCamera_Manager::ACTION)
		return;
	__super::Late_Tick(fTimeDelta);

}

HRESULT CCamera_Action::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	return S_OK;
}

CCamera_Action * CCamera_Action::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Action*	pInstance = new CCamera_Action(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Action::Clone(void* pArg)
{
	CCamera_Action*	pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Action::Free()
{
	__super::Free();

	
}
