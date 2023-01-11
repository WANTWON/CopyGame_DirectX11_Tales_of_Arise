#include "stdafx.h"
#include "..\Public\Camera_2D.h"
#include "GameInstance.h"
#include "Player.h"
#include "CameraManager.h"

CCamera_2D::CCamera_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_2D::CCamera_2D(const CCamera_2D & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_2D::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CCamera_2D::Initialize(void* pArg)
{
	if (pArg != nullptr)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERADESC_DERIVED));

	if (FAILED(__super::Initialize(&((CAMERADESC_DERIVED*)pArg)->CameraDesc)))
		return E_FAIL;
	
	m_vDistance = ((CAMERADESC_DERIVED*)pArg)->CameraDesc.vEye;
	m_pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&((CAMERADESC_DERIVED*)pArg)->InitPostion));
	return S_OK;
}

int CCamera_2D::Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_2D)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	Camera_2D(fTimeDelta);

	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;

	return OBJ_NOEVENT;
}

void CCamera_2D::Late_Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_2D)
		return;

	__super::Late_Tick(fTimeDelta);
}

HRESULT CCamera_2D::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera_2D::Camera_2D(_float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	if (pGameInstance->Key_Pressing(DIK_F1))
	{
		m_pTransform->Go_Straight(fTimeDelta);
	}
	if (pGameInstance->Key_Pressing(DIK_F2))
	{
		m_pTransform->Go_Backward(fTimeDelta);
	}

	CPlayer* pTarget = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	_float2 vTargetProjPos = pTarget->Get_ProjPosition();
	m_pTransform->LookDir(XMVectorSet(0.f, 0.f, 1.f, 0.f));

	if (vTargetProjPos.x > g_iWinSizeX || vTargetProjPos.x < 0)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	_float fPosX = XMVectorGetX(m_pTransform->Get_State(CTransform::STATE_POSITION));
	if (fPosX <= m_CameraDesc.fMaxXPos && fPosX >= m_CameraDesc.fMinXPos)
	{
		if (vTargetProjPos.x < 400)
			m_pTransform->Go_Left(fTimeDelta);
			

		if (vTargetProjPos.x > 800)
			m_pTransform->Go_Right(fTimeDelta);
	}
	else
	{
		_vector vPosition = m_pTransform->Get_State(CTransform::STATE_POSITION);

		if (fPosX > m_CameraDesc.fMaxXPos)
			vPosition = XMVectorSetX(vPosition, m_CameraDesc.fMaxXPos);
		if (fPosX < m_CameraDesc.fMinXPos)
			vPosition = XMVectorSetX(vPosition, m_CameraDesc.fMinXPos);

		m_pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
	}

	
	RELEASE_INSTANCE(CGameInstance);
}

CCamera_2D * CCamera_2D::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_2D*	pInstance = new CCamera_2D(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_2D::Clone(void* pArg)
{
	CCamera_2D*	pInstance = new CCamera_2D(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Dynamic"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_2D::Free()
{
	__super::Free();

	
}
