#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "Object_Pool_Manager.h"
#include "Npc.h"
#include "SnowFieldNpc.h"
#include "Portal.h"
#include "Item.h"
#include "TreasureBox.h"
#include "PlayerCreater.h"

float g_fSoundVolume = 0.0f;
float g_fSoundNatureVolume = 0.0f;
bool g_bIsSoundOn;
bool g_bEnd = false;

CLevel_Logo::CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	cout << " NonAnim Clone start" << endl;
	m_pLoader = CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_NONANIM);
	if (nullptr == m_pLoader)
		return E_FAIL;

	cout << " Player Clone start" << endl;
	m_pPlayerLoader =  CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_PLAYER);
	if (nullptr == m_pPlayerLoader)
		return E_FAIL;

	cout << " Monster Group1 Clone start" << endl;
	m_pMonsterLoader = CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_MONSTER);
	if (nullptr == m_pMonsterLoader)
		return E_FAIL;

	DWORD fTimeDelta = GetTickCount();

	while (m_pMonsterLoader->Get_Finished() == false ||
		m_pPlayerLoader->Get_Finished() == false ||
		m_pLoader->Get_Finished() == false )
	{
		if (fTimeDelta + 1000 < GetTickCount())
		{

			if (m_pPlayerLoader->Get_Finished() == false)
				cout << "Player cloning" << endl;
			if (m_pMonsterLoader->Get_Finished() == false)
				cout << "Monster cloning" << endl;
			if (m_pLoader->Get_Finished() == false)
				cout << "NonAnim cloning" << endl;

			fTimeDelta = GetTickCount();
		}

	}


	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_LOGO.wav"), g_fSoundVolume);
	
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	g_fSoundVolume += 0.01f;
	if (g_fSoundVolume >= 0.2f)
		g_fSoundVolume = 0.2f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);


	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		CGameInstance::Get_Instance()->PlaySounds(TEXT("6_UI_Sys_Do_Start.wav"), SOUND_SYSTEM, 0.4f);

		
		pGameInstance->Set_DestinationLevel(LEVEL_SNOWFIELD);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SNOWFIELD))))
			return;

		Safe_Release(pGameInstance);
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Screen"), LEVEL_LOGO, pLayerTag, nullptr)))
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
	
	Safe_Release(m_pMonsterLoader);
	Safe_Release(m_pPlayerLoader);
	Safe_Release(m_pLoader);
}
