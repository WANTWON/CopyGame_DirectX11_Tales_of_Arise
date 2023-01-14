#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "Player.h"

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
	
	m_vDistance = ((CAMERADESC_DERIVED*)pArg)->vDistance;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&((CAMERADESC_DERIVED*)pArg)->InitPostion));

	return S_OK;
}

int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Up(DIK_F1))
	{
		if (m_eCamMode != CAM_DEBUG)
		{
			m_ePreCamMode = m_eCamMode;
			m_eCamMode = CAM_DEBUG;
		}
		else
		{
			m_eCamMode = m_ePreCamMode;
			m_ePreCamMode = CAM_DEBUG;
		}
			
	}


	switch (m_eCamMode)
	{
	case Client::CCamera_Dynamic::CAM_PLAYER:
		Player_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_DEBUG:
		Debug_Camera(fTimeDelta);
		break;
	}

	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;;

	return OBJ_NOEVENT;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

}

HRESULT CCamera_Dynamic::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	return S_OK;
}

void CCamera_Dynamic::Set_CamMode(CAMERAMODE _eCamMode)
{
	if (_eCamMode == m_eCamMode)
		return;

	if (m_ePreCamMode != m_eCamMode)
		m_ePreCamMode = m_eCamMode;
	m_eCamMode = _eCamMode;
}

void CCamera_Dynamic::Set_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

void CCamera_Dynamic::Debug_Camera(_float fTimeDelta)
{

	if (GetKeyState('W') < 0)
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}

	if (GetKeyState('S') < 0)
	{
		m_pTransform->Go_Backward(fTimeDelta);
	}

	if (GetKeyState('A') < 0)
	{

		m_pTransform->Go_Left(fTimeDelta);
	}

	if (GetKeyState('D') < 0)
	{

		m_pTransform->Go_Right(fTimeDelta);
	}

	if (GetKeyState('Q') < 0)
	{
		_vector Axis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		m_pTransform->Turn(Axis, -fTimeDelta);
	}

	if (GetKeyState('E') < 0)
	{
		_vector Axis = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		m_pTransform->Turn(Axis, fTimeDelta);
	}

	if (GetKeyState('1') < 0)
	{

		_vector Axis = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		m_pTransform->Turn(Axis, -fTimeDelta);
	}

	if (GetKeyState('2') < 0)
	{
		_vector Axis = m_pTransform->Get_State(CTransform::STATE_RIGHT);
		m_pTransform->Turn(Axis, fTimeDelta);
	}

}

void CCamera_Dynamic::Player_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pTarget = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	Safe_AddRef(pTarget);
	_vector m_TargetPos = pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	Safe_Release(pTarget);
	m_TargetPos = m_TargetPos + XMVectorSet(m_vDistance.x, m_vDistance.y + m_fZoom, m_vDistance.z + m_fZoom, 0.f);

	m_pTransform->Go_PosLerp(fTimeDelta, m_TargetPos, 1.f);
	RELEASE_INSTANCE(CGameInstance);
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
