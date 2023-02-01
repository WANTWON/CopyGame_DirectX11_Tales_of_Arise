#include "..\Public\Transform.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

_float3 CTransform::Get_Scale(void)
{
	_float fRight, fUp, fLook;

	XMStoreFloat(&fRight, XMVector3Length(Get_State(CTransform::STATE_RIGHT)));
	XMStoreFloat(&fUp, XMVector3Length(Get_State(CTransform::STATE_UP)));
	XMStoreFloat(&fLook, XMVector3Length(Get_State(CTransform::STATE_LOOK)));

	return _float3(fRight, fUp, fLook);
}

void CTransform::Set_Scale(_float3 vScale)
{
	_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	WorldMatrix.r[CTransform::STATE_RIGHT] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_RIGHT]) * vScale.x;
	WorldMatrix.r[CTransform::STATE_UP] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_UP]) * vScale.y;
	WorldMatrix.r[CTransform::STATE_LOOK] = XMVector3Normalize(WorldMatrix.r[CTransform::STATE_LOOK]) * vScale.z;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

void CTransform::Set_Rotation(_float3 vRotation)
{
	_matrix RotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(vRotation.x), XMConvertToRadians(vRotation.y), XMConvertToRadians(vRotation.z));
	_float3 vScale = Get_Scale();

	_vector vRight = XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f), RotationMatrix) * vScale.x;
	_vector vUp = XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f), RotationMatrix) * vScale.y;
	_vector vLook = XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f), RotationMatrix) * vScale.z;

	_float3 fRight, fUp, fLook;
	XMStoreFloat3(&fRight, vRight);
	XMStoreFloat3(&fUp, vUp);
	XMStoreFloat3(&fLook, vLook);

	Set_State(CTransform::STATE_RIGHT, fRight);
	Set_State(CTransform::STATE_UP, fUp);
	Set_State(CTransform::STATE_LOOK, fLook);
}

HRESULT CTransform::Initialize_Prototype(void)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if (nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
	
	return S_OK;
}

void CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	_float3 fAfterPosition, fPrePosition;
	XMStoreFloat3(&fAfterPosition, vPosition);
	XMStoreFloat3(&fPrePosition, Get_State(CTransform::STATE_TRANSLATION));

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	else
	{
		_int iIsMove = pNavigation->isMove(&fAfterPosition, &fPrePosition);
		if (CNavigation::MOVE_IN == iIsMove)
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
		else if (CNavigation::MOVE_SLIDE == iIsMove && CNavigation::MOVE_IN == pNavigation->isMove(&fAfterPosition, &fPrePosition))
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	}
}

void CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	_float3 fAfterPosition, fPrePosition;
	XMStoreFloat3(&fAfterPosition, vPosition);
	XMStoreFloat3(&fPrePosition, Get_State(CTransform::STATE_TRANSLATION));

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	else
	{
		_int iIsMove = pNavigation->isMove(&fAfterPosition, &fPrePosition);
		if (CNavigation::MOVE_IN == iIsMove)
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
		else if (CNavigation::MOVE_SLIDE == iIsMove && CNavigation::MOVE_IN == pNavigation->isMove(&fAfterPosition, &fPrePosition))
				Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	}
}

void CTransform::Go_Right(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	_float3 fAfterPosition, fPrePosition;
	XMStoreFloat3(&fAfterPosition, vPosition);
	XMStoreFloat3(&fPrePosition, Get_State(CTransform::STATE_TRANSLATION));

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	else
	{
		_int iIsMove = pNavigation->isMove(&fAfterPosition, &fPrePosition);
		if (CNavigation::MOVE_IN == iIsMove)
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
		else if (CNavigation::MOVE_SLIDE == iIsMove && CNavigation::MOVE_IN == pNavigation->isMove(&fAfterPosition, &fPrePosition))
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	}
}

void CTransform::Go_Left(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	_float3 fAfterPosition, fPrePosition;
	XMStoreFloat3(&fAfterPosition, vPosition);
	XMStoreFloat3(&fPrePosition, Get_State(CTransform::STATE_TRANSLATION));

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	else
	{
		_int iIsMove = pNavigation->isMove(&fAfterPosition, &fPrePosition);
		if (CNavigation::MOVE_IN == iIsMove)
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
		else if (CNavigation::MOVE_SLIDE == iIsMove && CNavigation::MOVE_IN == pNavigation->isMove(&fAfterPosition, &fPrePosition))
			Set_State(CTransform::STATE_TRANSLATION, fAfterPosition);
	}
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	_vector vRight = XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix);
	_vector vUp = XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix);
	_vector vLook = XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix);

	_float3 fRight, fUp, fLook;
	XMStoreFloat3(&fRight, vRight);
	XMStoreFloat3(&fUp, vUp);
	XMStoreFloat3(&fLook, vLook);

	Set_State(CTransform::STATE_RIGHT, fRight);
	Set_State(CTransform::STATE_UP, fUp);
	Set_State(CTransform::STATE_LOOK, fLook);
}

void CTransform::LookAt(_fvector vPoint)
{
	_vector vLook = vPoint - Get_State(CTransform::STATE_TRANSLATION);

	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	_vector vUp = XMVector3Cross(vLook, vRight);

	_float3 vScale = Get_Scale();

	_float3 fRight, fUp, fLook;

	XMStoreFloat3(&fRight, XMVector3Normalize(vRight) * vScale.x);
	XMStoreFloat3(&fUp, XMVector3Normalize(vUp) * vScale.y);
	XMStoreFloat3(&fLook, XMVector3Normalize(vLook) * vScale.z);

	Set_State(CTransform::STATE_RIGHT, fRight);
	Set_State(CTransform::STATE_UP, fUp);
	Set_State(CTransform::STATE_LOOK, fLook);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTransform::Free(void)
{
	__super::Free();
}