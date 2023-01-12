#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"


#include "Camera_Dynamic.h"
#include "BackGround.h"
#include "Terrain.h"
#include "NonAnim.h"
#include "ModelManager.h"
#include "Imgui_Manager.h"
#include "TreasureBox.h"
//#include "Effect.h"
//#include "Sky.h"
//#include "UI.h"


CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

unsigned int APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = (CLoader*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_NextLevelID())
	{
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
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

HRESULT CLoader::Loading_ForLogoLevel()
{
	CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐 로딩 중."));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 로딩 중."));


	/* 셰이더 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("셰이더 로딩 중."));




	// CShader::Create(m_pDevice, m_pContext, TEXT(""), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);




	/* 객체 원형 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중."));
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	CModelManager*		pModelManager = GET_INSTANCE(CModelManager);

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐 로딩 중."));


	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	_matrix			PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picking_Symbol"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../Bin/Resources/Picking_Symbol/Picking_Symbol.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Boulder*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boulder.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Boulder/Boulder.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Warp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WarpHole.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/WarpHole/WarpHole.fbx", PivotMatrix))))
		return E_FAIL;

	//* 모델 로딩 중. */
	/*lstrcpy(m_szLoadingText, TEXT("게임 플레이 모델 로딩 중."));
	if (FAILED(Loading_ForGamePlayModel()))
		return E_FAIL;*/

	lstrcpy(m_szLoadingText, TEXT("던전 모델 로딩 중."));
	if (FAILED(Loading_ForDungeonModel()))
		return E_FAIL; 

	/*lstrcpy(m_szLoadingText, TEXT("룸 모델 로딩 중."));
	if (FAILED(Loading_ForRoomModel()))
		return E_FAIL;*/

	/*lstrcpy(m_szLoadingText, TEXT("타워 모델 로딩 중."));
	if (FAILED(Loading_ForTowerModel()))
		return E_FAIL; */


	/* 셰이더 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("셰이더 로딩 중."));

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModelforMaptool.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중."));

	
	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim"),
		CNonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBox"),
		CTreasureBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	RELEASE_INSTANCE(CModelManager);
	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_ForDungeonModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix			PivotMatrix = XMMatrixIdentity();

#pragma region TailCave
	for (int i = 1; i < 9; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			//const char* pFilePath = "../Bin/Resources/Meshes/Field/Field_%02d%c.fbx";

			_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Lv01TailCave_%02d%c.fbx"), i, j + 65);
			wsprintf(szFilePath, TEXT("../../../Bin/Resources/Meshes/NonAnim/TailCave/Lv01TailCave_%02d%c.fbx"), i, j + 65);

			char* FilePath = new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, szFilePath, MAX_PATH, FilePath, MAX_PATH, NULL, NULL);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, pModeltag,
				CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, FilePath, PivotMatrix))))
			{
				delete pModeltag;
				delete szFilePath;
				delete FilePath;

				continue;
			}
			CImgui_Manager::Get_Instance()->Add_TempTag(FilePath);
			CImgui_Manager::Get_Instance()->Add_TempTag(pModeltag);
			CImgui_Manager::Get_Instance()->Add_TempTag(szFilePath);

			delete FilePath;

		}
	}
#pragma endregion TailField


	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pawn.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Pawn/Pawn.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rola.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Rola/Rola.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BuzzBlob*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BuzzBlob.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/BuzzBlob/BuzzBlob.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TailBoss1.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/TailBoss/TailBoss1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tail1.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Tail/Tail1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_CollapseTile*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CollapseTile.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/CollapseTile/CollapseTile.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ClosedDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ClosedDoor.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/ClosedDoor/ClosedDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_RedZol*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ZolRed.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/ZolRed/ZolRed.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Keese*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Keese.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Keese/Keese.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_LockDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LockDoor.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/LockDoor/LockDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SquareBlock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SquareBlock.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/SquareBlock/SquareBlock.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Togezo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Togezo.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Togezo/Togezo.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BladeTrap*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BladeTrap.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/BladeTrap/BladeTrap.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BossDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BossDoor.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/BossDoor/BossDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_FullMoonCello*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FullMoonCello.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/FullMoonCello/FullMoonCello.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_LockBlock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LockBlock.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/LockBlock/LockBlock.fbx", PivotMatrix))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix			PivotMatrix = XMMatrixIdentity();

#pragma region Field
	for (int i = 0; i < 17; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			//const char* pFilePath = "../Bin/Resources/Meshes/Field/Field_%02d%c.fbx";

			_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Field_%02d%c.fbx"), i, j + 65);
			wsprintf(szFilePath, TEXT("../../../Bin/Resources/Meshes/NonAnim/Field/Field_%02d%c.fbx"), i, j + 65);

			char* FilePath = new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, szFilePath, MAX_PATH, FilePath, MAX_PATH, NULL, NULL);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, pModeltag,
				CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, FilePath, PivotMatrix))))
			{
				delete pModeltag;
				delete szFilePath;
				delete FilePath;

				continue;
			}

			CImgui_Manager::Get_Instance()->Add_TempTag(pModeltag);
			CImgui_Manager::Get_Instance()->Add_TempTag(szFilePath);

			delete FilePath;

		}
	}

#pragma endregion Field

	
	/* Model for Monster */
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MoblinSword.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/MoblinSword/MoblinSword.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Octorock.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Octorock/Octorock.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SmallKey.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DgnKey/SmallKey/SmallKey.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FootSwitch.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Switch/FootSwitch.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox.fbx"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/TreasureBox/TreasureBox.fbx", PivotMatrix))))
		return E_FAIL;



	/*For.Prototype_Component_Model_Lawn*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lawn.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Lawn/Lawn.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Lawn*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Grass/Grass.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Lawn*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass2x2.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Grass2x2/Grass2x2.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TailStatue.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/TailStatue/TailStatue.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveShutter*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TailCaveShutter.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/TailCaveShutter/TailCaveShutter.fbx", PivotMatrix))))
		return E_FAIL;

	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CuccoKeeper.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/CuccoKeeper/CuccoKeeper.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cucco.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Cucco/Cucco.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Butterfly.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Butterfly/Butterfly.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MadamMeowMeow.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/MadamMeowMeow/MadamMeowMeow.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("QuadrupletGreen.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/QuadrupletGreen/QuadrupletGreen.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GrandmaUlrira.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/GrandmaUlrira/GrandmaUlrira.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BirdGreen.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/BirdGreen/BirdGreen.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BirdOrange.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/BirdOrange/BirdOrange.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Moosh.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Moosh/Moosh.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WeatherClock.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/WeatherClock/WeatherClock.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fox.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Fox/Fox.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("QuadrupletsMother.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/QuadrupletsMother/QuadrupletsMother.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("EaglesTower.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/EaglesTower/EaglesTower.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForRoomModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix			PivotMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_MarinHouse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarinHouse.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/MarinHouse/MarinHouse.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TelephoneBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TelephoneBox.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/TelephoneBox/TelephoneBox.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TelephoneBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Telephone.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Telephone/Telephone.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Shop*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shop.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Shop/Shop.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_CraneGame*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CraneGame.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/CraneGame/CraneGame.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ShopNpc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopNpc.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/ShopNpc/ShopNpc.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Arrow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Arrow.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Arrow/Arrow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_DogFood*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("DogFood.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DogFood/DogFood.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_HeartContainer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HeartContainer.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/HeartContainer/HeartContainer.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_HeartContainer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MagicRod.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MagicRod/MagicRod.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Bow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bow.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Link/Bow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Marin*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Marin.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Marin/Marin.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Crane*/
	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Crane.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Crane/Crane.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CraneFence.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/CraneFence/CraneFence.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MovingFloorCrane.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MovingFloorCrane/MovingFloorCrane.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CraneGameButton.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/CraneGameButton/CraneGameButton.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Necklace.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Necklace/Necklace.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("YoshiDoll.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/YoshiDoll/YoshiDoll.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MagicPowder.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MagicPowder/MagicPowder.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("RubyPurple.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Ruby/RubyPurple.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GameShopOwner.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/GameShopOwner/GameShopOwner.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tarin.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Tarin/Tarin.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bed.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Bed/Bed.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pot.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Pot/Pot.fbx", PivotMatrix))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForTowerModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix			PivotMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_MarinHouse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tower.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Tower/Tower.fbx", PivotMatrix))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

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
