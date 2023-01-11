#include "..\Public\Transform.h"
#include "..\Public\Navigation.h"
#include "Cell.h"
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
	if (pArg != nullptr)
	{
		memcpy(&m_TransformDesc, pArg, sizeof(TRANSFORMDESC));
	}

	return S_OK;
}

bool CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation, _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_POSITION, vPosition);

	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
		Set_State(CTransform::STATE_POSITION, vPosition);
	else
		return false;
		
	return true;
}

bool CTransform::Go_StraightSliding(_float fTimeDelta, CNavigation * pNavigation,  _float fRadius)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (pNavigation)
	{
		pNavigation->Compute_CurrentIndex_byDistance(vPosition);
	}

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_POSITION, vPosition);
	else if (true == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
	{
		if (pNavigation->Get_CurrentCelltype() == CCell::DROP)
		{
			_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());

			_float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
			vNormal = vNormal*fDot*-1.f;
			_vector vSliding = XMVector3Normalize(vLook + vNormal);
			_vector vPos = Get_State(CTransform::STATE_POSITION);
			vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
			if (true == pNavigation->isMove(vPos + XMVector3Normalize(vLook)*fRadius) && pNavigation->Get_CurrentCelltype() != CCell::DROP)
				Set_State(CTransform::STATE_POSITION, vPos);
			return false;
		}
		else
			Set_State(CTransform::STATE_POSITION, vPosition);
	}
		
	else if (false == pNavigation->isMove(vPosition + XMVector3Normalize(vLook)*fRadius))
	{
		_vector vNormal = XMVector3Normalize(pNavigation->Get_LastNormal());

		_float fDot = XMVectorGetX(XMVector3Dot(vLook, vNormal));
		vNormal = vNormal*fDot*-1.f;
		_vector vSliding = XMVector3Normalize(vLook + vNormal);
		_vector vPos = Get_State(CTransform::STATE_POSITION);
		vPos += vSliding*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		if (true == pNavigation->isMove(vPos + XMVector3Normalize(vLook)*fRadius))
			Set_State(CTransform::STATE_POSITION, vPos);
		return false;
	}
		

	return true;
}

void CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = Get_State(CTransform::STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	if (nullptr == pNavigation)
		Set_State(CTransform::STATE_POSITION, vPosition);

	else if (true == pNavigation->isMove(vPosition))
		Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vRight = Get_State(CTransform::STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_TransformDesc.fSpeedPerSec * fTimeDelta;

	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Jump(_float fTimeDelta, _float fVelocity, _float fGravity, _float fStartiHeight, _float fEndiHeight)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	float fPosY = fStartiHeight + fVelocity * fTimeDelta - (0.5f*fGravity*fTimeDelta*fTimeDelta);

	vPosition = XMVectorSetY(vPosition, fPosY);

	float y = XMVectorGetY(vPosition);
	if (y < fEndiHeight)
		vPosition = XMVectorSetY(vPosition, fEndiHeight);
	Set_State(CTransform::STATE_POSITION, vPosition);
}

void CTransform::Go_PosTarget(_float fTimeDelta, _vector TargetPos, _vector distance)
{
	_vector vPos = Get_State(CTransform::STATE_POSITION);
	_vector vNewPos = TargetPos + distance;
	_vector vDir = vNewPos - vPos;
	_float fLength =  XMVectorGetX(XMVector3Length(vDir));
	vDir = XMVector3Normalize(vDir);

	if (fLength < 0.5f)
	{
		Set_State(CTransform::STATE_POSITION, vNewPos);
	}
	else
	{
		vPos += vDir*fTimeDelta*m_TransformDesc.fSpeedPerSec;
		Set_State(CTransform::STATE_POSITION, vPos);
	}
	
}

_bool CTransform::Go_PosLerp(_float fTimeDelta, _vector TargetPos, _float fVecOffset, _vector distance)
{
	_vector vPos = Get_State(CTransform::STATE_POSITION);
	_vector vOffsetPos = TargetPos + distance;
	_vector vDir = vOffsetPos - vPos;

	_vector m_TargetPos = TargetPos;

	_float fLength = XMVectorGetX(XMVector3Length(vPos - vOffsetPos));
	_float  fVec = (1 - fTimeDelta) * 0 + fTimeDelta*fLength;

	

	vPos += XMVector3Normalize(vDir)*fVec*m_TransformDesc.fSpeedPerSec*fVecOffset;
	Set_State(CTransform::STATE_POSITION, vPos);

	if (fVec == 0.0f || fLength < 0.1f)
		return true;

	return false;
	
}

bool CTransform::Go_PosDir(_float fTimeDelta, _vector vecDir, CNavigation* pNavigation)
{
	_vector vPos = Get_State(CTransform::STATE_POSITION);
	_vector vDir = vecDir;

	vDir = XMVector3Normalize(vDir);

	vPos += vDir*fTimeDelta*m_TransformDesc.fSpeedPerSec;

	if (nullptr == pNavigation)
	{
		Set_State(CTransform::STATE_POSITION, vPos);
		return true;
	}
	else if (true == pNavigation->isMove(vPos))
	{
		Set_State(CTransform::STATE_POSITION, vPos);
		return true;
	}
	else
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

void CTransform::Follow_Target(_float fTimeDelta, _vector TargetPos, _vector distance)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector     vNewPos = TargetPos + distance;


	Set_State(CTransform::STATE_POSITION, vNewPos);

}

void CTransform::LookAt(_fvector vAt)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = vAt - vPosition;
	_vector		vMyOriginalLook = Get_State(CTransform::STATE_LOOK);

	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(STATE_UP, XMVector3Normalize(vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * Get_Scale(CTransform::STATE_LOOK));

}

void CTransform::LookDir(_fvector vDir)
{
	_vector		vPosition = Get_State(CTransform::STATE_POSITION);

	_vector		vLook = vDir;
	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(STATE_UP, XMVector3Normalize(vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * Get_Scale(CTransform::STATE_LOOK));
}

void CTransform::Change_Direction(_float UpDown, _float RightLeft)
{
	if (UpDown == 0 && RightLeft == 0)
		return;

	_vector		vPosition = Get_State(CTransform::STATE_POSITION);
	_vector		vLook = vPosition + XMVectorSet(RightLeft, 0.f, UpDown, 0.f) - vPosition;

	_vector		vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_vector		vRight = XMVector3Cross(vAxisY, vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(STATE_UP, XMVector3Normalize(vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * Get_Scale(CTransform::STATE_LOOK));

}

void CTransform::Fix_Look_byFloor(_fvector vAxis)
{
	_vector		vUp = vAxis;
	_vector		vLook = Get_State(CTransform::STATE_LOOK);
	_vector		vRight = XMVector3Cross(vUp, vLook);

	_vector		vNewLook = XMVector3Cross(vRight, vUp);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * Get_Scale(CTransform::STATE_RIGHT));
	Set_State(STATE_UP, XMVector3Normalize(vUp) * Get_Scale(CTransform::STATE_UP));
	Set_State(STATE_LOOK, XMVector3Normalize(vNewLook) * Get_Scale(CTransform::STATE_LOOK));
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
