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

	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Slash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Slash.fbx", "../../../Bin/Bin_Data/Effect/Slash/", PivotMatrix))))
		return E_FAIL;*/

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock01.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock01LOD3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock01LOD3.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock02"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock02.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock02LOD2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock02LOD2.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock03LOD3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock03LOD3.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock03"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock03.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock04"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock04.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock04LOD3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock04LOD3.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;

	/*For.Prototype_Component_Model_Desert_CliffRock*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffRock.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffRock2.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock3"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffRock3.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock4"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CliffRock4.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
	//	return E_FAIL;

	//_matrix PivotMatrix;


	///*For.Prototype_Component_Model_SWO1(R00)*/
	/*PivotMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO1(R01)"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/SWO1(R01).fbx", "../../../Bin/Bin_Data/NonAnim/SWO1(R01)/", PivotMatrix))))
		return E_FAIL;*/

	///*For.Prototype_Component_Model_SWO1(R00)*/
	//PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO00"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO00/SWO0.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO00/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO01/SWO1.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO01/", PivotMatrix))))
	//	return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f)/* * XMMatrixRotationX(XMConvertToRadians(90.f))*/;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO02"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO2(R00).fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO2(R00)/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO03"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO03/SWO3.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO03/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO04"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO04/SWO4.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO04/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO05"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO05/SWO5.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO05/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO06"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO06/SWO6.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO06/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO07"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO07/SWO7.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO07/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO08"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO08/SWO8.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO08/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO09"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO09/SWO9.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO09/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO10"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO10/SWO10.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO10/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO11"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO11/SWO11.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO11/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO12"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO12/SWO12.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO12/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO13"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO13/SWO13.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO13/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO14"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO14/SWO14.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO14/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO15"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO15/SWO15.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO15/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO16"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO16/SWO16.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO16/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO17"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO17/SWO17.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO17/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO18"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO18/SWO18.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO18/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO19"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO19/SWO19.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO19/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO20"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO20/SWO20.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO20/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO21"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO21/SWO21.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO21/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO22"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO22/SWO22.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO22/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO23"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO23/SWO23.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO23/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO24"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO24/SWO24.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO24/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO25"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO25/SWO25.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO25/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO26"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO26/SWO26.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO26/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO27"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO27/SWO27.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO27/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO28"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO28/SWO28.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO28/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO29"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO29/SWO29.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO29/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO31"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO31/SWO31.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO31/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO32"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO32/SWO32.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO32/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO33"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO33/SWO33.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO33/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO100"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO(ARI)/SWO100/SWO100.fbx", "../../../Bin/Bin_Data/NonAnim/AlphenWeapon/SWO100/", PivotMatrix))))
	//	return E_FAIL;


	///*For.Prototype_Component_Model_IceWolf*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ice_Wolf.fbx", "../../../Bin/Bin_Data/Anim/Ice_Wolf/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_Hawk*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Hawk.fbx", "../../../Bin/Bin_Data/Anim/Hawk/", PivotMatrix))))
	//	return E_FAIL;
	
	/////*For.Prototype_Component_Model_Berserker*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Berserker.fbx", "../../../Bin/Bin_Data/Anim/Berserker/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_Slime*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)/* * XMMatrixRotationY(XMConvertToRadians(180.0f))*/;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Akizame"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Akizame.fbx", "../../../Bin/Bin_Data/Effect/Akizame/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderRt1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CylinderRt1.fbx", "../../../Bin/Bin_Data/Effect/Cylinder/", PivotMatrix))))
	//	return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)/* * XMMatrixRotationY(XMConvertToRadians(180.0f))*/;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderWind"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/CylinderWind.fbx", "../../../Bin/Bin_Data/Effect/Cylinder/", PivotMatrix))))
	//	return E_FAIL;


	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)/* * XMMatrixRotationY(XMConvertToRadians(180.0f))*/;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_Ak1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ring_Ak1.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
	//	return E_FAIL;


	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)/* * XMMatrixRotationY(XMConvertToRadians(180.0f))*/;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_Ak2"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ring_Ak2.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
	//	return E_FAIL;


	/*PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plane_Flash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Plane_Flash.fbx", "../../../Bin/Bin_Data/Effect/Plane/", PivotMatrix))))
		return E_FAIL;*/


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane4"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Hurricane4.fbx", "../../../Bin/Bin_Data/Effect/Water/", PivotMatrix))))
	//	return E_FAIL;

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_nm_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ring_nm_03.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ice0.fbx", "../../../Bin/Bin_Data/Effect/Ice/", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ice1.fbx", "../../../Bin/Bin_Data/Effect/Ice/", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ice2.fbx", "../../../Bin/Bin_Data/Effect/Ice/", PivotMatrix))))
		return E_FAIL;*/
	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_tk_05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Spiral_tk_05.fbx", "../../../Bin/Bin_Data/Effect/Spiral/", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_su_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Spiral_su_02.fbx", "../../../Bin/Bin_Data/Effect/Spiral/", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pipe_is_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Pipe_is_01.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_hn_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ring_hn_03.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_hn_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ring_hn_02.fbx", "../../../Bin/Bin_Data/Effect/Ring/", PivotMatrix))))
		return E_FAIL;*/

	PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sion.fbx", "../../../Bin/Bin_Data/Anim/Sion/", PivotMatrix))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rinwell"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Rinwell.fbx", "../../../Bin/Bin_Data/Anim/Rinwell/", PivotMatrix))))
	//	return E_FAIL;

	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sion"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sion.fbx", "../../../Bin/Bin_Data/Anim/Sion/", PivotMatrix))))
	//	return E_FAIL;


	/*PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sion.fbx", "../../../Bin/Bin_Data/Anim/Sion/", PivotMatrix))))
		return E_FAIL;*/


	/*PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Hurricane3.fbx", "../../../Bin/Bin_Data/Water/", PivotMatrix))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Hurricane4.fbx", "../../../Bin/Bin_Data/Effect/Water/", PivotMatrix))))
		return E_FAIL;*/

	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rinwell"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Rinwell.fbx", "../../../Bin/Bin_Data/Anim/Rinwell/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NpcFemaleYoung"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/NpcFemaleYoung.fbx", "../../../Bin/Bin_Data/Anim/NpcFemaleYoung/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_is01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Spiral_is01.fbx", "../../../Bin/Bin_Data/Effect/Spiral/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_is05"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Spiral_is05.fbx", "../../../Bin/Bin_Data/Effect/Spiral/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_ny05"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Spiral_ny05.fbx", "../../../Bin/Bin_Data/Effect/Spiral/", PivotMatrix))))
	//	return E_FAIL;

	/*For.Prototype_Component_Model_Alphen*/
	/*PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Alphen"),

		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Alphen.fbx", "../../../Bin/Bin_Data/Anim/Alphen/", PivotMatrix))))
		return E_FAIL;*/
	///*For.Prototype_Component_Model_Alphen*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMC0onvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sion"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Sion.fbx", "../../../Bin/Bin_Data/Anim/Sion/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_SIOW(O0)*/
	//PivotMatrix = XMMatrixScaling(0.03f, 0.03f, 0.03f)/* * XMMatrixRotationX(XMConvertToRadians(180.f))*//* * XMMatrixRotationZ(XMConvertToRadians(90.f))*/ * XMMatrixRotationX(XMConvertToRadians(270.f));
	////PivotMatrix.r[0] *= -1.f;
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SIOW"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SIOW(00).fbx", "../../../Bin/Bin_Data/NonAnim/SIOW(00)/", PivotMatrix))))
	//	return E_FAIL;

	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SionHead"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SionHead.fbx", "../../../Bin/Bin_Data/Anim/Sion/", PivotMatrix))))
		return E_FAIL;*/

		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fence"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Fence_001.fbx", "../../../Bin/Bin_Data/NonAnim/Alltexture/", PivotMatrix))))
		//	return E_FAIL;

		//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LightPillar"),
		//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/StreetLight_002.fbx", "../../../Bin/Bin_Data/NonAnim/Alltexture/", PivotMatrix))))
		//	return E_FAIL;




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
