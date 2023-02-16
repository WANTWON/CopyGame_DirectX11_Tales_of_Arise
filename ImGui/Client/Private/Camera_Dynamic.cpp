#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "Camera_Manager.h"
#include "Imgui_Manager.h"
#include "BaseObj.h"

CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Dynamic::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Dynamic::Initialize(void* pArg)
{
	

	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;
	

	return S_OK;
}

int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	if (CCamera_Manager::Get_Instance()->Get_CamMode() != CCamera_Manager::DYNAMIC)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	switch (m_eCamMode)
	{
	case CAM_DEBUG:
		DebugVCamera(fTimeDelta);
		break;
	case CAM_TARGET:
		PlayCamera(fTimeDelta);
		break;
	default:
		break;
	}


	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;

	return OBJ_NOEVENT;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	if (CCamera_Manager::Get_Instance()->Get_CamMode() != CCamera_Manager::DYNAMIC)
		return ;
	__super::Late_Tick(fTimeDelta);

}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	return S_OK;
}

int CCamera_Dynamic::PlayCamera(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	_float fStartTime = m_CamDatas[m_iIndex].fStartTime* m_fPlayTime;
	_float fEndTime = m_CamDatas[m_iIndex].fEndTime* m_fPlayTime;
	_float fValue = (m_fTime - fStartTime) / (fEndTime - fStartTime);

	_vector pTargetLook = m_pTarget->Get_TransformState(CTransform::STATE_LOOK);
	_vector pTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector pPlayertoCameraDir;

	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f,1.f,0.f,0.f), m_CamDatas[m_iIndex].fRadian);
	pPlayertoCameraDir = XMVector3TransformNormal(pTargetLook, RotationMatrix);
	_vector vCameraPos = pTargetPosition + pPlayertoCameraDir*m_CamDatas[m_iIndex].fLength;
	vCameraPos = XMVectorSetY(vCameraPos, XMVectorGetY(vCameraPos) + m_CamDatas[m_iIndex].fYoffset);

	_vector FinalPos = XMVectorLerp(m_vInitPos, vCameraPos, fValue); //_float4 저장 y올리기 
	
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	//_vector vAtPosition = pTargetPosition + XMLoadFloat4(&m_CamDatas[m_iIndex].vLookDir);
	//_vector vAtPosition = pTargetPosition + XMLoadFloat4(&m_CamDatas[m_iIndex].vLook);
	_vector vAtPosition = XMVectorSetY(pTargetPosition , XMVectorGetY(pTargetPosition) + 4.f);
	_vector vAt = XMVectorLerp(m_vInitAt, vAtPosition, fValue);
	m_pTransform->LookAt(vAt);
	//m_pTransform->LookDir(vAt);


	if (m_fTime >= fEndTime)
	{
		m_vInitAt = vAtPosition;
		//m_vInitAt = pTargetPosition + XMLoadFloat4(&m_CamDatas[m_iIndex].vLook);
		//m_vInitAt =  XMLoadFloat4(&m_CamDatas[m_iIndex].vLookDir);
		m_iIndex++;
		m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

		_vector vCameraLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		_vector vCameraCurrentPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
		_vector vCurrentAt = vCameraCurrentPos + XMVectorSetW(vCameraLook, 0.f);
		
		

		if (m_iIndex >= m_CamDatas.size())
		{
			m_iIndex = 0;
			m_bPlay = false;
			CImgui_Manager::Get_Instance()->Set_Play(false);
			Set_CamMode(CAM_DEBUG);
		}
	}

	return OBJ_NOEVENT;
}

int CCamera_Dynamic::DebugVCamera(_float fTimeDelta)
{
	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_LSHIFT))
	{
		/* Camera Inputs */
		if (pGameInstance->Key_Pressing(DIK_W))
			m_pTransform->Go_Straight(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_S))
			m_pTransform->Go_Backward(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_A))
			m_pTransform->Go_Left(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_D))
			m_pTransform->Go_Right(fTimeDelta * 2);

		_long MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}

	RELEASE_INSTANCE(CGameInstance);
	return 0;
}

void CCamera_Dynamic::Set_Play(_bool type)
{
	 m_bPlay = type;
	
	 if (m_bPlay)
	 {
		 m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

		 _vector vCameraLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
		 _vector vCameraCurrentPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
		 _vector vCurrentAt = vCameraCurrentPos + vCameraLook;

		 m_vInitAt = vCurrentAt;
		 //m_vInitAt = vCameraLook;
	 }
		

}



CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic*	pInstance = new CCamera_Dynamic(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Dynamic::Free()
{
	__super::Free();

	
}
