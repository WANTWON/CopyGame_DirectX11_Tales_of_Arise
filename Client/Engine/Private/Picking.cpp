#include "..\Public\Picking.h"
#include "Transform.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPicking)

CPicking::CPicking()
{
}


HRESULT CPicking::Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice); Safe_AddRef(m_pContext);

	m_hWnd = hWnd;

	m_iWinCX = iWinCX;

	m_iWinCY = iWinCY;

	return S_OK;
}

void CPicking::Tick()
{
	POINT			ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	_float3			vMousePos;

	/* 투영공간상의 마우스 좌표를 구한다. */
	vMousePos.x = _float(ptMouse.x / (m_iWinCX * 0.5f) - 1);
	vMousePos.y = _float(ptMouse.y / (m_iWinCY * -0.5f) + 1);
	vMousePos.z = 0.f;

	_vector vecMousePos = XMLoadFloat3(&vMousePos);
	vecMousePos = XMVectorSetW(vecMousePos, 1.f);

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	/* 뷰스페이스 상의 좌표를 구한다. */
	_matrix		ProjMatrixInv;
	ProjMatrixInv = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	ProjMatrixInv = XMMatrixInverse(nullptr, ProjMatrixInv);
	vecMousePos = XMVector3TransformCoord(vecMousePos, ProjMatrixInv);

	/* 뷰스페이스 상의 마우스 레이와 레이포스를 구하자. */
	_vector vecRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vecRayDir = vecMousePos - vecRayPos;
	

	/* 월드스페이스 상의 마우스 레이와 레이포스를 구하자. */
	_matrix		ViewMatrixInv;
	ViewMatrixInv = pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	ViewMatrixInv = XMMatrixInverse(nullptr, ViewMatrixInv);
	vecRayPos = XMVector3TransformCoord(vecRayPos, ViewMatrixInv);
	vecRayDir = XMVector3TransformNormal(vecRayDir, ViewMatrixInv);
	vecRayDir = XMVector3Normalize(vecRayDir);

	XMStoreFloat3(&m_vRayPos, vecRayPos);
	XMStoreFloat3(&m_vRayDir, vecRayDir);

	Safe_Release(pGameInstance);
}

void CPicking::Transform_ToLocalSpace(class CTransform * pTransform)
{
	_matrix WorldMatrixInverse = pTransform->Get_WorldMatrixInverse();
	
	_vector vRayPos = XMLoadFloat3(&m_vRayPos);
	vRayPos = XMVectorSetW(vRayPos, 1.f);
	_vector vRayDir = XMLoadFloat3(&m_vRayDir);

	XMVector3TransformCoord(vRayPos, WorldMatrixInverse);
	XMVector3TransformNormal(vRayDir, WorldMatrixInverse);
	XMVector3Normalize(vRayDir);

	XMStoreFloat3(&m_vRayPos_Local, vRayPos);
	XMStoreFloat3(&m_vRayDir_Local, vRayDir);

}


void CPicking::Compute_LocalRayInfo(_float3 * pRayDir, _float3 * pRayPos, CTransform * pTransform)
{
	_matrix		WorldMatrixInv = pTransform->Get_WorldMatrixInverse();

	XMStoreFloat3(pRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrixInv));
	XMStoreFloat3(pRayDir, XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrixInv));
}

void CPicking::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
