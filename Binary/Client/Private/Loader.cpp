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

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐 로딩 중"));

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 로딩 중."));
	_matrix PivotMatrix = XMMatrixIdentity()*XMMatrixScaling(0.01f,0.01f,0.01f)*XMMatrixRotationY(180.f);


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SM_AST_Prop_Rock01"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SM_AST_Prop_Rock01.fbx", "../../../Bin/Bin_Data/NonAnim/Rock/", PivotMatrix))))
	//	return E_FAIL;


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Berserker.fbx", "../../../Bin/Bin_Data/Anim/Berserker/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Berserker.fbx", "../../../Bin/Bin_Data/Anim/Berserker/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Hawk.fbx", "../../../Bin/Bin_Data/Anim/Hawk/", PivotMatrix))))
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
	/*PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Desert_CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Desert_CliffRock.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Desert_CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Desert_CliffRock2.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Desert_CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Desert_CliffRock3.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Desert_CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Desert_CliffRock4.fbx", "../../../Bin/Bin_Data/NonAnim/CliffRock/", PivotMatrix))))
		return E_FAIL;*/

	///*For.Prototype_Component_Model_SWO1(R00)*/
	//PivotMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO1(R00)"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO1(R00).fbx", "../../../Bin/Bin_Data/NonAnim/SWO1(R00)/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_SWO1(R00)*/
	//PivotMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SWO1(R01)"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/SWO1(R01).fbx", "../../../Bin/Bin_Data/NonAnim/SWO1(R01)/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_IceWolf*/
	//PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Ice_Wolf.fbx","../../../Bin/Bin_Data/Anim/Ice_Wolf/", PivotMatrix))))
	//	return E_FAIL;

	///*For.Prototype_Component_Model_Alphen*/
////	_matrix PivotMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
//	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
//		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Ice_Wolf.fbx", "../../../Bin/Bin_Data/Anim/Ice_Wolf/", PivotMatrix))))
//		return E_FAIL;

	///*For.Prototype_Component_Model_Water_Plane*/
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Water_Plane"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Plane.fbx", "../../../Bin/Bin_Data/NonAnim/Water/"))))
	//	return E_FAIL;

	//////////////동적 오브젝트//////////////////////////////

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/TreasureBox02.fbx", "../../../Bin/Bin_Data/Anim/TreasureBox02/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02_Blue"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/TreasureBox02_Blue.fbx", "../../../Bin/Bin_Data/Anim/TreasureBox02_Blue/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02_Red"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/TreasureBox02_Red.fbx", "../../../Bin/Bin_Data/Anim/TreasureBox02_Red/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox00"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/TreasureBox00.fbx", "../../../Bin/Bin_Data/Anim/TreasureBox00/", PivotMatrix))))
	//	return E_FAIL;

	/////////////정적 오브젝트////////////////////

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Apple"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Apple.fbx", "../../../Bin/Bin_Data/NonAnim/Apple/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Egg"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Egg.fbx", "../../../Bin/Bin_Data/NonAnim/Egg/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Mushroom"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Mushroom.fbx", "../../../Bin/Bin_Data/NonAnim/Mushroom/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BAG_002"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NOBJ_BAG_002.fbx", "../../../Bin/Bin_Data/NonAnim/NOBJ_BAG_002/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BASK_001"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NOBJ_BASK_001.fbx", "../../../Bin/Bin_Data/NonAnim/NOBJ_BASK_001/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BOX_000"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NOBJ_BOX_000.fbx", "../../../Bin/Bin_Data/NonAnim/NOBJ_BOX_000/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NOBJ_BOX_002"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/NOBJ_BOX_002.fbx", "../../../Bin/Bin_Data/NonAnim/NOBJ_BOX_002/", PivotMatrix))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ORE_001.fbx", "../../../Bin/Bin_Data/NonAnim/ORE_001/", PivotMatrix))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_002"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ORE_002.fbx", "../../../Bin/Bin_Data/NonAnim/ORE_002/", PivotMatrix))))
	//	return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ORE_003"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/ORE_003.fbx", "../../../Bin/Bin_Data/NonAnim/ORE_003/", PivotMatrix))))
	//	return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Meshes/Pot.fbx", "../../../Bin/Bin_Data/NonAnim/Pot/", PivotMatrix))))
		return E_FAIL;

	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Slime"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Meshes/Slime.fbx", "../../../Bin/Bin_Data/Anim/Slime/", PivotMatrix))))
	//	return E_FAIL;	


	/* 셰이더 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("셰이더 로딩 중."));

	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중."));

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

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
