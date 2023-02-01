#include "..\Public\Picking.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}

HRESULT CPicking::Initialize(HWND hWnd, ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	m_hWnd = hWnd;
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	return S_OK;
}

void CPicking::Tick(void)
{
	POINT ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);
	
	_uint iNumViewPorts = 1;

	D3D11_VIEWPORT Viewport;
	
	m_pContext->RSGetViewports(&iNumViewPorts, &Viewport);

	_vector vMousePoint = XMVectorSet(ptMouse.x / (Viewport.Width * 0.5f) - 1.f, ptMouse.y / -(Viewport.Height * 0.5f) + 1.f, 0.f, 1.f);

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_matrix ProjMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	
	ProjMatrix = XMMatrixInverse(nullptr, ProjMatrix);
	vMousePoint = XMVector3TransformCoord(vMousePoint, ProjMatrix);

	_vector vRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vRayDir = vMousePoint - vRayPos;
	
	_matrix ViewMatrix = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	RELEASE_INSTANCE(CGameInstance);

	ViewMatrix = XMMatrixInverse(nullptr, ViewMatrix);
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrix);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrix);
	vRayDir = XMVector3Normalize(vRayDir);

	XMStoreFloat3(&m_vRayPos, vRayPos);
	XMStoreFloat3(&m_vRayDir, vRayDir);
}

void CPicking::Transform_ToLocalSpace(_matrix WorldMatrixInverse)
{
	_vector vRayPos_Local = XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrixInverse);
	_vector vRayDir_Local = XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrixInverse);
	vRayDir_Local = XMVector3Normalize(vRayDir_Local);

	XMStoreFloat3(&m_vRayPos_Local, vRayPos_Local);
	XMStoreFloat3(&m_vRayDir_Local, vRayDir_Local);
}

_bool CPicking::Intersect_InWorldSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3 * pOut)
{
	_float fDist;

	//Intersects(_In_ FXMVECTOR Origin, _In_ FXMVECTOR Direction, _In_ FXMVECTOR V0, _In_ GXMVECTOR V1, _In_ HXMVECTOR V2, _Out_ float& Dist)
	if (true == TriangleTests::Intersects(XMLoadFloat3(&m_vRayPos), XMLoadFloat3(&m_vRayDir), XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist))
	{
		XMStoreFloat3(pOut, XMLoadFloat3(&m_vRayPos) + XMLoadFloat3(&m_vRayDir) * fDist);

		return true;
	}

	return false;
}

_bool CPicking::Intersect_InLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3 * pOut)
{
	_float fDist;

	_vector vDir = XMLoadFloat3(&m_vRayDir_Local);

	if (true == TriangleTests::Intersects(XMLoadFloat3(&m_vRayPos_Local), vDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist))
	{
		XMStoreFloat3(pOut, XMLoadFloat3(&m_vRayPos_Local) + XMLoadFloat3(&m_vRayDir_Local) * fDist);

		return true;
	}

	return false;
}

void CPicking::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
