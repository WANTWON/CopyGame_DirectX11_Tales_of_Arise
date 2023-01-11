#include "stdafx.h"
#include "..\Public\Level_End.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "UIScreen.h"


CLevel_End::CLevel_End(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_End::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("03 NameInput.mp3"), g_fBGMVolume);
	CUI_Manager::Get_Instance()->Set_NextLevel(false);
	return S_OK;
}

void CLevel_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	g_fBGMVolume += 0.001f;
	if (g_fBGMVolume >= 0.2f)
		g_fBGMVolume = 0.2f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fBGMVolume);

}

void CLevel_End::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("엔딩레벨입니다."));
}

HRESULT CLevel_End::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	pGameInstance->Clear_Layer(LEVEL_STATIC, TEXT("Layer_UI"));
	pGameInstance->Clear_Layer(LEVEL_STATIC, TEXT("Layer_InvenItem"));
	
	CUIScreen::BACKGROUNDESC BackgroundDesc;

	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_LOGO;
	BackgroundDesc.pTextureTag = TEXT("Prototype_Component_Texture_EndingScreen");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_LOGO, pLayerTag,
		&BackgroundDesc)))
		return E_FAIL;
	
	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_End * CLevel_End::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_End*	pInstance = new CLevel_End(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_End"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_End::Free()
{
	__super::Free();


}
