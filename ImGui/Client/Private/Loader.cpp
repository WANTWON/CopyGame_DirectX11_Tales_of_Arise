#include "stdafx.h"
#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Dynamic.h"
#include "Camera_Action.h"
#include "BackGround.h"
#include "Terrain.h"
#include "NonAnim.h"
#include "ModelManager.h"
#include "Imgui_Manager.h"
#include "TreasureBox.h"
#include "EffectTexture.h"
#include "EffectMesh.h"
#include "ParticleSystem.h"
#include <DirectXTK/ScreenGrab.h>
#include "Anim.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
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
	case LEVEL_LOGO:
		pLoader->Loading_ForLogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		pLoader->Loading_ForClient();
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
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	lstrcpy(m_szLoadingText, TEXT("Loading Level"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Finished Loading"));

	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForClient()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CModelManager* pModelManager = GET_INSTANCE(CModelManager);

	/* �ؽ��� �ε� ��. */
	lstrcpy(m_szLoadingText, TEXT("Loading Texture"));

	/*For.Prototype_Component_Texture_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/ForImguiDebug/Terrain%d_D.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush2.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picking_Symbol"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Picking_Symbol/Picking_Symbol.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Loading For Shader"));

	/* For.Prototype_Component_Shader_VtxModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxModelforMaptool.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Shaderfiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Loading For Prototype"));

	/*For.Prototype_GameObject_Player*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_NonAnim"),
		CNonAnim::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Anim"),
		CAnim::Create(m_pDevice, m_pContext))))
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

	/*For.Prototype_GameObject_Camera_Action */
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Action"),
		CCamera_Action::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectTexture*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectTexture"),
		CEffectTexture::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.Prototype_GameObject_EffectMesh*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_EffectMesh"),
		CEffectMesh::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*For.GameObject_ParticleSystem*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ParticleSystem"),
		CParticleSystem::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;
	CTerrain_Manager::Get_Instance()->Add_PrototypeTag(TEXT("Height"));


	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Terrain_HeightMap"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/HeightMap3.bmp"), false))))
		return E_FAIL;
	CTerrain_Manager::Get_Instance()->Add_PrototypeTag(TEXT("Terrain_HeightMap"));

	/*For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HeightMapPlane"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Height.bmp"), false))))
		return E_FAIL;
	CTerrain_Manager::Get_Instance()->Add_PrototypeTag(TEXT("HeightMapPlane"));


	//For Maptool
	if (FAILED(Loading_ForActor()))
		return E_FAIL;

	//////For Maptool
	//if (FAILED(Loading_ForMaptoolSnowFieldModel()))
	//	return E_FAIL;

	if (FAILED(Loading_ForMaptoolBossRoomModel()))
		return E_FAIL;

	//if (FAILED(Loading_ForMaptoolVillageModel()))
	//	return E_FAIL;

	/* if (FAILED(Loading_ForMaptoolCityModel()))
	 	return E_FAIL;

	 if (FAILED(Loading_ForMaptoolPlant()))
	 	return E_FAIL;*/

	/*if (FAILED(Loading_ForMaptoolInteriorModel()))
		return E_FAIL;

	if (FAILED(Loading_ForMaptoolWorkRoomModel()))
		return E_FAIL;

	if (FAILED(Loading_ForMaptoolKitchenModel()))
		return E_FAIL;*/

	////For Effect
	//if (FAILED(Loading_ForEffect()))
	//	return E_FAIL;
	// 
	//if (FAILED(Loading_ForEffectTexture()))
	//	return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("Finished"));

	RELEASE_INSTANCE(CModelManager);
	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_ForMaptoolSnowFieldModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Vehicle_Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Vehicle_Boat/Vehicle_Boat.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Vehicle_Boat"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/CliffRock4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock4"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Birch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Birch"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Birch1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Birch1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Birch2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Birch2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Birch2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Broken_Tree"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Broken_Tree"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Broken_Tree2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Broken_Tree2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Broken_Tree2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bush"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bush"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bush1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Bush1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bush1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Conifer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Conifer"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Conifer2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Conifer2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Conifer3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Conifer3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Conifer3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Dead_Grass"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Dead_Grass"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Dead_Grass1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Grass1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Dead_Grass1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Dead_Tree0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree0.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Dead_Tree0"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Dead_Tree1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Dead_Tree1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Dead_Tree1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tree1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Tree1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tree2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Tree2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tree5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WinterNature/Tree5.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Tree5"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fence01CornerOut_100_100"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Fence01CornerOut_100_100.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fence01CornerOut_100_100"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fence02_200_100"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Fence02_200_100.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fence02_200_100"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fence02_400_100"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Fence02_400_100.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fence02_400_100"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WoodFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Fence.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WoodFence"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GateArch01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/GateArch01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("GateArch01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GateDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/GateDoor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("GateDoor"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GateDoor_Lod2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/GateDoor_Lod2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("GateDoor_Lod2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lamp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lamp.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lamp"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lamp1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lamp1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lamp1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lamp2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lamp2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lamp2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lamp3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lamp3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lamp3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lamp4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lamp4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lamp4"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StreetLight_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Light/StreetLight_002.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("StreetLight_002"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StreetLightLamp_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Light/StreetLightLamp_001.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("StreetLightLamp_001"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LampOrganant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/LampOrganant.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LampOrganant"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LampOrganant2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/LampOrganant2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LampOrganant2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lattice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Lattice.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lattice"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rock1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rock2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Scale"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Scale.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Scale"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Snow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Snow"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Snow2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Snow2"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Snow3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Snow3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Snow3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stairs"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stairs.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stairs"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stalagmite3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stalagmite3"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stalagmite4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stalagmite4"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stalagmite5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Stalagmite5.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stalagmite5"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SteelFence"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/SteelFence.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SteelFence"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Torch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Torch.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Torch"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WallPillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WallPillar.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WallPillar"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WoodShielf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WoodShielf"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WoodShielf2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/WoodShielf2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WoodShielf2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Snow_Mountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Snow_Mountain/Snow_Mountain.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Snow_Mountain"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bag"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bag"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bag2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bag2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bag3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bag3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bag3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bale"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bale"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Barrel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Barrel"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bale2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bale2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Barrel3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Barrel3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Barrel3"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bale3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bale3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bale3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Basket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Basket"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Basket2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Basket2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Basket2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Beam"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Beam.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Beam"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Boat.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Boat"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bonfire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bonfire.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bonfire"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bowl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bowl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bowl"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Box"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Box"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Box2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Box2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Box3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Box3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Box3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bucket"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bucket.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bucket"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BushWood"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/BushWood.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BushWood"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Cage.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cage"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CampFire"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CampFire.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CampFire"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fence2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Fence2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fence2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hut"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Hut.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Hut"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SmallRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/SmallRock.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SmallRock"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SmallRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/SmallRock2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SmallRock2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WoodFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/WoodFloor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WoodFloor"));

	/*For.Prototype_Component_Model_Apple*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Apple/Apple.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Apple"));

	/*For.Prototype_Component_Model_Egg*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Egg"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Egg/Egg.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Egg"));

	/*For.Prototype_Component_Model_Mushroom*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Mushroom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Mushroom/Mushroom.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Mushroom"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Water"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Water/Plane.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Water"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Jewel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/ORE_003/ORE_003.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Jewel"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lettuce_001"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_001.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lettuce_001"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Lettuce_002"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Lettuce/Lettuce_002.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Lettuce_002"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bridge_End"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_End.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bridge_End"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bridge_Middle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/Bridge_Middle.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bridge_Middle"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CommonBridge"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Bld/CommonBridge.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CommonBridge"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GroundPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/GroundPlant.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("GroundPlant"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SlimPlant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Plant/SlimPlant.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SlimPlant"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("TreasureBox02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/TreasureBox02/TreasureBox02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Crystal"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Crystal/Crystal.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Crystal"));



	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolBossRoomModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	///////////////////////Ceil////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Ceil_A_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Ceil/Bld_Ceil_A_Lod1/Bld_Ceil_A_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Ceil_A_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Ceil_A2_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Ceil/Bld_Ceil_A2_Lod1/Bld_Ceil_A2_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Ceil_A2_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ceiling"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Ceil/Ceiling/Ceiling.dat"))))
		return E_FAIL;        // 위쪽 바라보면 있음 
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ceiling"));
	

	/////////////////////Chandelier////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Chandelier_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Chandelier/Bld_Chandelier_Lod1/Bld_Chandelier_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Chandelier_Lod1"));


	///////////////////////Door////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D03_TallDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Door/Bld_D03_TallDoor/Bld_D03_TallDoor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D03_TallDoor"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Elevator_Door"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Door/Bld_Elevator_Door/Bld_Elevator_Door.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Elevator_Door"));


	///////////////////////Fence////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Fence_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Fence/Bld_Fence_01/Bld_Fence_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Fence_01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Fence_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Fence/Bld_Fence_02/Bld_Fence_02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Fence_02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Fence04_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Fence/Bld_Fence04_Lod1/Bld_Fence04_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Fence04_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_FencePartsB_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Fence/Bld_FencePartsB_Lod1/Bld_FencePartsB_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_FencePartsB_Lod1"));


	///////////////////////Floor////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D01_EntStairs"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D01_EntStairs/Bld_D01_EntStairs.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D01_EntStairs"));


	//if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D03_Floor02_Detail_Lod1"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D03_Floor02_Detail_Lod1/Bld_D03_Floor02_Detail_Lod1.dat"))))
	//	return E_FAIL;   //3ds max에서 로드하면 온전하게 나오는데, 머테리얼 다 넣고 fbx파일로 볼 때는 구멍이 생겨있음. 
	//CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D03_Floor02_Detail_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Terrace_Floor01_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D04_Terrace_Floor01_A/Bld_D04_Terrace_Floor01_A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Terrace_Floor01_A"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Terrace_Floor01_Pts_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D04_Terrace_Floor01_Pts_Lod1/Bld_D04_Terrace_Floor01_Pts_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Terrace_Floor01_Pts_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Terrace_Floor02_Omt_02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_D04_Terrace_Floor02_Omt_02_Lod1/Bld_D04_Terrace_Floor02_Omt_02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Terrace_Floor02_Omt_02_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_A_200_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor_A_200_01/Bld_Floor_A_200_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_A_200_01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01/Bld_Floor01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor01_"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01_/Bld_Floor01_.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor01_"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor01_outside_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01_outside_Lod1/Bld_Floor01_outside_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor01_outside_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor01_outsideArch_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor01_outsideArch_Lod1/Bld_Floor01_outsideArch_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor01_outsideArch_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor02_Lod1/Bld_Floor02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor02_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor02_sub.props"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_Floor02_sub.props/Bld_Floor02_sub.props.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor02_sub.props"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_FloorPartsB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_FloorPartsB/Bld_FloorPartsB.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_FloorPartsB"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_under_Floor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Floor/Bld_under_Floor/Bld_under_Floor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_under_Floor"));


	///////////////////////Light////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prop_Light_stand_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Light/Prop_Light_stand_Lod1/Prop_Light_stand_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Prop_Light_stand_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prop_Light02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Light/Prop_Light02_Lod1/Prop_Light02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Prop_Light02_Lod1"));



	///////////////////////Pillar////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_outer_Pillar_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Pillar/Bld_outer_Pillar_Lod1/Bld_outer_Pillar_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_outer_Pillar_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Pillar01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Pillar/Bld_Pillar01_Lod1/Bld_Pillar01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Pillar01_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Pillar01_Sub_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Pillar/Bld_Pillar01_Sub_Lod1/Bld_Pillar01_Sub_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Pillar01_Sub_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Pillar02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Pillar/Bld_Pillar02_Lod1/Bld_Pillar02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Pillar02_Lod1"));


	///////////////////////Roaster////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prop_MP_roaster02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Roaster/Prop_MP_roaster02_Lod1/Prop_MP_roaster02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Prop_MP_roaster02_Lod1"));


	///////////////////////Throne////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D03_Throne"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Throne/Bld_D03_Throne/Bld_D03_Throne.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D03_Throne"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D03_Throne_Break_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Throne/Bld_D03_Throne_Break_Lod1/Bld_D03_Throne_Break_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D03_Throne_Break_Lod1"));


	///////////////////////Tiling////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_TilingA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Tiling/Bld_TilingA/Bld_TilingA.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_TilingA"));


	///////////////////////vine of thorns////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("POSE_A_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/vine of thorns/POSE_A_Lod1/POSE_A_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("POSE_A_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("POSE_B_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/vine of thorns/POSE_B_Lod1/POSE_B_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("POSE_B_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("POSE_C_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/vine of thorns/POSE_C_Lod1/POSE_C_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("POSE_C_Lod1"));


	///////////////////////Wall////////////////////////////////
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D03_Wall03_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_D03_Wall03_Lod1/Bld_D03_Wall03_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D03_Wall03_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Door01_Wall_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_D04_Door01_Wall_Lod1/Bld_D04_Door01_Wall_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Door01_Wall_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_FillWall_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_FillWall_01/Bld_FillWall_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_FillWall_01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_FloorPartsA_wall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_FloorPartsA_wall/Bld_FloorPartsA_wall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_FloorPartsA_wall"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Option02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Option02_Lod1/Bld_Option02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Option02_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_outer_Wall_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_outer_Wall_Lod1/Bld_outer_Wall_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_outer_Wall_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01/Bld_Wall01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall01_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01_A/Bld_Wall01_A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall01_A"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall01_B02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01_B02_Lod1/Bld_Wall01_B02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall01_B02_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall01_Coner_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall01_Coner_Lod1/Bld_Wall01_Coner_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall01_Coner_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall03_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_Wall03_Lod1/Bld_Wall03_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall03_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_WallB/Bld_WallB.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallB"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallB_corner_B_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_WallB_corner_B_Lod1/Bld_WallB_corner_B_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallB_corner_B_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallB_End"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Boss_AstralDoubt/Wall/Bld_WallB_End/Bld_WallB_End.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallB_End"));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolVillageModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FountainDel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/FountainDel.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FountainDel"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SkyDome"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/SkyDome.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SkyDome"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StoneBlock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/StoneBlock.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("StoneBlock"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("StoneBlockA"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/LawBattle/StoneBlockA.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("StoneBlockA"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Beam02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Beam/Beam02/Beam02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Beam02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CurveCeiling_01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Ceiling/CurveCeiling_01_Lod1/CurveCeiling_01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CurveCeiling_01_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("D01_Ceiling02_350x430_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Ceiling/D01_Ceiling02_350x430_Lod1/D01_Ceiling02_350x430_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("D01_Ceiling02_350x430_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Meal_Ceiling_01_A_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Ceiling/Meal_Ceiling_01_A_Lod1/Meal_Ceiling_01_A_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Meal_Ceiling_01_A_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chandelier01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Chandelier/Chandelier01_Lod1/Chandelier01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chandelier01_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("D04_Door_01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/D04_Door_01_Lod1/D04_Door_01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("D04_Door_01_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gim_Door_02_L_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/Gim_Door_02_L_Lod1/Gim_Door_02_L_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gim_Door_02_L_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gim_Door_02_R_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/Gim_Door_02_R_Lod1/Gim_Door_02_R_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gim_Door_02_R_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gim_Door01_01_L_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/Gim_Door01_01_L_Lod1/Gim_Door01_01_L_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gim_Door01_01_L_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gim_Door01_01_R_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/Gim_Door01_01_R_Lod1/Gim_Door01_01_R_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gim_Door01_01_R_Lod1"));


	/*if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Meal_Wall_Door_01_Set_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Door/MRG_Meal_Wall_Door_01_Set_Lod1/MRG_Meal_Wall_Door_01_Set_Lod1.dat"))))
		return E_FAIL;*/
	//CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Meal_Wall_Door_01_Set_Lod1"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Meal_Window_Wall_01_B_Library_Set"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Window/MRG_Meal_Window_Wall_01_B_Library_Set/MRG_Meal_Window_Wall_01_B_Library_Set.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Meal_Window_Wall_01_B_Library_Set"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Stairs_Deco_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Stairs_Deco_01/Bld_Stairs_Deco_01/Bld_Stairs_Deco_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Stairs_Deco_01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SultanDungeon_04_co"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/SultanDungeon_04_co/SultanDungeon_04_co.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SultanDungeon_04_co"));

#pragma region Floor
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_CurveFloor_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_CurveFloor_01/Bld_CurveFloor_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_CurveFloor_01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Floor_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_D04_Floor_01/Bld_D04_Floor_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Floor_01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Floor_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_D04_Floor_03/Bld_D04_Floor_03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Floor_03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Floor_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_D04_Floor_04/Bld_D04_Floor_04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Floor_04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_Floor_300x600"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_D04_Floor_300x600/Bld_D04_Floor_300x600.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_Floor_300x600"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_FloorSide_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_D04_FloorSide_03/Bld_D04_FloorSide_03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_FloorSide_03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Door01_SecondFloorA_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Door01_SecondFloorA_Lod1/Bld_Door01_SecondFloorA_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Door01_SecondFloorA_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_01_C_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_01_C_Lod1/Bld_Floor_01_C_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_01_C_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_01_Center_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_01_Center_Lod1/Bld_Floor_01_Center_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_01_Center_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_01_D"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_01_D/Bld_Floor_01_D.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_01_D"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_02/Bld_Floor_02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_02_300x600"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_02_300x600/Bld_Floor_02_300x600.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_02_300x600"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_03/Bld_Floor_03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_B_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_B_Lod1/Bld_Floor_B_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_B_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Floor_Stairs_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Floor_Stairs_02/Bld_Floor_Stairs_02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Floor_Stairs_02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Meal_Floor_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Meal_Floor_01/Bld_Meal_Floor_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Meal_Floor_01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Meal_Floor_02_A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/Bld_Meal_Floor_02_A/Bld_Meal_Floor_02_A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Meal_Floor_02_A"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Meal_Floor_01_Set"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Floor/MRG_Meal_Floor_01_Set/MRG_Meal_Floor_01_Set.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Meal_Floor_01_Set"));
#pragma endregion Floor

#pragma region Wall
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_CurveWall_01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_CurveWall_01_Lod1/Bld_CurveWall_01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_CurveWall_01_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D01_Bookshelf_02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_D01_Bookshelf_02_Lod1/Bld_D01_Bookshelf_02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D01_Bookshelf_02_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_FrontWall_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_D04_FrontWall_01/Bld_D04_FrontWall_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_FrontWall_01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_D04_WallFrontSide_01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_D04_WallFrontSide_01_Lod1/Bld_D04_WallFrontSide_01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_D04_WallFrontSide_01_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Door01_Wall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_Door01_Wall/Bld_Door01_Wall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Door01_Wall"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_OgiOrnament01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_OgiOrnament01/Bld_OgiOrnament01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_OgiOrnament01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_Wall_Door02_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_Wall_Door02_Lod1/Bld_Wall_Door02_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_Wall_Door02_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallCurve_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_WallCurve_01/Bld_WallCurve_01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallCurve_01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallParts_01_C_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_WallParts_01_C_Lod1/Bld_WallParts_01_C_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallParts_01_C_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallParts_03_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_WallParts_03_Lod1/Bld_WallParts_03_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallParts_03_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bld_WallParts_base"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/Bld_WallParts_base/Bld_WallParts_base.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bld_WallParts_base"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_D04_Meal_Wall_02_1p_Set_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/MRG_D04_Meal_Wall_02_1p_Set_Lod1/MRG_D04_Meal_Wall_02_1p_Set_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_D04_Meal_Wall_02_1p_Set_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Wall_Simple_1p_Library_Set_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/MRG_Wall_Simple_1p_Library_Set_Lod1/MRG_Wall_Simple_1p_Library_Set_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Wall_Simple_1p_Library_Set_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Wall01_Set_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/MRG_Wall01_Set_Lod1/MRG_Wall01_Set_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Wall01_Set_Lod1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MRG_Wall02_Set1_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Wall/MRG_Wall02_Set1_Lod1/MRG_Wall02_Set1_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MRG_Wall02_Set1_Lod1"));
#pragma endregion Wall


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolCityModel()
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rock1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Astrik/Rock2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rock2"));

	/*For.Prototype_Component_Model_Water_Plane*/
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Water"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Water/Plane.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Water"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Arena_AllWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Arena_AllWall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Arena_AllWall"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Arena_Wall_Frontgate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Arena_Wall_Frontgate.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Arena_Wall_Frontgate"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ArenaGate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ArenaGate.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ArenaGate"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bridge_base"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Inside/Bridge_base/Bridge_base.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bridge_base"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bench"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Bench.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bench"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Big_Wall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Big_Wall01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Big_Wall01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateArch01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigGateArch01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BigGateArch01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateDoor01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigGateDoor01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BigGateDoor01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGateDoor02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigGateDoor02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BigGateDoor02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigGatePillar01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigGatePillar01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BigGatePillar01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BigTower01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BigTower01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BigTower01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BridgeLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/BridgeLight.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BridgeLight"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Castle01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Castle01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Castle01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Castle01_Lod1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Castle01_Lod1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Castle01_Lod1"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("castleHouse"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/castleHouse.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("castleHouse"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Chair.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Chair02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffWall01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffWall02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffWall03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CliffWall03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffWall03"));




	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Around"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/FighttingArenaFloor_Around.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FighttingArenaFloor_Around"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/FighttingArenaFloor_Center.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FighttingArenaFloor_Center"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/FighttingArenaFloor_Center2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FighttingArenaFloor_Center2"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FighttingArenaFloor_Center4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/FighttingArenaFloor_Center4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FighttingArenaFloor_Center4"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Flower01A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Flower01A"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Flower01B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Flower01B"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Flower01C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Flower01C.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Flower01C"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fountain"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Fountain.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fountain"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ground01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ground02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ground03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ground04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("House01_2F"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/House01_2F.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("House01_2F"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("House01_2F_1200x800"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/House01_2F_1200x800.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("House01_2F_1200x800"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("house01_3F_1200x1200"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Castle_Outside/House/house01_3F_1200x1200/house01_3F_1200x1200.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("house01_3F_1200x1200"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("House16"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/House16.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("House16"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy01A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ivy01A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ivy01A"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy01B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ivy01B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ivy01B"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ivy1.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ivy1"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ivy2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ivy2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ivy2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Light01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Light01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Light01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LightPillar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LightPillar.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LightPillar"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LineNew01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LineNew02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("LineNew03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/LineNew03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("LineNew03"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MarketProps01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MarketProps02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps02B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps02B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MarketProps02B"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MarketProps03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MarketProps03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MarketProps03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MineProps01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MineProps01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MineProps01"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("MineProps02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/MineProps02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("MineProps02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ModeLine"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ModeLine.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ModeLine"));




	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Palm01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Palm01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Palm01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Planter01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Planter01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Planter02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Planter02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Planter02B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Planter02B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Planter02B"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("River"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/River.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("River"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rubble01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Rubble01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rubble01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rubble02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Rubble02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rubble02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SAT_Wall01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SAT_Wall01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SAT_Wall01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SAT_Wall02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SAT_Wall02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SAT_Wall02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sharp_Plant01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Sharp_Plant01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Sharp_Plant01"));




	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01A"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01A.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ShopShader01A"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ShopShader01B"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopShader01C"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/ShopShader01C.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ShopShader01C"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalk01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SideWalk01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SideWalk01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalk02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SideWalk02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SideWalk02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SideWalkCorner"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SideWalkCorner.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SideWalkCorner"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair01"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03b"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair03b.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair03b"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair03c"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair03c.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair03c"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stair04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stair04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stair04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Stall01B"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Stall01B.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Stall01B"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SteppingStone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/SteppingStone.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("SteppingStone"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("table01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/table01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("table01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Vehicle_Boat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Vehicle_Boat.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Vehicle_Boat"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WallHole"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/WallHole.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WallHole"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WallWell01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/WallWell01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WallWell01"));

	


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolInteriorModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("AugustDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/AugustDoor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("AugustDoor"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BedDouble"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BedDouble.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BedDouble"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BedSimple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BedSimple.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BedSimple"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BenchSmall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BenchSmall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BenchSmall"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BookForShelf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BookForShelf.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BookForShelf"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BookForShelf3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BookForShelf3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BookForShelf3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BookForShelf6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BookForShelf6.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BookForShelf6"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BooksPreset4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/BooksPreset4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BooksPreset4"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtain01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Curtain01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Curtain01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtain02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Curtain02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Curtain02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Curtainrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Curtainrail.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Curtainrail"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cabinet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cabinet"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cabinet3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cabinet3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cabinet4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cabinet4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cabinet4"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Carpet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Carpet.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Carpet"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Carpet3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Carpet3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Carpet3"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chair01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair01"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chair02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chair03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chair04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chair04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chair04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chess"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chess.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chess"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Chest"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Chest.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Chest"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ChimneyPart"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ChimneyPart.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ChimneyPart"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cigar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cigar.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cigar"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cupboard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Cupboard.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cupboard"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cupboard2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cupboard2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cupboard3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cupboard3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cupboard5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Cupboard5.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cupboard5"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Door"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Door.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Door"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Drawer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Drawer.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Drawer"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Drawer2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Drawer2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Drawer2"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("GlassJar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/GlassJar.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("GlassJar"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("houseFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/houseFloor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("houseFloor"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HouseWindow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/HouseWindow.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("HouseWindow"));




	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("InteriorFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/InteriorFloor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("InteriorFloor"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("InteriorWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/InteriorWall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("InteriorWall"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("InteriorWallDoor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/InteriorWallDoor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("InteriorWallDoor"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Inbench"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Inbench.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Inbench"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("InLight"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/InLight.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("InLight"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("napkinRed"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/napkinRed.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("napkinRed"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Painting"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Painting.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Painting"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picture"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Picture.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Picture"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potted"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Potted.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Potted"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potted4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Potted4.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Potted4"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pillow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Pillow.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Pillow"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pillow03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Pillow03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Pillow03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PillowBlue"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/PillowBlue.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("PillowBlue"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("PillowRed"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/PillowRed.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("PillowRed"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Potion.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Potion"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rug"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Rug.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rug"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shelf02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Shelf02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Shelf02"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shelf03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Shelf03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Shelf03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShelfLarge"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ShelfLarge.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ShelfLarge"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ShopCounter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/ShopCounter.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ShopCounter"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Skull"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Skull.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Skull"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sofa03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Sofa03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Sofa03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table01.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table01"));
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table02"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table05.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table05"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table06"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table06.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table06"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Table07Round"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Table07Round.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Table07Round"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Vase"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Vase.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Vase"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Wardrobe"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Interior/Wardrobe.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Wardrobe"));


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolKitchenModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CookingTable"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/CookingTable.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CookingTable"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bottle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bottle"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bottle_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle_mdl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bottle_mdl"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bottle_mdl2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle_mdl2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bottle_mdl2"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bottle3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bottle3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bottle3"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_Challah"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Challah.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_Challah"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_Croissant"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Croissant.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_Croissant"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_French"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_French.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_French"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_French_Slice"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_French_Slice.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_French_Slice"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_Loaf_Sourdough"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Loaf_Sourdough.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_Loaf_Sourdough"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_Muffin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Muffin.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_Muffin"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bread_Pizza"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Bread_Pizza.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bread_Pizza"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cabbage_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Cabbage_02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cabbage_02"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Carrot"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Carrot.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Carrot"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Carrot_Set"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Carrot_Set.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Carrot_Set"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cauldron"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Cauldron.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Cauldron"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Food_Broccoli"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Broccoli.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Food_Broccoli"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Food_Potato"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Food_Potato.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Food_Potato"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Apple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Apple"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Apple_Green"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Green.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Apple_Green"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Apple_Quarter"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Quarter.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Apple_Quarter"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Apple_Red"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Apple_Red.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Apple_Red"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Avocado"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Avocado.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Avocado"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Banana"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Banana.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Banana"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Mandarin"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Mandarin.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Mandarin"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Mango"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Mango.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Mango"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fruit_Pineapple"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fruit_Pineapple.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fruit_Pineapple"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Fryigpan"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Fryigpan.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Fryigpan"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gourd_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Gourd_mdl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gourd_mdl"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Jar_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Jar_mdl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Jar_mdl"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pitcher_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Pitcher_mdl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Pitcher_mdl"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plate_mdl"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Plate_mdl.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Plate_mdl"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Potbig"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Potbig.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Potbig"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("RedOnion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/RedOnion.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("RedOnion"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sausage"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Sausage.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Sausage"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("ServingPlate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/ServingPlate.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("ServingPlate"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Steak"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/Steak.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Steak"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("tray"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Food_Kichen/tray.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("tray"));




	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolWorkRoomModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Banner"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Banner.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Banner"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Anvil"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Anvil.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Anvil"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Broom"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Broom.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Broom"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Furnace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Furnace.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Furnace"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FurnaceB"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/FurnaceB.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("FurnaceB"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hammer"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Hammer.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Hammer"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Nail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Nail.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Nail"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pitchfork"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Pitchfork.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Pitchfork"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rake"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Rake.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rake"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shovel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Shovel.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Shovel"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tannery"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Tannery.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Tannery"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Tong"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Tong.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Tong"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WorkshopFloor"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/WorkshopFloor.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WorkshopFloor"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WorkshopWall"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/WorkshopWall.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WorkshopWall"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("WorkshopWall_door"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/WorkshopWall_door.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("WorkshopWall_door"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bow"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Bow.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Bow"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("BreastPlate"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/BreastPlate.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("BreastPlate"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("DisPlayStand"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/DisPlayStand.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("DisPlayStand"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Floor_Rubble"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Floor_Rubble.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Floor_Rubble"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Gun"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Gun.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Gun"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Halberd"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Halberd.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Halberd"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Helmet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Helmet.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Helmet"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Mace"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Helmet.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Mace"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Railing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Railing.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Railing"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Road"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Road.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Road"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Scabbard"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Scabbard.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Scabbard"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Shield"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Shield.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Shield"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spear"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Spear.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Spear"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sword"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/WorkToolRoom/Sword.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Sword"));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForMaptoolPlant()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Grass/Grass0.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Grass0"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Grass/Grass2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Grass2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Grass03.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Grass03"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Grass04.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Grass04"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Grass10"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Grass/Grass10.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Grass10"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ground_Plant02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Saternas/Ground_Plant02.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ground_Plant02"));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_ForActor()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CImgui_Manager* pImgui = GET_INSTANCE(CImgui_Manager);


#pragma region Animaition
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Alphen/Alphen.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Alphen");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sion"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Sion/Sion.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Sion");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Rinwell/Rinwell.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Rinwell");



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Law"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Law/Law.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Law");


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Ice_Wolf/Ice_Wolf.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Ice_Wolf");


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Hawk/Hawk.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Hawk");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Berserker/Berserker.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Berserker");

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Astral_Doubt"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../../../Bin/Bin_Data/Anim/Astral_Doubt/Astral_Doubt.dat"))))
		return E_FAIL;
	pImgui->m_AnimObj.push_back("Astral_Doubt");
#pragma endregion Animaition


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Alphen_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Alphen/Alphen.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Alphen_NonAnim"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rinwell_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Rinwell/Rinwell.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rinwell_NonAnim"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NpcFemaleYoung"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Npc/NpcFemaleYoung.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NpcFemaleYoung"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_GLD"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Npc/NPC_NMM_GLD.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_GLD"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMO_DOK"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Npc/NPC_NMO_DOK.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMO_DOK"));
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMY_PLC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMY_Plc/NPC_NMY_PLC.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMY_PLC"));



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NFC_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NFC_SLV_000/NPC_NFC_SLV_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NFC_SLV_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NFC_SLV_000_2th"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NFC_SLV_000_2th/NPC_NFC_SLV_000_2th.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NFC_SLV_000_2th"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NFM_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NFM_SLV_000/NPC_NFM_SLV_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NFM_SLV_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NFY_FIA_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NFY_FIA_000/NPC_NFY_FIA_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NFY_FIA_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_BEF_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMM_BEF_000/NPC_NMM_BEF_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_BEF_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_BLS_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMM_BLS_000/NPC_NMM_BLS_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_BLS_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_DIM_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMM_DIM_000/NPC_NMM_DIM_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_DIM_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_MHB_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMM_MHB_000/NPC_NMM_MHB_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_MHB_000"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMM_SLV_000"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMM_SLV_000/NPC_NMM_SLV_000.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMM_SLV_000"));


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NMY_GNL"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NMY_GNL/NPC_NMY_GNL.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NMY_GNL"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NPC_NFY_WAC"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/NPC_NFY_WAC/NPC_NFY_WAC.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NPC_NFY_WAC"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Dog"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Dog/Dog.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Dog"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Duck"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Duck/Duck.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Duck"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf_NonAnim"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/MonsterforMaptool/Ice_Wolf.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ice_Wolf_NonAnim"));


	RELEASE_INSTANCE(CImgui_Manager);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForEffect()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FloorCircle"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/FloorCircle.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossPlane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossPlane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossBuffer6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossBuffer6.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrossFlash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/CrossPlane/CrossFlash.dat"))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Pipe_is_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Pipe_is_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_hn_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_hn_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_nm_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_nm_03.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_Ak1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Ak1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_Ak2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Ak2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_hn1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_hn1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("RingVertical"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/RingVertical.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ring_Warp"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ring/Ring_Warp.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cone01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cone/Cone01.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_ko_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_ko_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderRt1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderRt1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderSO2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderSO2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_SO2_2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_SO2_2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_sw_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_sw_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_nm_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_nm_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderSphere_nm_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderSphere_nm_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CylinderWind"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/CylinderWind.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_hn02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_hn02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Cylinder_rt02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Cylinder/Cylinder_rt02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hurricane4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/Hurricane4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("waterhorn"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("waterhorn1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("waterhorn2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/waterhorn2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FountainWater"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Water/FountainWater.dat"))))
		return E_FAIL;

	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Akizame"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Akizame/Akizame.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Sphere0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Sphere/Sphere0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HalfSphere_ak_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Sphere/HalfSphere_ak_04.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_hn01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_hn02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_hn03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_hn04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_hn04.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_rm_07"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_rm_07.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_is01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_is01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_is05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_is05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_ny05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_ny05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_tk_05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_tk_05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Spiral_su_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Spiral/Spiral_su_02.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SionStrikLaserCrossPlane"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCrossPlane.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SionStrikLaserCylinder"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SionStrikLaserCylinder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SionStrikLaserCylinder3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/SionStrike/SionStrikLaserCylinder3.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plane_aj01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_aj01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plane_ak20"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_ak20.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("SparkLight01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/SparkLight01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Plane_Flash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Plane/Plane_Flash.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Bullet"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Bullet/Bullet.dat"))))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice0"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice0.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/Ice2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Auratrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Auratrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Firetrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Firetrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Trail1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Trail/Trail1.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Rock/Rock1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Rock/Rock2.dat"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder1"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder1.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder2.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder3.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder4.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder5"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder5.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder6"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder6.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder7"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder7.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder8"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder8.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Thunder9"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Thunder/Thunder9.dat"))))
		return E_FAIL;
	


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceGround"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceGround.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceHalfSphere"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceHalfSphere.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceQuardRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceQuardRing.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceRing"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceRing.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceTrail"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceTrail.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("HolyLanceTube"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/HolyLanceTube.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Splash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/HolyLance/Splash.dat"))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Crystal_BreakPts_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Crystal_BreakPts_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Crystal_BreakPts_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/Crystal_BreakPts_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrystalPts_rt_01"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_01.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrystalPts_rt_02"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_02.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrystalPts_rt_03"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_03.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrystalPts_rt_04"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_04.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CrystalPts_rt_05"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Crystal/CrystalPts_rt_05.dat"))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("FreezeLance"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Ice/FreezeLance.dat"))))
		return E_FAIL;
	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLoader::Loading_ForEffectTexture()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* Effect Textures */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLance_Mask_04_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLance_Mask_04_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLance_Trail01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLance_Trail01_M.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("HolyLanceGradient02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/HolyLance/HolyLanceGradient02.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Distortion_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distortion_Noise.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Particle_Horizontal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle_Horizontal.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Particle_Vertical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle_Vertical.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Default_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Default_Mask.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_00.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Spark_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Spark_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flash_00"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Flash_00.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flash_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Flash_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Aura_ak_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Aura_ak_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Noise_02_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Noise_02_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Noize_03_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Noize_03_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_02_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_02_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_03_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_03_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_04_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_04_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_05_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_05_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_06_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_06_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_07_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_07_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_08_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_08_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Pattern_09_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_Pattern_09_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_WaveNoise_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Common_WaveNoise_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Nois_hn_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Nois_hn_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_07_SO_AlphaC"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_07_SO_AlphaC.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_10.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_256_ny_11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_256_ny_11.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_06.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_07"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_07.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_12"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_12.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_13"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_13.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_14"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_14.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_15"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_15.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_16"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_16.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_17"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_17.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_512_ny_18"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_512_ny_18.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_ny_AlphaC09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_ny_AlphaC09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Noise_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_Noise_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC01_t2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC01_t2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC02_t2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC02_t2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC09"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC09.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_SO_AlphaC10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_SO_AlphaC10.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_noise_t2_10"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Aura/TO14_T_FX_noise_t2_10.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Cloud_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Cloud_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_SandMist_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_SandMist_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Snow_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_Snow_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_Snow_01_M_copy"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/TO14_T_FX_Common_Snow_01_M_copy.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MagumaTrail_tk_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/MagumaTrail_tk_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("copy_Foam2_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/copy_Foam2_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Warter_Splash_N_ny01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Warter_Splash_N_ny01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Warter_Splash_ny01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Warter_Splash_ny01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Water_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Water_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Wave_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Water/Wave_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FlareHorizon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/FlareHorizon.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FlareHorizon_Vertical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/FlareHorizon_Vertical.png"), 1))))
		return E_FAIL;
	

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/Line_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Line_hn_11"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/Line_hn_11.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineScroll_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/LineScroll_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("LineUnique_ym_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/LineUnique_ym_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SpellText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/SpellText.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SpellText_rm"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/SpellText_rm.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_trail_ny_08"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Line/TO14_T_FX_trail_ny_08.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("ShockWave4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/ShockWave4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Flare01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Shock/Flare01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation0"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation0.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation1.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("RockFormation2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/RockFormation2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RadialGradient_01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RadialGradient_01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RadialGradient_inv_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RadialGradient_inv_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Common_RainbowFlare01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/TO14_T_FX_Common_RainbowFlare01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_rm_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_rm_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DotNoise_tk_06"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/DotNoise_tk_06.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Noise_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/Noise_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Voronoi_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/DotNoise/Voronoi_Noise.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gradient"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Gradient.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_circle_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_circle_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Gradation_ak_05"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Gradation_ak_05.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Gradation_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Gradation_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_RadiusQuarter_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_RadiusQuarter_tk_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Ring_hn_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Ring_hn_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("TO14_T_FX_Round_SO_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/TO14_T_FX_Round_SO_04.png"), 1))))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CoreAura_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/CoreAura_rm_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CoreAura_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/CoreAura_rm_02.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DropMask_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/DropMask_tk_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("SlashMask_tk_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/SlashMask_tk_01.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smash_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/Smash_rm_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smash_rm_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Unique/Smash_rm_02.png"), 1))))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("circle_ny_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/circle_ny_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Gradient_rm_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Gradient_rm_01.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_03.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_04"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_04.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Round_SO_04_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Ring/Round_SO_04_2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Explode_Fractal01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/Explode_Fractal01_M.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Explode_Smoke01_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/Explode_Smoke01_M.png"), 1))))
		return E_FAIL; 
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("FireNoise_is_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Explode/FireNoise_is_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("DustParticle_M"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Particle/DustParticle_M.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smoke_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/Smoke_Mask.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Smoke_Mask1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Smoke/Smoke_Mask1.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Puddle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Mask/Puddle.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("CrackPattern"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/CrackPattern.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("MagmaCracks"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/MagmaCracks.dds"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_01"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_01.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_02"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_02.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Crack_03"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Crack/Crack_03.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation5.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Height_Gradation6"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Gradation/Height_Gradation6.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("floormask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Mask/floormask.png"), 1))))
		return E_FAIL;
	
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_1.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_2.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_3.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_4.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_a"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_a.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_b"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_b.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Thunder_c"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Thunder/Thunder_c.png"), 1))))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevel)
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