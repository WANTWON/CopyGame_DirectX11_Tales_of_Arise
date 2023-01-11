#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "CameraManager.h"
#include <time.h>

CMainApp::CMainApp()
	: m_pGameInstance(CGameInstance::Get_Instance())
	, m_pUI_Manager(CUI_Manager::Get_Instance())
	, m_pCollisionManager(CCollision_Manager::Get_Instance())
	, m_pDataManager(CData_Manager::Get_Instance())
{
	Safe_AddRef(m_pUI_Manager);
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pCollisionManager);
	Safe_AddRef(m_pDataManager);
}

HRESULT CMainApp::Initialize()
{
	srand(unsigned int(time(NULL)));

	GRAPHIC_DESC		Graphic_Desc;
	ZeroMemory(&Graphic_Desc, sizeof(GRAPHIC_DESC));

	Graphic_Desc.hWnd = g_hWnd;
	Graphic_Desc.iWinSizeX = g_iWinSizeX;
	Graphic_Desc.iWinSizeY = g_iWinSizeY;
	Graphic_Desc.eWinMode = GRAPHIC_DESC::MODE_WIN;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, LEVEL_END, Graphic_Desc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(CData_Manager::Get_Instance()->Init(m_pDevice, m_pContext)))
		return E_FAIL;	// 추가

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
	if (FAILED(m_pGameInstance->Add_Fonts(m_pDevice, m_pContext, TEXT("Font_Nexon"), TEXT("../../../Bin/Resources/Fonts/130.spritefont"))))
		return E_FAIL;

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;

	if (m_pGameInstance->Key_Up(DIK_I))
		m_pUI_Manager->Set_UI_OpenType(CUI_Manager::UI_INVEN);
	if (m_pGameInstance->Key_Up(DIK_MINUS))
		m_pUI_Manager->Set_UI_OpenType(CUI_Manager::UI_MAP);

	if (m_pUI_Manager->Get_UI_OpenType() == CUI_Manager::UI_INVEN && m_pGameInstance->Get_CurrentLevelIndex() != LEVEL_LOADING)
		m_pUI_Manager->Tick_Inventory();
	else if (m_pUI_Manager->Get_UI_OpenType() == CUI_Manager::UI_MAP && m_pGameInstance->Get_CurrentLevelIndex() != LEVEL_LOADING)
		m_pUI_Manager->Tick_Map();

	m_pGameInstance->Tick_Engine(fTimeDelta);

#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif // _DEBUG

}

HRESULT CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 0.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pRenderer->Render_GameObjects();

	
#ifdef _DEBUG
	++m_iNumRender;

	if (m_fTimeAcc > 1.0f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);		

		//OutputDebugString(m_szFPS);

		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	//m_pGameInstance->Render_Font(TEXT("Font_Nexon"), m_szFPS, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.6f);
#endif // _DEBUG

	m_pGameInstance->Present();
	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVEL eLevel)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	CLevel_Loading*			pLevel_Loading = CLevel_Loading::Create(m_pDevice, m_pContext, eLevel);
	if (nullptr == pLevel_Loading)
		return E_FAIL;

	m_pGameInstance->Open_Level(LEVEL_LOADING, pLevel_Loading);

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_UI.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;
	


	Safe_AddRef(m_pRenderer);

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pRenderer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();


	Safe_Release(m_pUI_Manager);
	Safe_Release(m_pCollisionManager);
	Safe_Release(m_pDataManager);

	CCollision_Manager::Get_Instance()->Destroy_Instance();
	CData_Manager::Get_Instance()->Destroy_Instance();
	CUI_Manager::Get_Instance()->Destroy_Instance();
	CCameraManager::Get_Instance()->Destroy_Instance();
}

