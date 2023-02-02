#include "stdafx.h"
#include "Loader.h"

#include "GameInstance.h"

#include "Camera_Dynamic.h"
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/SnowField/Terrain%d_D.dds"), 4))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush2.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picking_Symbol"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Picking_Symbol/Picking_Symbol.dat"))))
		return E_FAIL;

	/*_matrix			PivotMatrix = XMMatrixIdentity();
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picking_Symbol"), CModel::Create(m_pDevice, m_pContext,
		CModel::TYPE_NONANIM, "../Bin/Resources/Picking_Symbol/Picking_Symbol.fbx", PivotMatrix))))
		return E_FAIL;*/

	/* Effect Textures */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Default_Particle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Default_Particle.png"), 1))))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Distortion_Strength"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/Distortion_Strength.png"), 1))))
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

	if (FAILED(Loading_ForGamePlayModel()))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("Finished"));

	RELEASE_INSTANCE(CModelManager);
	RELEASE_INSTANCE(CGameInstance);

	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_ForGamePlayModel()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Alphen"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Alphen/Alphen.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Alphen"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("NpcFemaleYoung"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Npc/NpcFemaleYoung.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("NpcFemaleYoung"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rinwell"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Rinwell/Rinwell.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rinwell"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Ice_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/MonsterforMaptool/Ice_Wolf.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Ice_Wolf"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Hawk"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/MonsterforMaptool/Hawk.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Hawk"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Berserker"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/MonsterforMaptool/Berserker.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Berserker"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Rock"), 
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Rock/Rock.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Rock.dat"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Slash"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/Effect/Slash/Slash.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("Slash.dat"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock2"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock2.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock2"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock3"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock3.dat"))))
		return E_FAIL;
	CModelManager::Get_Instance()->Add_PrototypeTag(TEXT("CliffRock3"));

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("CliffRock4"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/CliffRock/Desert_CliffRock4.dat"))))
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