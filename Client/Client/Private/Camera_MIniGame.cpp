#include "stdafx.h"
#include "..\Public\Camera_MiniGame.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
#include "BattleManager.h"
#include "Monster.h"

CCamera_MiniGame::CCamera_MiniGame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_MiniGame::CCamera_MiniGame(const CCamera_MiniGame & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_MiniGame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_MiniGame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;

	//m_vDistance = ((CAMERADESC_DERIVED*)pArg)->vDistance;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&((CAMERADESC_DERIVED*)pArg)->InitPostion));
	m_OriginPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	return S_OK;
}

int CCamera_MiniGame::Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_MINIGAME)
		return OBJ_NOEVENT;

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
	case Client::CCamera_MiniGame::MINIGAME_SLASH:
		MiniGameSlash_Camera(fTimeDelta);
		break;
	case Client::CCamera_MiniGame::CAM_DEBUG:
		Debug_Camera(fTimeDelta);
		break;
	case Client::CCamera_MiniGame::CAM_TARGETMODE:
		TargetTool_Camera(fTimeDelta);
		break;
	case Client::CCamera_MiniGame::CAM_TARGETMODE_OFF:
		TargetTool_CameraOff(fTimeDelta);
		break;
	}

	if (m_bShakingMode)
		Shaking_Camera(fTimeDelta);


	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;;

	return OBJ_NOEVENT;
}

void CCamera_MiniGame::Late_Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_MINIGAME)
		return;

	__super::Late_Tick(fTimeDelta);
}

HRESULT CCamera_MiniGame::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_MiniGame::Set_CamMode(CAMERAMODE _eCamMode)
{
	if (_eCamMode == m_eCamMode)
		return;

	if (m_ePreCamMode != m_eCamMode)
		m_ePreCamMode = m_eCamMode;

	if( _eCamMode ==  CAM_TARGETMODE)
		m_vLasrDirwithPlayer = (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransform->Get_State(CTransform::STATE_TRANSLATION));

	m_vDistance = (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
	m_eCamMode = _eCamMode;
	m_fTime = 0.f;
	m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_bLerp = true;
}

void CCamera_MiniGame::Set_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_OriginPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
}


void CCamera_MiniGame::Set_Play(_bool type)
{
	m_bPlay = type;

	if (m_bPlay)
	{
		m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

		_vector vCameraLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
		_vector vCameraCurrentPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
		_vector vCurrentAt = vCameraCurrentPos + vCameraLook;
	}
}

void CCamera_MiniGame::Set_ShakingMode(_bool type, _float fPower, _float fMinusPower)
{
	 m_bShakingMode = type; 
	 m_fVelocity = fPower;
	 m_fMinusVelocity = fMinusPower;
	 m_vShakingStartPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
}

void CCamera_MiniGame::TargetTool_Camera(_float fTimeDelta)
{
	m_fTime += fTimeDelta;

	_float fStartTime = m_CamDatas[m_iIndex].fStartTime* m_fPlayTime;
	_float fEndTime = m_CamDatas[m_iIndex].fEndTime* m_fPlayTime;
	_float fValue = (m_fTime - fStartTime) / (fEndTime - fStartTime);

	_vector pTargetLook = m_pTarget->Get_TransformState(CTransform::STATE_LOOK);
	_vector pTargetPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector pPlayertoCameraDir;

	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_CamDatas[m_iIndex].fRadian);
	pPlayertoCameraDir = XMVector3TransformNormal(pTargetLook, RotationMatrix);
	_vector vCameraPos = pTargetPosition + pPlayertoCameraDir*m_CamDatas[m_iIndex].fLength;
	vCameraPos = XMVectorSetY(vCameraPos, XMVectorGetY(vCameraPos) + m_CamDatas[m_iIndex].fYoffset + m_fCameraOffsetY);


	_vector FinalPos = XMVectorLerp(m_vInitPos, vCameraPos, fValue); //_float4 저장 y올리기 
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);


	_vector vCenterPos = XMVectorSetY(pTargetPosition, XMVectorGetY(pTargetPosition) + m_fLookOffsetY);
	m_pTransform->LookAt(vCenterPos);
	

	if (m_fTime >= fEndTime)
	{
		m_iIndex++;
		m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);

		_vector vCameraLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
		_vector vCameraCurrentPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
		_vector vCurrentAt = vCameraCurrentPos + XMVectorSetW(vCameraLook, 0.f);

		if (m_iIndex >= m_CamDatas.size())
		{
			m_iIndex = 0;
			m_bPlay = false;
			Set_CamMode(CAM_TARGETMODE_OFF);
			m_CamDatas.clear();
		}
	}
}

void CCamera_MiniGame::TargetTool_CameraOff(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_vNewPos = vPlayerPosition - m_vLasrDirwithPlayer;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 저장 y올리기 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
		Set_CamMode(CAM_ROOM);
	}

	_vector vCenterPos = XMVectorSetY(vPlayerPosition, XMVectorGetY(vPlayerPosition) + m_fLookOffsetY);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	m_pTransform->LookAt(vCenterPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_MiniGame::Room_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (m_pTarget == nullptr)
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = (vCameraPosition - m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_lMouseWheel = pGameInstance->Get_DIMMoveState(DIMM_WHEEL);

	ZoomSetting(-5.f, 0.25f);

	if (XMouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
	{
		m_bLerp = true;

		if (CBattleManager::Get_Instance()->Get_IsBattleMode() == false)
		{
			if (XMouseMove < 0)
			{
				m_fAngle += 4.f;
				if (m_fAngle >= 360.f)
					m_fAngle = 0.f;
			}
			else if (XMouseMove > 0)
			{
				m_fAngle -= 4.f;
				if (m_fAngle <= 0.f)
					m_fAngle = 360.f;
			}
		}


	}
	// 항상 플레이어 위치 바라보게 하기
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	_vector vCenterPos = vPlayerPosition;
	vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	fLength = 5.f; //XMVectorGetX(XMVector3Normalize(vPosition - vCenterPos));
	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;

	if (YMouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
	{
		m_bLerp = true;


		if (CBattleManager::Get_Instance()->Get_IsBattleMode() == false)
		{

			if (YMouseMove > 0)
			{
				m_fCameraOffsetY += 0.3f;
				m_fLookOffsetY -= 0.1f;
			}
			else if (YMouseMove < 0)
			{
				m_fCameraOffsetY -= 0.3f;
				m_fLookOffsetY += 0.1f;
			}
		}

		if (m_fCameraOffsetY >= 8.f)
			m_fCameraOffsetY = 8.f;
		else if (m_fCameraOffsetY <= -2.f)
			m_fCameraOffsetY = -2.f;

		if (m_fLookOffsetY >= 6.f)
			m_fLookOffsetY = 6.f;
		else if (m_fLookOffsetY <= 4.f)
			m_fLookOffsetY = 4.f;

	}

	vPlayerPosition = XMVectorSetY(vPlayerPosition, XMVectorGetY(vPlayerPosition) + m_fLookOffsetY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vPlayerPosition) + m_fCameraOffsetY);


	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), (m_vNewPos + vZoomDir*m_fZoomOffset), m_fTime); //_float4 저장 y올리기 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = m_vNewPos + vZoomDir*m_fZoomOffset;
		m_fTime = 0.f;
	}



	if (XMVectorGetX(FinalPos) < -5.5f)
		FinalPos = XMVectorSetX(FinalPos, -5.5f);

	if (XMVectorGetX(FinalPos) > 41.f)
		FinalPos = XMVectorSetX(FinalPos, 41.f);

	if (XMVectorGetZ(FinalPos) < -5.5f)
		FinalPos = XMVectorSetZ(FinalPos, -5.5f);

	if (XMVectorGetZ(FinalPos) > 53.f)
		FinalPos = XMVectorSetZ(FinalPos, 53.f);


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	m_pTransform->LookAt(vPlayerPosition);




	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_MiniGame::Debug_Camera(_float fTimeDelta)
{
	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_SPACE))
		m_bStop = !m_bStop;


	if (!m_bStop)
	{
		/* Camera Inputs */
		if (pGameInstance->Key_Pressing(DIK_UP))
			m_pTransform->Go_Straight(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_DOWN))
			m_pTransform->Go_Backward(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_LEFT))
			m_pTransform->Go_Left(fTimeDelta * 2);
		if (pGameInstance->Key_Pressing(DIK_RIGHT))
			m_pTransform->Go_Right(fTimeDelta * 2);

		_long MouseMove = 0;
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
			m_pTransform->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * 0.1f);
		if (MouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
			m_pTransform->Turn(m_pTransform->Get_State(CTransform::STATE_RIGHT), fTimeDelta * MouseMove * 0.1f);
	}


	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_MiniGame::MiniGameSlash_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(18.f, 18.f, 20.f, 1.f));
	m_pTransform->LookAt(XMVectorSet(18, 0.f, 31.f, 1.f));


	RELEASE_INSTANCE(CGameInstance);
}


void CCamera_MiniGame::ZoomSetting(_float fDistance, _float fSpeed)
{
	if (m_bZoom)
	{
		m_fZoomOffset -= fSpeed;
		if (m_fZoomOffset <= fDistance)
			m_fZoomOffset = fDistance;
	}
	else
	{
		m_fZoomOffset += fSpeed;
		if (m_fZoomOffset >= 0.f)
			m_fZoomOffset = 0.f;
	}
	
	/* Zoom Blur */
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (m_pTarget)
	{
		_float fFocusPower = 3.f;

		_float fInterpFactor = m_fZoomOffset / -3.f;
		_int iFocusDetailLerp = 1 + fInterpFactor * (7 - 1);

		if (m_fZoomOffset == 0.f)
		{
			if (!m_bBlurResetted)
			{
				m_pTarget->Get_Renderer()->Set_ZoomBlur(false);
				m_bBlurResetted = true;
			}
		}
		else
		{
			m_pTarget->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);	
			m_bBlurResetted = false;
		}
	}
}

void CCamera_MiniGame::Shaking_Camera(_float fTimeDelta)
{
	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vCenterPos = vPlayerPosition;

	++m_iShakingCount;
	if (m_iShakingCount % 4 == 0)
	{
		m_fAngle -= m_fVelocity;

		if (rand() % 2 == 0)
			m_fCameraOffsetY -= m_fVelocity*0.1f;
		else
			m_fCameraOffsetY += m_fVelocity*0.1f;
	}
	else if (m_iShakingCount % 4 == 1)
	{
		m_fAngle += m_fVelocity;

		if (rand() % 2 == 0)
			m_fCameraOffsetY -= m_fVelocity*0.1f;
		else
			m_fCameraOffsetY += m_fVelocity*0.1f;
	}
	

	m_fVelocity -= m_fMinusVelocity;

	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;

	vCenterPos = XMVectorSetY(vCenterPos, XMVectorGetY(vCenterPos) + m_fLookOffsetY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vCenterPos) + m_fCameraOffsetY);

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 저장 y올리기 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
	}

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);


	m_fVelocity -= m_fMinusVelocity;
	if (m_fVelocity < 0.0f)
	{
		m_fVelocity = 0.f;
		m_iShakingCount = 0;
		m_bShakingMode = false;
	}


}


CCamera_MiniGame * CCamera_MiniGame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_MiniGame* pInstance = new CCamera_MiniGame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_MiniGame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_MiniGame::Clone(void* pArg)
{
	CCamera_MiniGame* pInstance = new CCamera_MiniGame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_MiniGame"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_MiniGame::Free()
{
	__super::Free();
}