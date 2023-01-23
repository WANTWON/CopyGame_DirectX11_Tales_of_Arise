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
#include "Effect_Manager.h"
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/ForImguiDebug/Texture%d.dds"), 7))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Brush */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Terrain/Brush2.png"), 1))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Picking_Symbol"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../../../Bin/Bin_Data/NonAnim/Picking_Symbol/Picking_Symbol.dat"))))
		return E_FAIL;

	/*For.Prototype_Component_Texture_Test */
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Test"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../../../Bin/Resources/Textures/Effect/TO14_T_FX_Common_Kirakira%02d.png"), 3))))
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

	/*For.Prototype_GameObject_Effect*/
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Effect"),
		CEffect::Create(m_pDevice, m_pContext))))
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