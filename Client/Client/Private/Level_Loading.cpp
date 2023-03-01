#include "stdafx.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "Loader.h"
#include "Level_Logo.h"
#include "Level_BattleZone.h"
#include "Level_SnowField.h"
#include "Level_BossZone.h"
#include "Level_City.h"
#include "Level_Restaurant.h"
#include "Level_WorkTool.h"
#include "Level_LawBattle.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevel)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_eNextLevel = eNextLevel;

	CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Loading"), LEVEL_STATIC, TEXT("UI_LOADING"), nullptr);

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eNextLevel);
	if (!m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pLoader->Get_Finished() == true)
	{
		CLevel* pNewLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_BATTLE:
			pNewLevel = CLevel_BattleZone::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_SNOWFIELD:
			pNewLevel = CLevel_SnowField::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_BOSS:
			pNewLevel = CLevel_BossZone::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_CITY:
			pNewLevel = CLevel_City::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_RESTAURANT:
			pNewLevel = CLevel_Restaurant::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_WORKTOOL:
			pNewLevel = CLevel_WorkTool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LAWBATTLE:
			pNewLevel = CLevel_LawBattle::Create(m_pDevice, m_pContext);
			break;
		}

		if (!pNewLevel)
			return;

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}
}

void CLevel_Loading::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());
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