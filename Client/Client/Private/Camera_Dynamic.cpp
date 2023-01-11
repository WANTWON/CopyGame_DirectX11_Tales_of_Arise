#include "stdafx.h"
#include "..\Public\Camera_Dynamic.h"
#include "GameInstance.h"
#include "Player.h"
#include "CameraManager.h"


CCamera_Dynamic::CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Dynamic::CCamera_Dynamic(const CCamera_Dynamic & rhs)
	: CCamera(rhs)
{
}

void CCamera_Dynamic::Set_CamMode(CAMERAMODE _eCamMode)
{
	if (_eCamMode == m_eCamMode)
		return;

	if (m_ePreCamMode != m_eCamMode)
		m_ePreCamMode = m_eCamMode;
	m_eCamMode = _eCamMode; 
	m_fTime = 0.f;
	m_bSetCamLook = false;
	m_dwSettingTime = GetTickCount();
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
	
	m_vDistance = ((CAMERADESC_DERIVED*)pArg)->CameraDesc.vEye;
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((CAMERADESC_DERIVED*)pArg)->InitPostion));
	return S_OK;
}

int CCamera_Dynamic::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	switch (m_eCamMode)
	{
	case Client::CCamera_Dynamic::CAM_PLAYER:
		Player_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_SHAKING:
		Shaking_Camera(fTimeDelta, m_fPower);
		break;
	case Client::CCamera_Dynamic::CAM_TERRAIN:
		Terrain_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_ITEMGET:
		Item_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_TALK:
		Talk_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_ROOM:
		Room_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_CRANE_GAME:
		CraneGame_Camera(fTimeDelta);
		break;
	case Client::CCamera_Dynamic::CAM_TARGET:
		Target_Camera(fTimeDelta);
		break;
	default:
		break;
	}

	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_DYNAMIC)
		return OBJ_NOEVENT;

	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;

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

void CCamera_Dynamic::Player_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_F1))
	{
		m_vDistance.y -= 0.03f;
	}
	if (pGameInstance->Key_Pressing(DIK_F2))
	{
		m_vDistance.y += 0.03f;
	}

	CPlayer* pTarget = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	Safe_AddRef(pTarget);
	_vector m_TargetPos = pTarget->Get_TransformState(CTransform::STATE_POSITION);

	Safe_Release(pTarget);
	
	m_pTransform->Go_PosLerp(fTimeDelta, m_TargetPos, 1.f, XMVectorSet(m_vDistance.x, m_vDistance.y + m_fZoom, m_vDistance.z + m_fZoom, 0.f));
	
	if (m_dwSettingTime + 2000 < GetTickCount())
		m_bSetCamLook = true;
		
	if (m_bSetCamLook == false)
	{
		m_TargetPos = XMVectorSetX(m_TargetPos, XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_POSITION)));
		m_pTransform->LookAt(m_TargetPos);
			
	}

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Shaking_Camera(_float fTimeDelta, _float fPower)
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CBaseObj* pTarget = nullptr;
	_float3 vTargetPos;

	switch (m_ePreCamMode)
	{
	case Client::CCamera_Dynamic::CAM_PLAYER:
		pTarget = (CBaseObj*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		XMStoreFloat3(&vTargetPos, pTarget->Get_TransformState(CTransform::STATE_POSITION));
		break;
	case Client::CCamera_Dynamic::CAM_TERRAIN:
		XMStoreFloat3(&vTargetPos, XMLoadFloat4(&m_fTargetPos));
		break;
	}
	

	++m_iShakingCount;
	if (m_iShakingCount % 4 == 0)
	{
		vTargetPos.y += fPower*m_fVelocity;
		if (rand() % 2 == 0)
			vTargetPos.z -= fPower*m_fVelocity;
		else
			vTargetPos.z += fPower*m_fVelocity;

		if (rand() % 2 == 0)
			vTargetPos.x -= fPower*m_fVelocity;
		else
			vTargetPos.x += fPower*m_fVelocity;
	}
	else if (m_iShakingCount % 4 == 1)
	{
		vTargetPos.y -= fPower*m_fVelocity;
		if (rand() % 2 == 0)
			vTargetPos.z -= fPower*m_fVelocity;
		else
			vTargetPos.z += fPower*m_fVelocity;

		if (rand() % 2 == 0)
			vTargetPos.x -= fPower*m_fVelocity;
		else
			vTargetPos.x += fPower*m_fVelocity;

	}

	m_fVelocity -= m_fMinusVelocity;
	if (m_fVelocity < 0.0f)
	{

		switch (m_ePreCamMode)
		{
		case Client::CCamera_Dynamic::CAM_PLAYER:
			m_eCamMode = CAM_PLAYER;
			break;
		case Client::CCamera_Dynamic::CAM_TERRAIN:
			m_eCamMode = CAM_TERRAIN;
			break;
		}
		m_ePreCamMode = m_eCamMode;
		Safe_Release(pGameInstance);
		return;
	}

	_vector vecTargetPos = XMLoadFloat3(&vTargetPos);
	vecTargetPos = XMVectorSetW(vecTargetPos, 1.f);

	m_pTransform->LookAt(vecTargetPos);
	m_pTransform->Go_PosLerp(fTimeDelta, vecTargetPos, 1.f, XMVectorSet(m_vDistance.x, m_vDistance.y + m_fZoom, m_vDistance.z, 0.f));
	//m_pTransform->Follow_Target(fTimeDelta, vecTargetPos, XMVectorSet(m_vDistance.x, m_vDistance.y, m_vDistance.z + m_fZoom, 0.f));
	Safe_Release(pGameInstance);
}

void CCamera_Dynamic::Terrain_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	list<CGameObject*>*  pBackgroundList = pGameInstance->Get_ObjectList(iLevel, TEXT("Layer_BackGround"));
	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	
	_vector pPlayerPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);

	_float fMinDistance = 9999;



	for (auto& iter : *pBackgroundList)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(pPlayerPos - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION)));

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			XMStoreFloat4(&m_fTargetPos, dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION));
			
		}
	}

	
	m_pTransform->Go_PosLerp(fTimeDelta, XMLoadFloat4(&m_fTargetPos), 1.f, XMVectorSet(m_vDistance.x, m_vDistance.y + m_fZoom, m_vDistance.z, 0.f));

	
	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Room_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	list<CGameObject*>*  pBackgroundList = pGameInstance->Get_ObjectList(iLevel, TEXT("Layer_BackGround"));
	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	_vector pPlayerPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);

	_float fMinDistance = 9999;


	if (pGameInstance->Key_Pressing(DIK_F1))
	{
		m_vDistance.y -= 0.03f;
	}
	if (pGameInstance->Key_Pressing(DIK_F2))
	{
		m_vDistance.y += 0.03f;
	}


	for (auto& iter : *pBackgroundList)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(pPlayerPos - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION)));

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			XMStoreFloat4(&m_fTargetPos, dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION));

		}
	}

	//m_fTargetPos.x = 0.f;
	m_pTransform->LookAt(XMLoadFloat4(&m_fTargetPos));
	//_vector Position = XMVectorSet(0.f, 11.8f, -5.5f, 1.f);
	//m_pTransform->Go_PosLerp(fTimeDelta, Position, 1.f);
	m_pTransform->Go_PosLerp(fTimeDelta, XMLoadFloat4(&m_fTargetPos), 1.f, XMVectorSet(m_vDistance.x, m_vDistance.y, m_vDistance.z, 0.f));


	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Item_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pTarget = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	_vector m_TargetPos = pTarget->Get_TransformState(CTransform::STATE_POSITION);
	_vector vOffsetPos = XMVectorSet(0.f,0.f,0.f,1.f);

	if(pGameInstance->Get_CurrentLevelIndex() == LEVEL_TOWER)
		vOffsetPos = m_TargetPos + XMVectorSet(0, m_vDistance.y + 2, m_vDistance.z, 0.f);
	else
		vOffsetPos = m_TargetPos + XMVectorSet(0, m_vDistance.y - 2, m_vDistance.z, 0.f);
	
	m_pTransform->Go_PosLerp(fTimeDelta, vOffsetPos, 1.f);

	if (m_dwSettingTime + 2000 < GetTickCount())
		m_bSetCamLook = true;

	if (m_bSetCamLook == false)
	{
		m_TargetPos = XMVectorSetX(m_TargetPos, XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_POSITION)));
		m_pTransform->LookAt(m_TargetPos);

	}

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Talk_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pTarget = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	_vector m_TargetPos = pTarget->Get_TransformState(CTransform::STATE_POSITION);
	m_TargetPos = m_TargetPos + XMVectorSet(0.f, 2.f, 0.f, 0.f);

	_vector vOffsetPos = m_TargetPos + XMVectorSet(0, 3, -7, 0.f);

	m_pTransform->LookAt(m_TargetPos);
	m_pTransform->Go_PosLerp(fTimeDelta, vOffsetPos, 0.1f);

	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::CraneGame_Camera(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	LEVEL iLevel = (LEVEL)pGameInstance->Get_CurrentLevelIndex();

	list<CGameObject*>*  pBackgroundList = pGameInstance->Get_ObjectList(iLevel, TEXT("Layer_BackGround"));
	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	_vector pPlayerPos = pPlayer->Get_TransformState(CTransform::STATE_POSITION);

	_float fMinDistance = 9999;


	for (auto& iter : *pBackgroundList)
	{
		_float fDistance = XMVectorGetX(XMVector3Length(pPlayerPos - dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION)));

		if (fDistance < fMinDistance)
		{
			fMinDistance = fDistance;
			XMStoreFloat4(&m_fTargetPos, dynamic_cast<CBaseObj*>(iter)->Get_TransformState(CTransform::STATE_POSITION));

		}
	}
	m_fTargetPos.x -= 2.f;
	m_fTargetPos.z += m_vDistance.z + 3.f;
	m_fTargetPos.y += 11.f;

	m_pTransform->Go_PosLerp(fTimeDelta, XMLoadFloat4(&m_fTargetPos), 1.f);


	RELEASE_INSTANCE(CGameInstance);
}

void CCamera_Dynamic::Target_Camera(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);



	m_pTransform->Go_PosLerp(fTimeDelta, XMLoadFloat4(&m_fTargetPos), 0.8f, XMVectorSet(m_vDistance.x, 0.f, m_vDistance.z, 0.f));
	if (m_dwSettingTime + 2000 < GetTickCount())
		m_bSetCamLook = true;

	if (m_bSetCamLook == false)
	{
		_vector m_TargetPos = XMVectorSetX(XMLoadFloat4(&m_fTargetPos), XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_POSITION)));
		m_pTransform->LookAt(m_TargetPos);

	}

	RELEASE_INSTANCE(CGameInstance);
}


void CCamera_Dynamic::Set_Position(_vector vPosition)
{
	m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
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
