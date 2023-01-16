#include "stdafx.h"
#include "..\Public\BaseObj.h"
#include "GameInstance.h"
#include "VIBuffer_Navigation.h"
#include "PipeLine.h"

CBaseObj::CBaseObj(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CBaseObj::CBaseObj(const CBaseObj & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBaseObj::Initialize_Prototype()
{


	return S_OK;
}

HRESULT CBaseObj::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

int CBaseObj::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CBaseObj::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

#ifdef _DEBUG
	if (m_pAABBCom != nullptr)
		m_pRendererCom->Add_Debug(m_pAABBCom);
	if (m_pOBBCom != nullptr)
		m_pRendererCom->Add_Debug(m_pOBBCom);
	if (m_pSPHERECom != nullptr)
		m_pRendererCom->Add_Debug(m_pSPHERECom);
#endif
}

HRESULT CBaseObj::Render()
{
	return S_OK;
}


_bool CBaseObj::Check_IsinFrustum(_float fOffset)
{
	_float3 vScale = Get_Scale();
	_float fCullingRadius = max(max(vScale.x, vScale.y), vScale.z);

	return CGameInstance::Get_Instance()->isIn_WorldFrustum(m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION), fCullingRadius + fOffset);
}

void CBaseObj::SetUp_BillBoard()
{
	_float4x4 ViewMatrix;

	ViewMatrix = CGameInstance::Get_Instance()->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW); // Get View Matrix
	_matrix matViewInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&ViewMatrix));      // Get Inverse of View Matrix (World Matrix of Camera)

	_vector vRight = (_vector)matViewInv.r[0];
	_vector vUp = (_vector)matViewInv.r[1];
	_vector vLook = (_vector)matViewInv.r[2];
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, XMVector3Normalize(vRight) *Get_Scale().x);
	m_pTransformCom->Set_State(CTransform::STATE_UP, XMVector3Normalize(vUp) * Get_Scale().y);
	m_pTransformCom->Set_State(CTransform::STATE_LOOK, XMVector3Normalize(vLook) *Get_Scale().z);
}


void CBaseObj::Update_Collider()
{
	if (m_pAABBCom != nullptr)
		m_pAABBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pOBBCom != nullptr)
		m_pOBBCom->Update(m_pTransformCom->Get_WorldMatrix());
	if (m_pSPHERECom != nullptr)
		m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());
}


_vector CBaseObj::Get_TransformState(CTransform::STATE eState)
{
	if (m_pTransformCom == nullptr)
		return _vector();


	return m_pTransformCom->Get_State(eState);
}

_float2 CBaseObj::Get_ProjPosition()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_TRANSLATION);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix ViewMatrix =  pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	vPosition = XMVector3TransformCoord(vPosition, ViewMatrix);
	vPosition = XMVector3TransformCoord(vPosition, ProjMatrix);

	_float ScreenX = XMVectorGetX(vPosition)* (g_iWinSizeX / 2) + (g_iWinSizeX / 2);
	_float ScreenY = XMVectorGetY(vPosition) * (g_iWinSizeY / 2) + (g_iWinSizeY / 2);

	RELEASE_INSTANCE(CGameInstance);

	return _float2( ScreenX, ScreenY);
}

void CBaseObj::Set_State(CTransform::STATE eState, _fvector vState)
{
	if (m_pTransformCom == nullptr)
		return;

	m_pTransformCom->Set_State(eState, vState);
	m_pAnimTransformCom->Set_State(eState, vState);
}

void CBaseObj::Set_Scale(_float3 vScale)
{
	m_vScale = vScale;

	m_pTransformCom->Set_Scale(CTransform::STATE_RIGHT, m_vScale.x);
	m_pTransformCom->Set_Scale(CTransform::STATE_UP, m_vScale.y);
	m_pTransformCom->Set_Scale(CTransform::STATE_LOOK, m_vScale.z);

}

CCollider * CBaseObj::Get_Collider()
{
	if (m_pAABBCom != nullptr)
		return m_pAABBCom;
	else if (m_pOBBCom != nullptr)
		return m_pOBBCom;
	else if (m_pSPHERECom != nullptr)
		return m_pSPHERECom;
	else
		return nullptr;
}

void CBaseObj::Free()
{
	__super::Free();

	Safe_Release(m_pAABBCom);
	Safe_Release(m_pOBBCom);
	Safe_Release(m_pSPHERECom);

	Safe_Release(m_pAnimTransformCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
}
