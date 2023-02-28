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

	CGameInstance::Get_Instance()->PlayBGM(TEXT("LogoSong.wav"), g_fSoundVolume);
	
	return S_OK;
}

void CLevel_Logo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	
	g_fSoundVolume += 0.001f;
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

HRESULT CLevel_Logo::Ready_InstancingForPooling(const _tchar* pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	NONANIMDESC stModelDesc;

#pragma region BattleZone
	strcpy(stModelDesc.pModeltag, "Conifer3_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Dead_Grass_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Dead_Tree1_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bush_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Broken_Tree_BattleZone");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Instancing"));

#pragma endregion BattleZone

#pragma region SnowField
	strcpy(stModelDesc.pModeltag, "Birch1_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Birch2_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bush_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Broken_Tree_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "BushWood_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Conifer3_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Dead_Grass_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Dead_Tree_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Fence_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Lamppillar_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Snow1_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Snow2_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "SmallRock2_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Tree5_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Stalagmite5_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Stalagmite4_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Tree_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

#pragma endregion SnowField

#pragma region BossZone
	strcpy(stModelDesc.pModeltag, "Prop_Light02_Lod1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BOSS, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bld_Wall01");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BOSS, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bld_WallB");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BOSS, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bld_Pillar01_Lod1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BOSS, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Bld_Wall01_B02_Lod1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BOSS, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));

#pragma endregion BossZone

#pragma region CityZone
	strcpy(stModelDesc.pModeltag, "CIty_BigWall");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "CIty_Bench");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "CIty_Chair");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "City_Grass0");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "CIty_Grass10");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "City_Ivy1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));


	strcpy(stModelDesc.pModeltag, "City_LightPillar");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "City_Palm");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "City_Planter");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "SAT_Wall02");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "City_Table");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));



#pragma endregion CityZone

#pragma region RestuarantZone
	strcpy(stModelDesc.pModeltag, "houseFloor_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "InteriorWall_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Plate_mdl_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Potted_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "WallDeco_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Vase_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Table_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Chair_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Window_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));



	strcpy(stModelDesc.pModeltag, "Apple_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Banana_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Carrot_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Mango_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	
	strcpy(stModelDesc.pModeltag, "PineApple_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Potato_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "RedOnion_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Broccoli_Restaurant");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_RESTAURANT, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));

#pragma endregion RestuarantZone

#pragma region WorkToolZone
	strcpy(stModelDesc.pModeltag, "BenchSmall_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Floor_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Furnace_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Pillow_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Pillow2_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Railing_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Rubble_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "ShopCounter_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "SmallRock2_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "Wall_WorkTool");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_WORKTOOL, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));


#pragma endregion WorkToolZone


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Npc(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNpc::NPCDESC NpcDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, NpcDesc.Modeldesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("NpcFemaleYoung")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::FEMALE_YOUNG;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_GLD")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_GLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMO_DOK")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_OLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMY_PLC")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_PLC;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));

		}
		else if (!wcscmp(pModeltag, TEXT("Rinwell")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::RINWELL_NPC;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, TEXT("LayerNpcRinwell"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("LayerNpcRinwell"));

		}

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_DecoObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/CliffRock.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);



	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Snow.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Torch.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_SnowDecoObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/CliffRock2.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Rock.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("Rock1")))
			strcpy(ModelDesc.pModeltag, "MapRock1");
		else if (!wcscmp(pModeltag, TEXT("Rock2")))
			strcpy(ModelDesc.pModeltag, "MapRock2");

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Torch.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/BagDeco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_RestaurantObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_RESTAURANT, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);


	hFile = 0;
	dwByte = 0;
	iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Ceil.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_RESTAURANT, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);


	CPortal::PORTALDESC PortalDesc;
	/* 타일의 개수 받아오기 */
	PortalDesc.m_ModelDesc.vPosition.x = 16.7f;
	PortalDesc.m_ModelDesc.vPosition.y = 0.f;
	PortalDesc.m_ModelDesc.vPosition.z = -4.f;
	PortalDesc.iNextLevel = LEVEL_CITY;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_RESTAURANT, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Portal"));

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BossMapObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/Floor.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_BOSS, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	ModelDesc;
	strcpy(ModelDesc.pModeltag, "Astral_Doubt");
	ModelDesc.vPosition = _float3(50, 0.f, 50.f);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AstralDoubt"), LEVEL_STATIC, TEXT("Layer_Boss"), &ModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Boss"));
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_CityMapObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/CIffWall.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/House01_2F.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);


	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Ground.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/MarketDeco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_CITY, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));

	}
	CloseHandle(hFile);


	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/PortalPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	CPortal::PORTALDESC PortalDesc;
	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	
	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_RESTAURANT;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));


	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_WORKTOOL;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));

	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_WorkToolMapObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_WORKTOOL, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Road.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_WORKTOOL, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Deco"));


	}

	CloseHandle(hFile);

	CPortal::PORTALDESC PortalDesc;
	/* 타일의 개수 받아오기 */
	PortalDesc.m_ModelDesc.vPosition.x = 15.f;
	PortalDesc.m_ModelDesc.vPosition.y = 0.f;
	PortalDesc.m_ModelDesc.vPosition.z = -4.f;
	PortalDesc.iNextLevel = LEVEL_CITY;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_WORKTOOL, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Portal"));


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Interact_SnowField(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CItem::ITEMDESC ItemDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Interact.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ItemDesc.m_ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Apple")))
		{
			ItemDesc.etype = CItem::APPLE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Mushroom")))
		{
			ItemDesc.etype = CItem::MUSHROOM;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Lettuce_002")))
		{
			ItemDesc.etype = CItem::LETTUCE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Jewel")))
		{
			ItemDesc.etype = CItem::JEWEL;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("GroundPlant")))
		{
			ItemDesc.etype = CItem::PLANT;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Box")))
		{
			ItemDesc.etype = CItem::BOX;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Box3")))
		{
			ItemDesc.etype = CItem::BOX;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Crystal")))
		{
			ItemDesc.etype = CItem::CRYSTAL;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_SNOWFIELD, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));

	}
	CloseHandle(hFile);


	CTreasureBox::BOXDESC m_TreasureBoxDesc;
	hFile = 0;
	dwByte = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/treasurebox.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(m_TreasureBoxDesc), sizeof(CTreasureBox::BOXDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBox"), LEVEL_SNOWFIELD, pLayerTag, &m_TreasureBoxDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));

	}
	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);


	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Interact_Restaurant(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CItem::ITEMDESC ItemDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Interact.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ItemDesc.m_ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Apple")))
		{
			ItemDesc.etype = CItem::APPLE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else
		{
			ItemDesc.etype = CItem::APPLE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Interact"));

	}
	CloseHandle(hFile);

	//hFile = 0;
	//dwByte = 0;
	//iNum = 0;

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Apple.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	//	
	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;
	//	
	//}
	//CloseHandle(hFile);

	//hFile = 0;
	//dwByte = 0;
	//iNum = 0;

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Banana.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	//hFile = 0;
	//dwByte = 0;
	//iNum = 0;

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Bread_French.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Carrot.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/mango.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/PineApple.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);


	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Potato.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	//hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/RedOnion.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

	//	ItemDesc.etype = CItem::APPLE;
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
	//		return E_FAIL;

	//}
	//CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_Interact_WorkTool(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CItem::ITEMDESC ItemDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Interact.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ItemDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ItemDesc.m_ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Apple")))
		{
			ItemDesc.etype = CItem::APPLE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_WORKTOOL, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else
		{
			ItemDesc.etype = CItem::APPLE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_WORKTOOL, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Interact"));

	}
	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);

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
