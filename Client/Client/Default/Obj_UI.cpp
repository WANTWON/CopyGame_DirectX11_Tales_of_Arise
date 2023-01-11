#include "stdafx.h"
#include "Obj_UI.h"



CObj_UI::CObj_UI(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext)
{
}

CObj_UI::CObj_UI(const CObj_UI & rhs)
	: CGameObject(rhs)
{
}

HRESULT CObj_UI::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixTranspose(XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f)));

	return S_OK;
}

HRESULT CObj_UI::SetUp_ShaderResources()
{
	return S_OK;
}

void CObj_UI::Free()
{
}
