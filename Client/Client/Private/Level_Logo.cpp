#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UIScreen.h"


float g_fBGMVolume = 0.f;
float g_fObjVolume = 0.2f;
float g_fUIVolume = 0.4f;

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	CGameInstance::Get_Instance()->PlayBGM(TEXT("0_Title (No Intro Version).mp3"), g_fBGMVolume);
	CUI_Manager::Get_Instance()->Set_NextLevel(false);
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	if (CUI_Manager::Get_Instance()->Get_NextLevelFinished())
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);
		pGameInstance->Set_DestinationLevel(LEVEL_GAMEPLAY);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
		Safe_Release(pGameInstance);
	}

	g_fBGMVolume += 0.001f;
	if (g_fBGMVolume >= 0.2f)
		g_fBGMVolume = 0.2f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fBGMVolume);


	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Sys_Do_Start.wav"), SOUND_SYSTEM, 0.4f);
		
		CUI_Manager::Get_Instance()->Set_NextLevel(true);
		CUI_Manager::Get_Instance()->Set_NextLevelFinished(false);
	}
		
}

void CLevel_Logo::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("로고레벨입니다."));
}

HRESULT CLevel_Logo::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUIScreen::BACKGROUNDESC BackgroundDesc;

	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_LOGO;
	BackgroundDesc.pTextureTag = TEXT("Prototype_Component_Texture_TitleScreen");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_LOGO, pLayerTag,
		&BackgroundDesc)))
		return E_FAIL;


	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_SCREEN;
	BackgroundDesc.pTextureTag = nullptr;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_STATIC, TEXT("UI_Screen"),
		&BackgroundDesc)))
		return E_FAIL;

	
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Logo"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	__super::Free();


}
