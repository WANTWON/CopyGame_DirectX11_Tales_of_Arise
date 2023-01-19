#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize(void)
{
	GRAPHIC_DESC Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, Graphic_Desc, &m_pDevice, &m_pContext)))
		return E_FAIL;
	
	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;
	
	if (FAILED(Open_Level(LEVEL_GAMEPLAY)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;
	
	m_pGameInstance->Tick_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif // _DEBUG
}

HRESULT CMainApp::Render(void)
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;
	
	CLevel_Loading* pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevel);
	if (nullptr == pLevel_Loading)
		return E_FAIL;
	
	m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component(void)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create(void)
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free(void)
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);

	CGameInstance::Release_Engine();
}
