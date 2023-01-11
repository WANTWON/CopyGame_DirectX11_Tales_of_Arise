#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "UIScreen.h"
#include "Level_TailCave.h"
#include "Level_Room.h"
#include "Level_Tower.h"
#include "Level_End.h"

bool g_FirstLoading = false;

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}



HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (g_FirstLoading)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_UI"))))
			return E_FAIL;
	}


	g_FirstLoading = true;
	m_eNextLevel = eNextLevel;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;

	

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	g_fBGMVolume -= 0.001f;
	if (g_fBGMVolume <= 0.01f)
		g_fBGMVolume = 0.01f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fBGMVolume);

	if (true == m_pLoader->Get_Finished())
	{
			/* 넥스트레벨에 대한 준비가 끝나면 실제 넥스트레벨을 할당한다. */
			CLevel*			pNewLevel = nullptr;

			switch (m_eNextLevel)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_TAILCAVE:
				pNewLevel = CLevel_TailCave::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_ROOM:
				pNewLevel = CLevel_Room::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_TOWER:
				pNewLevel = CLevel_Tower::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_END:
				pNewLevel = CLevel_End::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return;

			CGameInstance* pGameInstance = CGameInstance::Get_Instance();
			if (nullptr == pGameInstance)
				return;
			Safe_AddRef(pGameInstance);

			if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
				return;

			Safe_Release(pGameInstance);		
	}
}

void CLevel_Loading::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
}

HRESULT CLevel_Loading::ForLoadingLevelTexture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);
	g_FirstLoading = true;

	return S_OK;
}

HRESULT CLevel_Loading::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	CUIScreen::BACKGROUNDESC BackgroundDesc;

	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_LOADING;
	BackgroundDesc.pTextureTag = TEXT("Prototype_Component_Texture_LoadingScreen_UI");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_LOADING, pLayerTag,
		&BackgroundDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Loading"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
