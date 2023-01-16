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
	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

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

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Player_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	_vector vPlayerPosition = pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_pTransform->LookAt(vPlayerPosition);

	vPlayerPosition = vPlayerPosition + XMVectorSet(m_vDistance.x, m_vDistance.y + m_fZoom, m_vDistance.z + m_fZoom, 0.f);
	m_pTransform->Go_PosLerp(fTimeDelta, vPlayerPosition, 1.f);

	RELEASE_INSTANCE(CGameInstance);
}

CCamera_Dynamic * CCamera_Dynamic::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Dynamic::Clone(void* pArg)
{
	CCamera_Dynamic* pInstance = new CCamera_Dynamic(*this);

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