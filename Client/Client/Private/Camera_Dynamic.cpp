#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "Player.h"
#include "PlayerManager.h"
#include "BattleManager.h"
#include "Monster.h"

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

	//m_vDistance = ((CAMERADESC_DERIVED*)pArg)->vDistance;
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, XMLoadFloat4(&((CAMERADESC_DERIVED*)pArg)->InitPostion));
	m_OriginPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	return S_OK;
}

int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_DYNAMIC)
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
	case Client::CCamera_Dynamic::CAM_PLAYER:
		MiniGameSlash_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_DEBUG:
		Debug_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_BATTLEZONE:
		Battle_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_BATTLE_CLEAR:
		BattleClear_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_LOCKON:
		LockOn_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_LOCKOFF:
		LockOff_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_AIBOOSTON:
		AIBoostOn_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_AIBOOSTOFF:
		AIBoostOff_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_TARGETMODE:
		TargetTool_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_TARGETMODE_OFF:
		TargetTool_CameraOff(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_ROOM:
		Room_Camera(fTimeDelta);
		break;
	}

	if (m_bShakingMode)
		Shaking_Camera(fTimeDelta);


	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;;

	return OBJ_NOEVENT;
}

void CCamera_Dynamic::Late_Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_DYNAMIC)
		return;

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

	if(_eCamMode == CAM_AIBOOSTON || _eCamMode == CAM_LOCKON || _eCamMode ==  CAM_TARGETMODE)
		m_vLasrDirwithPlayer = (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransform->Get_State(CTransform::STATE_TRANSLATION));

	m_vDistance = (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
	m_eCamMode = _eCamMode;
	m_fTime = 0.f;
	m_vInitPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_bLerp = true;
}

void CCamera_Dynamic::Set_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_OriginPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
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

		//m_vInitAt = vCurrentAt;
		//m_vInitAt = vCameraLook;
	}
}

void CCamera_Dynamic::Set_ShakingMode(_bool type, _float fPower, _float fMinusPower)
{
	 m_bShakingMode = type; 
	 m_fVelocity = fPower;
	 m_fMinusVelocity = fMinusPower;
	 m_vShakingStartPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
}

void CCamera_Dynamic::TargetTool_Camera(_float fTimeDelta)
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


	_vector FinalPos = XMVectorLerp(m_vInitPos, vCameraPos, fValue); //_float4 ���� y�ø��� 
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	//_vector vAtPosition = pTargetPosition + XMLoadFloat4(&m_CamDatas[m_iIndex].vLookDir);
	//_vector vAtPosition = pTargetPosition + XMLoadFloat4(&m_CamDatas[m_iIndex].vLook);
	//_vector vAtPosition = XMVectorSetY(pTargetPosition, XMVectorGetY(pTargetPosition) + m_CamDatas[m_iIndex].fYoffset*0.5f);
//	_vector vAt = XMVectorLerp(m_vInitAt, vAtPosition, fValue);
	_vector vCenterPos = XMVectorSetY(pTargetPosition, XMVectorGetY(pTargetPosition) + m_fLookOffsetY);
	m_pTransform->LookAt(vCenterPos);
	//m_pTransform->LookDir(vAt);


	if (m_fTime >= fEndTime)
	{
		//m_vInitAt = vAtPosition;
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
			Set_CamMode(CAM_TARGETMODE_OFF);
			m_CamDatas.clear();
		}
	}
}

void CCamera_Dynamic::TargetTool_CameraOff(_float fTimeDelta)
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

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
		Set_CamMode(CAM_BATTLEZONE);
	}

	_vector vCenterPos = XMVectorSetY(vPlayerPosition, XMVectorGetY(vPlayerPosition) + m_fLookOffsetY);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	m_pTransform->LookAt(vCenterPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Room_Camera(_float fTimeDelta)
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
	// �׻� �÷��̾� ��ġ �ٶ󺸰� �ϱ�
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
		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), (m_vNewPos + vZoomDir*m_fZoomOffset), m_fTime); //_float4 ���� y�ø��� 

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

void CCamera_Dynamic::Debug_Camera(_float fTimeDelta)
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

void CCamera_Dynamic::MiniGameSlash_Camera(_float fTimeDelta)
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
	// �׻� �÷��̾� ��ġ �ٶ󺸰� �ϱ�
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
		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), (m_vNewPos + vZoomDir*m_fZoomOffset), m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = m_vNewPos +vZoomDir*m_fZoomOffset;
		m_fTime = 0.f;
	}



	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	m_pTransform->LookAt(vPlayerPosition);




	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Battle_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (m_pTarget == nullptr)
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vCenterPos = vPlayerPosition;

	ZoomSetting(-3.f, 1.f);

	// ���� ���Ͱ� ������ �׻� �÷��̾�� ������ ȭ�鿡 ������ �ϱ�
	CBaseObj* pLockOnMonster = CBattleManager::Get_Instance()->Get_LackonMonster();
	if (pLockOnMonster != nullptr)
	{
		_vector vLockOnPosition = pLockOnMonster->Get_TransformState(CTransform::STATE_TRANSLATION);
		_vector vRight = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_RIGHT));
		_vector vLook = XMVector3Normalize(m_pTransform->Get_State(CTransform::STATE_LOOK));
		_vector vPlayerLook = m_pTarget->Get_TransformState(CTransform::STATE_LOOK);

		_vector vCameraLockonDir = XMVector3Normalize(XMVectorSetY(vLockOnPosition - m_pTransform->Get_State(CTransform::STATE_TRANSLATION), 0.f));
		_vector vPlayerLockonDir = XMVector3Normalize(XMVectorSetY(vLockOnPosition - vPlayerPosition, 0.f));

		vLook = XMVectorSetY(vLook, 0.f);
		vPlayerLockonDir = XMVectorSetY(vPlayerLockonDir, 0.f);
		vCameraLockonDir = XMVectorSetY(vCameraLockonDir, 0.f);
		vRight = XMVectorSetY(vRight, 0.f);


		_float fDot = XMVectorGetX(XMVector3Dot(vPlayerLockonDir, vLook));
		_float fRightDot = XMVectorGetX(XMVector3Dot(vCameraLockonDir, vRight));
		_float fDistance = XMVectorGetX(XMVector3Length(vLockOnPosition - vPlayerPosition));
		//if (pLockOnMonster->Check_IsinBattleZoneFrustum() == false )
			m_bTurn = true;

		if(m_bTurn)
		{
			if (fDot < 0.9f)
			{
				if (fRightDot > 0.f)
				{
					if(fDot > 0.8 )
						m_fAngle -= (0.9f - fDot)* 2.f;
					else if (fDot > 0.5)
						m_fAngle -= (0.9f - fDot)* 3.f;
					else if (fDot > 0.3)
						m_fAngle -= (0.9f - fDot)* 5.f;
					else
						m_fAngle -= (0.8f - fDot)* 7.f;
				/*	else if(fDistance < 15.f)
						m_fAngle -= (0.8f - fDot)* 2.f;
					else
						m_fAngle -= (0.8f - fDot)* 5.f;*/
				}
				else
				{
					if (fDot > 0.8)
						m_fAngle += (0.9f - fDot)* 2.f;
					else if (fDot > 0.5)
						m_fAngle += (0.9f - fDot)* 3.f;
					else if (fDot > 0.3)
						m_fAngle += (0.9f - fDot)* 5.f;
					else
						m_fAngle += (0.9f - fDot)* 7.f;
				}
					
			}
			else
				m_bTurn = false;
				
		}


		
		if (fDistance > 30.f)
		{
			fLength -= 0.1f;
			m_fCameraOffsetY -= 0.2f;
			
		}
		else if (fDistance >= 15.f && fDistance <= 30.f)
		{
			if (YMouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
			{
				if (YMouseMove > 0)
					m_fCameraOffsetY += 0.2f;
				else if (YMouseMove < 0)
					m_fCameraOffsetY -= 0.2f;
			}

		}
		else if(fDistance < 15.f)
		{
			fLength += 0.1f;
			m_fCameraOffsetY += 0.2f;
		}

		if (fLength >= 8.f)
			fLength = 8.f;
		if (fLength <= 5.f)
			fLength = 5.f;
		if (m_fCameraOffsetY >= 5.f)
			m_fCameraOffsetY = 5.f;
		else if (m_fCameraOffsetY <= 1.f)
			m_fCameraOffsetY = 1.f;
	}

	


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

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 ���� y�ø��� 
		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = m_vNewPos + vZoomDir*m_fZoomOffset;
		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = m_vNewPos + vZoomDir*m_fZoomOffset;
		m_fTime = 0.f;
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	
	m_pTransform->LookAt(vCenterPos);



	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::BattleClear_Camera(_float fTimeDelta)
{
	m_fAngle += 0.05f;
	if (m_fAngle >= 360.f)
		m_fAngle = 0.f;

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = 4.f; 
	m_vNewPos = XMVectorSetX(m_vNewPos, (XMVectorGetX(m_vTargetPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = XMVectorSetZ(m_vNewPos, (XMVectorGetZ(m_vTargetPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = XMVectorSetY(m_vNewPos,  3.f);

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime);
		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
	}

	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	m_pTransform->LookAt(m_vTargetPos);

}

void CCamera_Dynamic::LockOn_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	g_fSoundVolume -= 0.01f;
	if (g_fSoundVolume <= 0.08f)
		g_fSoundVolume = 0.08f;

	_vector vPosition = m_vTargetPos - m_vDistance ;

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - vPosition)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;
	else
		m_bLerp = true;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), vPosition, m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = vPosition;
		m_fTime = 0.f;
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	//m_pTransform->LookAt(m_vTargetPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::LockOff_Camera(_float fTimeDelta)
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

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
		Set_CamMode(CAM_BATTLEZONE);
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::AIBoostOn_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	m_pTarget->Set_IsActionMode(true);
	_vector vPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vDistance;

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - vPosition)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;
	

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), vPosition, m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		FinalPos = vPosition;
		m_fTime = 0.f;
	}

	m_vLastLookPos = FinalPos + m_pTransform->Get_State(CTransform::STATE_LOOK);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	RELEASE_INSTANCE(CGameInstance);

}

void CCamera_Dynamic::AIBoostOff_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	m_vNewPos = vPlayerPosition - m_vLasrDirwithPlayer;


	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 ���� y�ø��� 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		m_pTarget->Set_IsActionMode(false);
		FinalPos = m_vNewPos;
		m_fTime = 0.f;
		Set_CamMode(CAM_BATTLEZONE);
	}

	vPlayerPosition = XMVectorSetY(vPlayerPosition, XMVectorGetY(vPlayerPosition) + m_fLookOffsetY);
	_vector vLookPos = XMVectorLerp(m_vLastLookPos, vPlayerPosition, m_fTime);
	//m_pTransform->LookAt(vLookPos);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);
	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Change_LockOn(_uchar eKeyID)
{
	CBaseObj* pLockOnMonster = CBattleManager::Get_Instance()->Get_LackonMonster();
	vector<CBaseObj*> vecMonster = CBattleManager::Get_Instance()->Get_BattleMonster();

	_int iIndex = 0;
	for (auto& iter : vecMonster)
	{
		if (iter == pLockOnMonster)
		{
			if (eKeyID == DIK_Z)
			{
				iIndex--;
				if (iIndex < 0)
					iIndex = vecMonster.size() - 1;

				m_bLerp = true;
				m_fTime = 0.f;
				CBattleManager::Get_Instance()->Set_LackonMonster(vecMonster[iIndex]);

			}
			else if (eKeyID == DIK_X)
			{
				iIndex++;
				if (iIndex >= vecMonster.size())
					iIndex = 0;

				m_bLerp = true;
				m_fTime = 0.f;
				CBattleManager::Get_Instance()->Set_LackonMonster(vecMonster[iIndex]);
			}

		}
		else
			iIndex++;

	}
}

void CCamera_Dynamic::ZoomSetting(_float fDistance, _float fSpeed)
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

void CCamera_Dynamic::Shaking_Camera(_float fTimeDelta)
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

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), m_vNewPos, m_fTime); //_float4 ���� y�ø��� 

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