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

	
	m_vLastPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	m_vDistance = (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) - m_pTransform->Get_State(CTransform::STATE_TRANSLATION));
	m_eCamMode = _eCamMode;
	m_fAngle = 0.f;
	m_fTime = 0.f;
	m_bLerp = true;
}

void CCamera_Dynamic::Set_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_OriginPos = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
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

void CCamera_Dynamic::Player_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (m_pTarget == nullptr)
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = (vCameraPosition - m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	m_lMouseWheel = pGameInstance->Get_DIMMoveState(DIMM_WHEEL);

	if (m_bZoom)
	{
		m_fZoom -= 0.1f;
		if (m_fZoom <= -3.f)
			m_fZoom = -3.f;
	}
	else
	{
		m_fZoom += 0.1f;
		if (m_fZoom >= 0.f)
			m_fZoom = 0.f;
	}


	if (XMouseMove = pGameInstance->Get_DIMMoveState(DIMM_X))
	{
		m_bLerp = true;
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

	// 항상 플레이어 위치 바라보게 하기
	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);

	_vector vCenterPos = vPlayerPosition;
	vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = 5.f; //XMVectorGetX(XMVector3Normalize(vPosition - vCenterPos));
	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;

	if (YMouseMove = pGameInstance->Get_DIMMoveState(DIMM_Y))
	{
		m_bLerp = true;
		if (YMouseMove > 0)
		{
			m_fCameraY += 0.3f;
			m_fOffsetPosY -= 0.1f;
		}
		else if (YMouseMove < 0)
		{
			m_fCameraY -= 0.3f;
			m_fOffsetPosY += 0.1f;
		}

		if (m_fCameraY >= 8.f)
			m_fCameraY = 8.f;
		else if (m_fCameraY <= -2.f)
			m_fCameraY = -2.f;

		if (m_fOffsetPosY >= 6.f)
			m_fOffsetPosY = 6.f;
		else if (m_fOffsetPosY <= 4.f)
			m_fOffsetPosY = 4.f;

	}

	vPlayerPosition = XMVectorSetY(vPlayerPosition, XMVectorGetY(vPlayerPosition) + m_fOffsetPosY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vPlayerPosition) + m_fCameraY);


	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), (m_vNewPos + vZoomDir*m_fZoom), m_fTime); //_float4 저장 y올리기 

		if (m_fTime >= 1.f)
			m_bLerp = false;
	}
	else
	{
		_vector vZoomDir = XMVector3Normalize(vPlayerPosition - m_vNewPos);
		FinalPos = m_vNewPos + +vZoomDir*m_fZoom;
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

	// 락온 몬스터가 있으면 항상 플레이어와 락온이 화면에 들어오게 하기
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

		if (pLockOnMonster->Check_IsinFrustum() == false)
			m_bTurn = true;

		if(m_bTurn)
		{
			if (fDot < 0.8f)
			{
				if (fRightDot > 0.f)
					m_fAngle -= (1 - fDot)*10.f;// 3.f;
				else
					m_fAngle += (1 - fDot)*10.f;// 3.f;
			}
			else
				m_bTurn = false;
		}
		
	}

	vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = 5.f; 
	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;


	vCenterPos = XMVectorSetY(vCenterPos, XMVectorGetY(vCenterPos) + m_fOffsetPosY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vCenterPos) + m_fCameraY);

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

	_vector vPosition = m_vTargetPos - m_vDistance ;

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - vPosition)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;
	else
		m_bLerp = true;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), vPosition, m_fTime); //_float4 저장 y올리기 

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

	//if (m_pTarget == nullptr)
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = (vCameraPosition - m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vCenterPos = vPlayerPosition;

	vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = 5.f;
	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;


	vCenterPos = XMVectorSetY(vCenterPos, XMVectorGetY(vCenterPos) + m_fOffsetPosY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vCenterPos) + m_fCameraY);

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
		Set_CamMode(CAM_BATTLEZONE);
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::AIBoostOn_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_vector vPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION) - m_vDistance;

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - vPosition)) <= 0.2f && m_fTime <= 0.2f)
		m_bLerp = false;
	else
		m_bLerp = true;

	_vector FinalPos = { 0.f,0.f,0.f,0.f };
	if (m_bLerp)
	{
		m_fTime += fTimeDelta*0.3f;

		FinalPos = XMVectorLerp(m_pTransform->Get_State(CTransform::STATE_TRANSLATION), vPosition, m_fTime); //_float4 저장 y올리기 

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
}

void CCamera_Dynamic::AIBoostOff_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	//if (m_pTarget == nullptr)
	m_pTarget = CPlayerManager::Get_Instance()->Get_ActivePlayer();

	_vector vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = (vCameraPosition - m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION));

	_vector vPlayerPosition = m_pTarget->Get_TransformState(CTransform::STATE_TRANSLATION);
	_vector vCenterPos = vPlayerPosition;

	vCameraPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
	_float fLength = 5.f;
	vCameraPosition = XMVectorSetX(vCameraPosition, (XMVectorGetX(vCenterPos) + cosf(XMConvertToRadians(m_fAngle))*fLength - sin(XMConvertToRadians(m_fAngle))*fLength));
	vCameraPosition = XMVectorSetZ(vCameraPosition, (XMVectorGetZ(vCenterPos) + sin(XMConvertToRadians(m_fAngle))*fLength + cos(XMConvertToRadians(m_fAngle))*fLength));
	m_vNewPos = vCameraPosition;


	vCenterPos = XMVectorSetY(vCenterPos, XMVectorGetY(vCenterPos) + m_fOffsetPosY);
	m_vNewPos = XMVectorSetY(vCameraPosition, XMVectorGetY(vCenterPos) + m_fCameraY);

	if (XMVectorGetX(XMVector4Length(m_pTransform->Get_State(CTransform::STATE_TRANSLATION) - m_vNewPos)) <= 0.2f )
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
		Set_CamMode(CAM_BATTLEZONE);
	}


	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, FinalPos);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Change_LockOn(_uchar eKeyID)
{
	CBaseObj* pLockOnMonster = CBattleManager::Get_Instance()->Get_LackonMonster();
	vector<CBaseObj*> vecMonster = CBattleManager::Get_Instance()->Get_AllMonster();

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