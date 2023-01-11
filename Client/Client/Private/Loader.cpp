#include "stdafx.h"
#include "..\Public\Loader.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Camera_2D.h"
#include "Data_Manager.h"
#include "Level_Manager.h"

//for Player
#include "Player.h"
#include "PlayerBullet.h"
#include "Weapon.h"

//for Monster
#include "Octorock.h"
#include "MoblinSword.h"
#include "Rola.h"
#include "MonsterBullet.h"
#include "Pawn.h"
#include "BuzzBlob.h"
#include "TailBoss.h"
#include "RedZol.h"
#include "Keese.h"
#include "Tail.h"
#include "Togezo.h"
#include "Cucco.h"
#include "Albatoss.h"

//for NPC
#include "ShopNpc.h"
#include "MarinNpc.h"
#include "CraneGameNpc.h"
#include "FieldNpc.h"
#include "TarinNpc.h"

//for UI
#include "UIScreen.h"
#include "InvenTile.h"
#include "UIButton.h"
#include "InvenItem.h"
#include "PlayerState.h"
#include "MessageBox.h"
#include "UIName.h"
#include "UIIcon.h"

//for Object 
#include "Terrain.h"
#include "NonAnim.h"
#include "DgnKey.h"
#include "TreasureBox.h"
#include "FootSwitch.h"
#include "CollapseTile.h"
#include "Door.h"
#include "SquareBlock.h"
#include "BladeTrap.h"
#include "Portal.h"
#include "PrizeItem.h"
#include "Grass.h"
#include "CraneButton.h"
#include "Crane.h"
#include "FieldDecoObject.h"
#include "Sky.h"

//for Effect
#include "ObjectEffect.h"
#include "PlayerEffect.h"
#include "FightEffect.h"
#include "MonsterEffect.h"
#include "UIEffect.h"
#include "FireEffect.h"
#include "TreasureBoxEffect.h"
#include "SwordTrail.h"


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
	case LEVEL_TAILCAVE:
		pLoader->Loading_ForTailCaveLevel();
		break;
	case LEVEL_ROOM:
		pLoader->Loading_ForRoomLevel();
		break;
	case LEVEL_TOWER:
		pLoader->Loading_ForTowerLevel();
		break;
	case LEVEL_END:
		pLoader->Loading_ForEndLevel();
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
	lstrcpy(m_szLoadingText, TEXT("UI 텍스쳐 로딩 중."));
	if (FAILED(Loading_For_UITexture()))
		return E_FAIL;

	/* 객체 원형 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("Static data 생성 중."));
	if (FAILED(Loading_ForStaticLevel()))
		return E_FAIL;

	/* 이펙트 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("Effect data 생성 중."));
	if (FAILED(Loading_For_Effect()))
		return E_FAIL;

	/* 객체 원형 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중."));
	if (FAILED(Loading_For_ObjectPrototype()))
		return E_FAIL;
	

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForStaticLevel()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);

	/* 텍스쳐 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("텍스쳐 로딩 중."));
	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Slash */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Slash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Meshes/Effect/Slash/slash_00.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Sky */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/SkyBox/SkyBoxDefault.dds"), 1))))
		return E_FAIL;

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 생성 중."));
	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 30, 30, -0.01f))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_Component_VIBuffer_Swordtrail*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_SwordTrail"),
		CVIBuffer_SwordTrail::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	lstrcpy(m_szLoadingText, TEXT("네비게이션 생성 중."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Field"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Filed_Navi2.dat")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_TailCave"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/TailCave_Navi.dat")))))
		return E_FAIL;
	
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Shop"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Shop_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Room"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Room_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_CraneGame"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/CraneGame_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Tower"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Tower_Navi.dat")))))
		return E_FAIL;

	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation_Telephone"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Telephone_Navi.dat")))))
		return E_FAIL;




	lstrcpy(m_szLoadingText, TEXT("Static 모델 생성 중."));
	/*For.Prototype_Component_Model_Link*/
	_matrix			PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Link"),
	//CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Link/Link.fbx", PivotMatrix))))
	//	return E_FAIL;
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Link"), LEVEL_STATIC, CData_Manager::DATA_ANIM);

	

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Marin"),
	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Marin/Marin.fbx", PivotMatrix))))
		return E_FAIL;



	
	PivotMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Warp*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_WarpHole"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/WarpHole/WarpHole.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BladeTrap*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Boulder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Boulder/Boulder.fbx", PivotMatrix))))
		return E_FAIL;
	
	/*For.Prototype_Component_Model_Lawn*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Lawn"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

	/*For.Prototype_Component_Model_Grass*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);

	/*For.Prototype_Component_Model_Grass2x2*/
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Grass2x2"), LEVEL_STATIC, CData_Manager::DATA_NONANIM);


	PivotMatrix = XMMatrixIdentity();


	/*For.Prototype_Component_Model_Arrow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Arrow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Arrow/Arrow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_DogFood*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DogFood"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DogFood/DogFood.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_HeartContainer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HeartContainer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/HeartContainer/HeartContainer.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_MagicRod*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_MagicRod"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MagicRod/MagicRod.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SeaLilysBell*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SeaLilysBell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/SeaLilysBell/SeaLilysBell.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Bow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ItemBow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Anim/Link/Bow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SmallKey*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmallKey"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DgnKey/SmallKey/SmallKey.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BossKey*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_BossKey"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DgnKey/BossKey/BossKey.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TailKey"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DgnKey/TailKey/TailKey.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Compass*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Compass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Compass/Compass.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_DgnMap*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DgnMap"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DgnMap/DgnMap.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_HeartRecovery*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HeartRecovery"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/HeartRecovery/HeartRecovery.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Ruby*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ruby"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Ruby/Ruby.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Feather*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Feather"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Feather/Feather.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_FullMoonCello*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_FullMoonCello"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/FullMoonCello/FullMoonCello.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Drum*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Drum"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Drum/Drum.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_ConchHorn*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ConchHorn"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/ConchHorn/ConchHorn.fbx", PivotMatrix))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Necklace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Necklace/Necklace.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_YoshiDoll"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/YoshiDoll/YoshiDoll.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_MagicPowder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MagicPowder/MagicPowder.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RubyPurple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Ruby/RubyPurple.fbx", PivotMatrix))))
		return E_FAIL;



	/*For.Prototype_Component_Model_Bow*/
	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixScaling(2.f, 2.f, 2.f) * XMMatrixTranslation(0.f, 0.2f, 0.2f);
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Bow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Link/Bow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_DogFood*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_DogFoodParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/DogFood/DogFood.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_MagicRod*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_MagicRodParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MagicRod/MagicRod.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordSlashParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Slash/SwordSlash_02.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Arrow*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ArrowParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Arrow/Arrow.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_HeartContainer*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HeartContainerParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/HeartContainer/HeartContainer.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TelephoneBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TelephoneParts"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Telephone/Telephone.fbx", PivotMatrix))))
		return E_FAIL;

	


	/*For.Prototype_Component_Model_TreasureBox*/
	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/TreasureBox/TreasureBox.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_FootSwitch*/
	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_FootSwitch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Switch/FootSwitch.fbx", PivotMatrix))))
		return E_FAIL;




	/* 셰이더 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("셰이더 로딩 중."));
	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCubeTexture"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxCubeTexture.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_EffectModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_EffectTexture.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_EffectModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Model"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_EffectModel.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_EffectModelFire */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_Fire"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/ShaderFiles/Shader_EffectFire.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	RELEASE_INSTANCE(CData_Manager);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForGamePlayLevel()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);

	lstrcpy(m_szLoadingText, TEXT("네비게이션 생성 중."));
	/* For.Prototype_Component_Navigation */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation_Filed_Navi_forChild"),
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Data/Filed_Navi_forChild.dat")))))
		return E_FAIL;

	

	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 생성 중."));
	_matrix			PivotMatrix = XMMatrixIdentity();

	for (int i = 4; i < 16; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			/*_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Field_%02d%c.fbx"), i, j+65);
			wsprintf(szFilePath, TEXT("../../../Bin/Resources/Meshes/NonAnim/Field/Field_%02d%c.fbx"), i, j + 65);
			
			char* FilePath= new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, szFilePath, MAX_PATH, FilePath, MAX_PATH, NULL, NULL);
			
			if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, pModeltag,
				CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, FilePath, PivotMatrix))))
			{
				delete pModeltag;
				delete szFilePath;
				delete FilePath;
			
							continue;
			}*/
			_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Field_%02d%c.fbx"), i, j + 65);
	
			if (FAILED(pData_Manager->Create_Try_BinModel(pModeltag, LEVEL_GAMEPLAY, CData_Manager::DATA_NONANIM)))
			{
				Safe_Delete(pModeltag);
				Safe_Delete(szFilePath);
			}
			Safe_Delete(szFilePath);
			pData_Manager->Add_ModelTag(pModeltag);
		}
	}

	


	/*For.Prototype_Component_Model_Octorock*/
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Octorock"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("MoblinSword"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);


	/*For.Prototype_Component_Model_TailStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TailStatue"),
	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/TailStatue/TailStatue.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_TailShutter*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TailCaveShutter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/TailCaveShutter/TailCaveShutter.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_OctorockBullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_OctorockBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Bullet/Octorock/Octorock.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("EaglesTower.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/EaglesTower/EaglesTower.fbx", PivotMatrix))))
		return E_FAIL;

	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("CuccoKeeper"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("MadamMeowMeow"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("QuadrupletGreen"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("GrandmaUlrira"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("QuadrupletsMother"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Fox"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("WeatherClock"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BirdGreen"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("BirdOrange"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Moosh"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);
	CData_Manager::Get_Instance()->Create_Try_BinModel(TEXT("Cucco"), LEVEL_GAMEPLAY, CData_Manager::DATA_ANIM);


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Butterfly"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Butterfly/Butterfly.fbx", PivotMatrix))))
		return E_FAIL;

	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	/* 객체 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("객체 생성 중."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	RELEASE_INSTANCE(CData_Manager);
	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;


	return S_OK;
}

HRESULT CLoader::Loading_ForTailCaveLevel()
{

	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	CData_Manager* pData_Manager = GET_INSTANCE(CData_Manager);


	_matrix			PivotMatrix = XMMatrixIdentity();
	/* 모델 로딩 중. */
	lstrcpy(m_szLoadingText, TEXT("모델 생성 중."));
	for (int i = 1; i < 9; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			//const char* pFilePath = "../Bin/Resources/Meshes/Field/Field_%02d%c.fbx";

			/*_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Lv01TailCave_%02d%c.fbx"), i, j + 65);
			wsprintf(szFilePath, TEXT("../../../Bin/Resources/Meshes/NonAnim/TailCave/Lv01TailCave_%02d%c.fbx"), i, j + 65);

			char* FilePath = new char[MAX_PATH];
			WideCharToMultiByte(CP_ACP, 0, szFilePath, MAX_PATH, FilePath, MAX_PATH, NULL, NULL);

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, pModeltag,
				CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, FilePath, PivotMatrix))))
			{
				Safe_Delete(pModeltag);
				Safe_Delete(szFilePath);
				Safe_Delete(FilePath);

				continue;
			}
			Safe_Delete(szFilePath);
			Safe_Delete(FilePath);
			CData_Manager::Get_Instance()->Add_ModelTag(pModeltag);*/


			_tchar*			pModeltag = new _tchar[MAX_PATH];
			_tchar*			szFilePath = new _tchar[MAX_PATH];
			wsprintf(pModeltag, TEXT("Lv01TailCave_%02d%c.fbx"), i, j + 65);

			if (FAILED(pData_Manager->Create_Try_BinModel(pModeltag, LEVEL_TAILCAVE, CData_Manager::DATA_NONANIM)))
			{
				Safe_Delete(pModeltag);
				Safe_Delete(szFilePath);
			}
			Safe_Delete(szFilePath);
			pData_Manager->Add_ModelTag(pModeltag);
		}

		int a = 0;
	}


	/*For.Prototype_Component_Model_Rola*/
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Rola"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Rola/Rola.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_RolaBullet*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_RolaBullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Bullet/RollingSpike/RollingSpike.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Pawn*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Pawn"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Pawn/Pawn.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Keese*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Keese"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Keese/Keese.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_BuzzBlob*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BuzzBlob"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/BuzzBlob/BuzzBlob.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/TailBoss/TailBoss1.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/TailBoss/TailBoss2.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_TailBoss3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/TailBoss/TailBoss3.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Tail*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Tail1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Tail/Tail1.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_Tail*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Tail2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Tail/Tail2.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_Tail*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Tail3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Tail/Tail3.fbx", PivotMatrix))))
		return E_FAIL;

	/* For.Prototype_Component_Model_CollapseTile */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_CollapseTile"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/CollapseTile/CollapseTile.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_RedZol*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_RedZol"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/ZolRed/ZolRed.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ClosedDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_ClosedDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/ClosedDoor/ClosedDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_LockDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_LockDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/LockDoor/LockDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SquareBlock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_SquareBlock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/SquareBlock/SquareBlock.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_LockBlock*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_LockBlock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/LockBlock/LockBlock.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Togezo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_Togezo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Monster/Togezo/Togezo.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BladeTrap*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BladeTrap"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/BladeTrap/BladeTrap.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_BossDoor*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Model_BossDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Obj/Door/BossDoor/BossDoor.fbx", PivotMatrix))))
		return E_FAIL;

	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TAILCAVE, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	RELEASE_INSTANCE(CData_Manager);
	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForRoomLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

	_matrix			PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	/*For.Prototype_Component_Model_ShopNpc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_ShopNpc"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/ShopNpc/ShopNpc.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GameShopOwner*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_GameShopOwner"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/GameShopOwner/GameShopOwner.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GameShopOwner*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_Tarin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Tarin/Tarin.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixIdentity();
	
	/*For.Prototype_Component_Model_MarinHouse*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("MarinHouse.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/MarinHouse/MarinHouse.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Shop*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Shop.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Shop/Shop.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_CraneGame*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("CraneGame.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/CraneGame/CraneGame.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TelephoneBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("TelephoneBox.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/TelephoneBox/TelephoneBox.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TelephoneBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_Telephone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Telephone/Telephone.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Model_Crane*/
	PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_Crane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Crane/Crane.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Crane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_CraneFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/CraneFence/CraneFence.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_MovingFloorCrane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/MovingFloorCrane/MovingFloorCrane.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Prototype_Component_Model_CraneGameButton"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/CraneGameButton/CraneGameButton.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Bed.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Bed/Bed.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TailCaveStatue*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_ROOM, TEXT("Pot.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Obj/Pot/Pot.fbx", PivotMatrix))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForTowerLevel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 콜라이더 생성 중. */
	lstrcpy(m_szLoadingText, TEXT("콜라이더 생성 중."));

	/* For.Prototype_Component_Collider_AABB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Prototype_Component_Collider_OBB"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_SPHERE */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	_matrix			PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	

	PivotMatrix = XMMatrixIdentity();

	/*For.Prototype_Component_Model_Tower*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Tower.fbx"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Tower/Tower.fbx", PivotMatrix))))
		return E_FAIL;

	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	/*For.Prototype_Component_Model_Albatoss*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Albatoss"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Resources/Meshes/Anim/Albatoss/Albatoss.fbx", PivotMatrix))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_TOWER, TEXT("Albatoss_Feather"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/NonAnim/Bullet/Albatoss/Albatoss_Feather.fbx", PivotMatrix))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_ForEndLevel()
{
	m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_ObjectPrototype()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/*For.Prototype_GameObject_SwordTrail*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SwordTrail"),
		CSwordTrail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectTreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBoxEffect"),
		CTreasureBoxEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectFire*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FireEffect"),
		CFireEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UIIcon*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIIcon"),
		CUIIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UIEffect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIEffect"),
		CUIEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_UIName*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UIName"),
		CUIName::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_MonsterEffect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterEffect"),
		CMonsterEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_ObjectEffect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ObjectEffect"),
		CObjectEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_PlayerEffect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_AttackEffect"),
		CFightEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_PlayerEffect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerEffect"),
		CPlayerEffect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Tarin*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TarinNpc"),
		CTarinNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Albatoss*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Albatoss"),
		CAlbatoss::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For.Prototype_GameObject_Sky*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For.Prototype_GameObject_FieldDecoObject*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FieldDecoObject"),
		CFieldDecoObject::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For.Prototype_GameObject_Cucco*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Cucco"),
		CCucco::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_CuccoKeeper*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CuccoKeeper"),
		CFieldNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Crane*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Crane"),
		CCrane::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_CraneButton*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CraneButton"),
		CCraneButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_CraneGameNpc*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CraneGameNpc"),
		CCraneGameNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_MarinNpc*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MarinNpc"),
		CMarinNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_ShopNpc*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ShopNpc"),
		CShopNpc::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Grass*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Grass"),
		CGrass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_PrizeItem*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PrizeItem"),
		CPrizeItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Camera2D*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_2D"),
		CCamera_2D::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Portal*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Portal"),
		CPortal::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_BladeTrap*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BladeTrap"),
		CBladeTrap::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Togezo*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Togezo"),
		CTogezo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Tail*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Tail"),
		CTail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_SquareBlock*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SquareBlock"),
		CSquareBlock::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Keese*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Keese"),
		CKeese::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_RedZol*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RedZol"),
		CRedZol::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Door*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Door"),
		CDoor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_CollapseTile*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CollapeTile"),
		CCollapseTile::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_TailBoss*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TailBoss"),
		CTailBoss::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_BuzzBlob*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BuzzBlob"),
		CBuzzBlob::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_Camera_Dynamic */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Dynamic"),
		CCamera_Dynamic::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_UI"),
		CUIScreen::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For.Prototype_GameObject_UI */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_InvenTile_UI"),
		CInvenTile::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CUIButton"),
		CUIButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CInvenItem"),
		CInvenItem::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim"),
		CNonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerState"),
		CPlayerState::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MessageBox"),
		CMessageBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/*For.Prototype_GameObject_Player */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_PlayerBullet"),
		CPlayerBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	/*For.Prototype_GameObject_Monster */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Pawn"),
		CPawn::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Rola"),
		CRola::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Octorock"),
		COctorock::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MoblinSword"),
		CMoblinSword::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MonsterBullet"),
		CMonsterBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	/*For.Prototype_GameObject_Object */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_DgnKey"),
		CDgnKey::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TreasureBox"),
		CTreasureBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FootSwitch"),
		CFootSwitch::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_UITexture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Icon/Icon_%02d.png"), 15))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Name"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Name/Name_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CraneGameTalk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/CraneGameTalk_%d.dds"), 7))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MarinTalk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/MarinTalk_%d.dds"), 21))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Rupee"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Ruby/Rupee_00.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Number/Number_%d.dds"), 10))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GetMessage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/MesGetFrame_%02d.dds"), 22))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FieldNpcMessage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/FieldNpcTalk_%d.dds"), 8))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_AlbatossMessage"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/TowerTalk_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TarinTalk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/TarinTalk_%d.dds"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TelephoneTalk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/TelephoneTalk_%d.dds"), 17))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ShopTalk"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Message/ShopTalk_%d.dds"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EndingScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Screen/EndScreen.dds"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingScreen_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Screen/LoadingScreen.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_TitleScreen"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Screen/TitleScreen.dds"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GamePlayScreen_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Screen/PlayGameScreen_%d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InvenTile"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/InvenTile/InvenTile_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipTile"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/InvenTile/EquipTile_%d.dds"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ButtonX"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Button/BtnX_%02d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ButtonY"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Button/BtnY_%02d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_InteractButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Button/InteractBT_%d.dds"), 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ChoiceButton"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Button/ChoiceBT_%d.dds"), 27))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EquipItem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/EqItem_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UsableItem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/Item_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DungeonKey"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/CollectDgnKey_%02d.png"), 6))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_CollectItem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/CollectItem_%d.png"), 9))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_QuestItem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/QuestItem_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PrizeItem"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Item/DgnItem_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Hp/Heart%02d.dds"), 5))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmallKey"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/UI/Key/SmallKey_00.dds"), 1))))
		return E_FAIL;


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_For_Effect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	_matrix			PivotMatrix = XMMatrixIdentity();

#pragma region SwordEffect

	PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	/*For.Prototype_Component_Model_SwordSlash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordSlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Slash/SwordSlash_02.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SwordBeam*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordBeam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/SwordBeam/SwordBeam.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SwordSwish0*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordSwish0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Slash/Swish_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_SwordSwish1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordSwish1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Slash/Swish_2.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_RolCut0*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RollCut"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/RollCut/RollCut_2.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_RolCut1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RollCut_Blast"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/RollCut/RollCut_1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Arc*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordArc"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Slash/Arc.fbx", PivotMatrix))))
		return E_FAIL;

#pragma endregion SwordEffect

#pragma region FightEffect
	/*For.Prototype_Component_Model_HitFlash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HitFlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Hit/HitFlash_2.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_HitRing*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HitRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Hit/HitRing_1.fbx", PivotMatrix))))
		return E_FAIL;
	/*For.Prototype_Component_Model_HitSpark*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_HitSpark"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Hit/HitSpark.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GuardFlash*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardFlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Guard/GuardFlash.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GuardRing*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Guard/GuardRing_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GuardRing_2*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardRing_2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Guard/GuardRing_1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GuardDust*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GuardDust"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Guard/GuardDust.fbx", PivotMatrix))))
		return E_FAIL;
#pragma endregion FightEffect

#pragma region ItemEffect
	/*For.Prototype_Component_Model_RainbowRing*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RainbowRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Ring/RainbowRing.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_GradRing*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GradRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Ring/GradRing.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_RainBowHalo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_RainbowHalo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Halo/Halo_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureBoxHalo*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBoxHalo"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Halo/Halo_1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_TreasureBox_Effect"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/TreasureBox/TreasureBox_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_TreasureEntranceBox*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Treasure_EntranceBox"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/TreasureBox/TreasureBox_1.fbx", PivotMatrix))))
		return E_FAIL;

#pragma endregion ItemEffect
	
#pragma region MonsterEffect
	/*For.Prototype_Component_Model_Octorok_Ring*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Octorock_Ring"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Ring/Octorok_Ring.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Octorok_Stone*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Octorock_Stone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Stone/Octorok_Stone.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Octorok_SmokeSphere*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_SmokeSphere"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/SmokeSphere/SmokeSphere.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_ClawRing*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_ClawRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Ring/DistRing1.fbx", PivotMatrix))))
		return E_FAIL;

#pragma endregion MonsterEffect

	/*For.Prototype_Component_Model_GrassLeaf*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_GrassLeaf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Grass/Grass_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_LawnLeaf_0*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_LawnLeaf_0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Grass/Lawn_0.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_LawnLeaf_1*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_LawnLeaf_1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Grass/Lawn_1.fbx", PivotMatrix))))
		return E_FAIL;

	/*For.Prototype_Component_Model_Fire*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Model_Fire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Resources/Meshes/Effect/Fire/Fire1.fbx", PivotMatrix))))
		return E_FAIL;


	/*For.Prototype_Component_Ripple */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Ripple"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ripple/ripple_%02d.dds"), 4))))
		return E_FAIL;


	/*For.Prototype_Component_Texture_WandSmoke */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WandSmoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/WandSmoke/smoke_array_pattern_02_Slice_%d_.png"), 32))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Blast */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Blast"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Blast/blast_%02d.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Smoke */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/Smoke_%d.dds"), 32))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Smoke2 */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Smoke2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke2/Smoke_%d.dds"), 32))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_SmokeBack */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDeath"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DeathSmoke/Smoke_%02d.dds"), 5))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Flash */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Flash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Flash/flash_%02d.dds"), 7))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Grass */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Grass"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Grass/Grass_%d.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Glow */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Glow/glow_%02d.dds"), 12))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Cross */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cross"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Cross/cross_00.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Dissolve */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Dissolve/dissolve_02.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_SmokeDst */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SmokeDst"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DeathSmoke/smoke_%02d_dst.dds"), 3))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Ray */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Ray"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ray/ray_00.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Fog */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Fog"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Fog/cloud_00.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_FireNoise */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireNoise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Meshes/Effect/Fire/fire_00.dds"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_FireAlpha */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireAlphaForme"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Meshes/Effect/Fire/fire_forme_%02d.dds"), 3))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_FireAlpha */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_FireAlphaSmall"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Meshes/Effect/Fire/fire_small_%02d.dds"), 2))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Distorition */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Distorition"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distorition/distortion_00.png"), 1))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Effect */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SwordTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Slash/slash_%02d.dds"), 3))))
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
