#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"

CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForGamePlayLevel();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevel)
{
	CoInitializeEx(nullptr, 0);

	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel(void)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* �ؽ��� �ε� ��. */
	lstrcpy(m_szLoadingText, TEXT("Loading"));

	_matrix PivotMatrix;


	PivotMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.0f));


		//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Chandelier_Lod1"),
		//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bld_Chandelier_Lod1.fbx", "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Chandelier/Bld_Chandelier_Lod1/", PivotMatrix))))
		//return E_FAIL;

		//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Fence_01"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bld_Fence_01.fbx", "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Fence/Bld_Fence_01/", PivotMatrix))))
		//	return E_FAIL;

		//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Terrace_Floor01_Pts_Lod1"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bld_D04_Terrace_Floor01_Pts_Lod1.fbx", "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D04_Terrace_Floor01_Pts_Lod1/", PivotMatrix))))
		//	return E_FAIL;

		//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D01_EntStairs"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bld_D01_EntStairs.fbx", "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D01_EntStairs/", PivotMatrix))))
		//	return E_FAIL;

		//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_A_200_01"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bld_Floor_A_200_01.fbx", "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor_A_200_01/", PivotMatrix))))
		//	return E_FAIL;

		PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Arena_AllWall"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Arena_AllWall.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Arena_Wall_Frontgate"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Arena_Wall_Frontgate.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ArenaGate"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ArenaGate.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Banner"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Banner.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bench"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Bench.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Big_Wall01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Big_Wall01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateArch01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BigGateArch01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateDoor01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BigGateDoor01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateDoor02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BigGateDoor02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGatePillar01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BigGatePillar01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigTower01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BigTower01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BridgeLight"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/BridgeLight.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Castle01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Castle01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Castle01_Lod1"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Castle01_Lod1.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("castleHouse"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/castleHouse.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Chair.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Chair02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffWall01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffWall02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffWall03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CookingTable"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CookingTable.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cupboard"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Cupboard.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtain01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Curtain01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtain02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Curtain02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtainrail"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Curtainrail.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Around"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FighttingArenaFloor_Around.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FighttingArenaFloor_Center.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center2"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FighttingArenaFloor_Center2.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center4"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/FighttingArenaFloor_Center4.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01A"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Flower01A.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Flower01B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01C"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Flower01C.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Grass03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass04"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Grass04.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground_Plant02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ground_Plant02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ground01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ground02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ground03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground04"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ground04.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("House01_2F"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House01_2F.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("House01_2F_1200x800"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/House01_2F_1200x800.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HouseWindow"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/HouseWindow.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Inbench"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Inbench.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("InLight"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/InLight.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Intable"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Intable.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy01A"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ivy01A.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy01B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ivy01B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy1"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ivy1.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy2"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ivy2.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Light01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Light01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LightPillar"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/LightPillar.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/LineNew01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/LineNew02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/LineNew03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MarketProps01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MarketProps02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps02B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MarketProps02B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MarketProps03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MineProps01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MineProps01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;



		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MineProps02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/MineProps02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ModeLine"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ModeLine.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NAIL"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NAIL.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Palm01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Palm01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Planter01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Planter02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter02B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Planter02B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("River"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/River.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rubble01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Rubble01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rubble02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Rubble02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SAT_Wall01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SAT_Wall01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SAT_Wall02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SAT_Wall02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sharp_Plant01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Sharp_Plant01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01A"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ShopShader01A.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ShopShader01B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01C"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ShopShader01C.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;



		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalk01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SideWalk01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalk02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SideWalk02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalkCorner"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SideWalkCorner.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;



		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair02"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair02.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair03.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03b"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair03b.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03c"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair03c.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair04"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stair04.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stall01B"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Stall01B.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SteppingStone"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SteppingStone.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("table01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/table01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tannery"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Tannery.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;


		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WallHole"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/WallHole.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WallWell01"),
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/WallWell01.fbx", "../../../Bin/Bin_Data/NonAnim/Saternas/", PivotMatrix))))
			return E_FAIL;





	lstrcpy(m_szLoadingText, TEXT("Finish Loading"));

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVEL eNextLevel)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
