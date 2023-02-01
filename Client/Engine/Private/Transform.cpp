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

void CTransform::Set_Scale(STATE eState, _float fScale)
{
	_matrix			WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	WorldMatrix.r[eState] = XMVector3Normalize(WorldMatrix.r[eState]) * fScale;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void * pArg)
{
	if(nullptr != pArg)
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));

	return S_OK;
}

bool CTransform::Go_Straight(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_Backward(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_Left(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_Right(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_Up(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vUp = Get_State(CTransform::STATE_UP);

	vPosition += XMVector3Normalize(vUp) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vUp)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Sliding_Straight(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (pNavigation)
	{
		pNavigation->Compute_CurrentIndex_byXZ(vPosition);
	}

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (false == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
	{
		_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());
		_float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
		vNormal = vNormal*fDot*-1.f;
		_vector vSliding = XMVector3Normalize(vLook + vNormal);
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		if (true == pNavigation->isMove(vPos + XMVector3Normalize(vLook)*fRadius))
			Set_State(CTransform::STATE_TRANSLATION, vPos);
		return false;
	}

	return true;
}

bool CTransform::Sliding_Backward(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK)*-1.f;

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (pNavigation)
	{
		pNavigation->Compute_CurrentIndex_byXZ(vPosition);
	}

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (false == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
	{
		_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());
		_float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
		vNormal = vNormal*fDot*-1.f;
		_vector vSliding = XMVector3Normalize(vLook + vNormal);
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		if (true == pNavigation->isMove(vPos + XMVector3Normalize(vLook)*fRadius))
			Set_State(CTransform::STATE_TRANSLATION, vPos);
		return false;
	}

	return true;
}

bool CTransform::Sliding_Left(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT)*-1.f;

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (pNavigation)
	{
		pNavigation->Compute_CurrentIndex_byXZ(vPosition);
	}

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (false == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
	{
		_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());
		_float fDot = XMVectorGetX(XMVector3Dot(vRight, vNormal));
		vNormal = vNormal*fDot*-1.f;
		_vector vSliding = XMVector3Normalize(vRight + vNormal);
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		if (true == pNavigation->isMove(vPos + XMVector3Normalize(vRight)*fRadius))
			Set_State(CTransform::STATE_TRANSLATION, vPos);
		return false;
	}

	return true;
}

bool CTransform::Sliding_Right(_float fTimeDelta, CNavigation * pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (pNavigation)
	{
		pNavigation->Compute_CurrentIndex_byXZ(vPosition);
	}

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (false == pNavigation->isMove(vPosition + XMVector3Normalize(vRight)*fRadius))
	{
		_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());
		_float fDot = XMVectorGetX(XMVector3Dot(vRight, vNormal));
		vNormal = vNormal*fDot*-1.f;
		_vector vSliding = XMVector3Normalize(vRight + vNormal);
		_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
		vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		if (true == pNavigation->isMove(vPos + XMVector3Normalize(vRight)*fRadius))
			Set_State(CTransform::STATE_TRANSLATION, vPos);
		return false;
	}

	return true;
}

bool CTransform::Sliding_Anim(_vector vecMove, _float fRotation, class CNavigation* pNavigation)
{
	_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

	_vector vWorldPos = WorldMatrix.r[3];
	WorldMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	_vector vWorldRot = XMQuaternionNormalize(XMQuaternionRotationMatrix(WorldMatrix));

	_vector RotationQuat = XMQuaternionRotationAxis(WorldMatrix.r[1], (fRotation * -1.f));

	WorldMatrix = XMMatrixRotationQuaternion(XMQuaternionNormalize(XMQuaternionMultiply(vWorldRot, RotationQuat)));

	_vector vTranslation = XMVector3TransformCoord(vecMove, XMMatrixRotationY(XMConvertToRadians(180.f)) * WorldMatrix);

	WorldMatrix.r[0] = XMVector4Normalize(WorldMatrix.r[0]) * Get_Scale(CTransform::STATE_RIGHT);
	WorldMatrix.r[1] = XMVector4Normalize(WorldMatrix.r[1]) * Get_Scale(CTransform::STATE_UP);
	WorldMatrix.r[2] = XMVector4Normalize(WorldMatrix.r[2]) * Get_Scale(CTransform::STATE_LOOK);
	WorldMatrix.r[3] = vWorldPos;

	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);

	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vAfterPosition = vPosition + vTranslation;
	vAfterPosition = XMVectorSetW(vAfterPosition, 1.f);

	if (pNavigation)
		pNavigation->Compute_CurrentIndex_byXZ(vAfterPosition);

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vAfterPosition);
	else if (true == pNavigation->isMove(vAfterPosition))
		Set_State(CTransform::STATE_TRANSLATION, vAfterPosition);
	/*else if (false == pNavigation->isMove(vAfterPosition))
	{
	_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());
	_float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
	vNormal = vNormal * fDot * -1.f;
	_vector vSliding = XMVector3Normalize(vLook + vNormal);
	vPosition += vSliding * XMVectorGetX(XMVector4Length(vAfterPosition - vPosition));
	if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook) * fRadius))
	Set_State(CTransform::STATE_TRANSLATION, vPosition);
	return false;
	}*/

	return true;
}

void CTransform::Jump(_float fTimeDelta, _float fVelocity, _float fGravity, _float fStartiHeight, _float fEndiHeight)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);
	float fPosY = fStartiHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);

	vPosition = XMVectorSetY(vPosition, fPosY);
	float y = XMVectorGetY(vPosition);
	Set_State(CTransform::STATE_TRANSLATION, vPosition);
}

bool CTransform::Go_PosTarget(_float fTimeDelta, _vector TargetPos, CNavigation * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPos = TargetPos;
	_vector vDir = vTargetPos - vPosition;
	_float fLength = XMVectorGetX(XMVector3Length(vDir));
	vDir = XMVector3Normalize(vDir);

	if (fLength < 0.5f)
	{
		Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
		return true;
	}
	
	vPosition += vDir*fTimeDelta*m_TransformDesc.fSpeedPerSec;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_PosDir(_float fTimeDelta, _vector vecDir, CNavigation * pNavigation)
{
	_vector vPosition = Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = vecDir;

	vDir = XMVector3Normalize(vDir);

	vPosition += vDir*fTimeDelta*m_TransformDesc.fSpeedPerSec;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else if (true == pNavigation->isMove(vPosition))
		Set_State(CTransform::STATE_TRANSLATION, vPosition);
	else
		return false;

	return true;
}

bool CTransform::Go_PosLerp(_float fTimeDelta, _vector TargetPos, _float fVecOffset)
{
	_vector vPos = Get_State(CTransform::STATE_TRANSLATION);
	_vector vDir = TargetPos - vPos;

	_float fLength = XMVectorGetX(XMVector3Length(vPos - TargetPos));
	_float  fVec = (1 - fTimeDelta) * 0 + fTimeDelta*fLength;

	vPos += XMVector3Normalize(vDir)*fVec*m_TransformDesc.fSpeedPerSec*fVecOffset;

	Set_State(CTransform::STATE_TRANSLATION, vPos);

	if (fVec == 0.0f || fLength < 0.1f)
		return true;

	return false;
}


void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_TransformDesc.fRotationPerSec * fTimeDelta);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(Get_State(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(Get_State(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(Get_State(CTransform::STATE_LOOK), RotationMatrix));
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_RIGHT), RotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(CTransform::STATE_UP), RotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(CTransform::STATE_LOOK), RotationMatrix));
}

/* This is NOT an additive rotation like the "Turn()" function above.
This function set the specific rotation on the specific axis. */
void CTransform::Set_Rotation(_float3 fAngle)
{
	// Get current RotationMatrix from the WorldMatrix by decomposition.
	_vector vScale, vRotationQuat, vTranslation;
	XMMatrixDecompose(&vScale, &vRotationQuat, &vTranslation, Get_WorldMatrix());
	_matrix RotationMatrix = XMMatrixRotationQuaternion(vRotationQuat);

	// Multiply the WorldMatrix by the Inverse of current RotationMatrix (to get a WorldMatrix without any Rotation)
	_matrix InverseRotationMatrix = XMMatrixInverse(nullptr, RotationMatrix);
	_matrix WorldMatrixWithoutRotation = XMMatrixMultiply(Get_WorldMatrix(), InverseRotationMatrix);

	// Make a NewRotationMatrix with new angle values
	_matrix NewRotationMatrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(fAngle.x), XMConvertToRadians(fAngle.y), XMConvertToRadians(fAngle.z));
	m_fCurrentRotationX = fAngle.x;
	m_fCurrentRotationY = fAngle.y;
	m_fCurrentRotationZ = fAngle.z;

	// Set NewRotationMatrix to WorldMatrixWithoutRotation
	Set_State(CTransform::STATE_RIGHT, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[0], NewRotationMatrix));
	Set_State(CTransform::STATE_UP, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[1], NewRotationMatrix));
	Set_State(CTransform::STATE_LOOK, XMVector3TransformNormal(WorldMatrixWithoutRotation.r[2], NewRotationMatrix));
}

void CTransform::Set_RotationY(_float fAngleY)
{
	Set_Rotation(_float3(m_fCurrentRotationX, fAngleY, m_fCurrentRotationZ));
}

void CTransform::LookAt(_fvector vAt)
{
	_vector		vPosition = Get_State(CTransform::STATE_TRANSLATION);

	_vector		vLook = vAt - vPosition;
	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(STATE_UP, XMVector3Normalize(vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * Get_Scale(CTransform::STATE_LOOK));
}

void CTransform::RoamingTurn(_matrix WorldMatrix, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

	XMStoreFloat4x4(&m_WorldMatrix, XMLoadFloat4x4(&m_WorldMatrix) * RotationMatrix);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*	pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


CComponent * CTransform::Clone(void * pArg)
{
	CTransform*	pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CTransform"));
		Safe_Release(pInstance);
	}
		
	return pInstance;
}

void CTransform::Free()
{
	__super::Free();

}
