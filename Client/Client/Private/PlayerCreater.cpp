#include "stdafx.h"
#include "PlayerCreater.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "SnowFieldNpc.h"
#include "CItyNpc.h"
#include "BattleManager.h"
#include "TreasureBox.h"
#include "Item.h"
#include "Monster.h"
#include "Object_Pool_Manager.h"
#include "Npc.h"
#include "Portal.h"
#include "MiniGameNpc.h"
#include "Trigger.h"
#include "Effect.h"


CPlayerCreater::CPlayerCreater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
} 

unsigned int APIENTRY Thread_Clone(void* pArg)
{
	CPlayerCreater* pLoader = (CPlayerCreater*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_CreateActorType())
	{
	case CLONE_PLAYER:
		pLoader->Cloning_ForPlayer();
		break;
	case CLONE_MONSTER:
		pLoader->Cloning_ForMonster();
		break;
	case CLONE_NPC:
		pLoader->Cloning_ForNpc();
		break;
	case CLONE_NONANIM:
		pLoader->Cloning_ForNpc();
		pLoader->Cloning_ForNonAnim();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CPlayerCreater::Initialize(THREAD_ID eThreadID)
{
	CoInitializeEx(nullptr, 0);

	m_eCloneType = eThreadID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Clone, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
		
	return S_OK;
}


HRESULT CPlayerCreater::Cloning_ForPlayer()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Alphen"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(44, 0, 22, 1.f));
		//pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(150, 0, 150, 1.f));
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
		CPlayerManager::Get_Instance()->Save_LastPosition();
		pPlayer->Change_Level(LEVEL_SNOWFIELD);
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Player"));

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sion"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;
		////CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Player"));

		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
		//	return E_FAIL;
		//
		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Law"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
		//	return E_FAIL;

		//CGameObject* pObject = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"), 2);
		//CGameObject* pObject2 = CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"), 3);
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Player"), pObject);
		//CObject_Pool_Manager::Get_Instance()->Add_Pooling_Object(LEVEL_STATIC, TEXT("Layer_Player"), pObject2);
	}

	cout << "Player Clone Finished" << endl;
	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;
	return S_OK;
}


HRESULT CPlayerCreater::Cloning_ForNpc()
{
	if (FAILED(Ready_Layer_NpcSnowField(TEXT("Layer_Npc"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NpcCity(TEXT("Layer_Npc"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NpcMIniGame(TEXT("Layer_Npc"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_NpcLawBattle(TEXT("Layer_Npc"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerCreater::Cloning_ForMonster()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CTrigger::TRIGGERDESC TriggerDesc;
	NONANIMDESC ModelDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Triggerbox.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(TriggerDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		TriggerDesc.eType = CTrigger::MONSTER_TRIGGER;
		TriggerDesc.iIndex = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Trigger"), LEVEL_SNOWFIELD, TEXT("Layer_Trigger"), &TriggerDesc)))
			return E_FAIL;
		
		
	}

	CloseHandle(hFile);
	Loading_MonsterAtFirst();
	
	cout << "Monster Clone Finished" << endl;
	

	RELEASE_INSTANCE(CGameInstance);
	m_isFinished = true;
	return S_OK;
}

HRESULT CPlayerCreater::Cloning_ForNonAnim()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	if (FAILED(Ready_InstancingForPooling(TEXT("Layer_Instancing"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Deco_SnowField(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_SnowDecoObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BossMapObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_CityMapObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_RestaurantObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_WorkToolMapObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_LawBattleMapObject(TEXT("Layer_Deco"))))
		return E_FAIL;


	if (FAILED(Ready_Layer_Interact_SnowField(TEXT("Layer_Interact"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Interact_Restaurant(TEXT("Layer_Interact"))))
		return E_FAIL;
	if (FAILED(Ready_Layer_Interact_WorkTool(TEXT("Layer_Interact"))))
		return E_FAIL;

	cout << " NonAnim Clone Finished" << endl;
	m_isFinished = true;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayerCreater::Loading_MonsterAtFirst()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc;
	_uint iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Monster.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; i++)
	{
		_bool m_bNotCreate = false;
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("Ice_Wolf")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ice_Wolf"), LEVEL_STATIC, TEXT("Layer_Monster"), &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Hawk")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hawk"), LEVEL_STATIC, TEXT("Layer_Monster"), &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Berserker")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Berserker"), LEVEL_STATIC, TEXT("Layer_Monster"), &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Rinwell")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AiRinwell"), LEVEL_STATIC, TEXT("Layer_Monster"), &ModelDesc)))
				return E_FAIL;
		}
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Monster"));


	}

	CloseHandle(hFile);


	 hFile = 0;
	 dwByte = 0;
	 iNum = 0;
	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/BossPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		strcpy(ModelDesc.pModeltag, "Astral_Doubt");
		ModelDesc.vScale = _float3(2.f, 2.f, 2.f);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AstralDoubt"), LEVEL_STATIC, TEXT("Layer_Boss"), &ModelDesc)))
			return E_FAIL;

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Boss"));

		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		strcpy(ModelDesc.pModeltag, "Astral_Doubt");
		ModelDesc.vScale = _float3(2.f, 2.f, 2.f);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AstralDoubt"), LEVEL_STATIC, TEXT("Layer_SecondBoss"), &ModelDesc)))
			return E_FAIL;

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_SecondBoss"));
	

	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CPlayerCreater::Ready_InstancingForPooling(const _tchar* pLayerTag)
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
	strcpy(stModelDesc.pModeltag, "CliffRock1_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));

	strcpy(stModelDesc.pModeltag, "CliffRock2_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
	strcpy(stModelDesc.pModeltag, "CliffRock3_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
	strcpy(stModelDesc.pModeltag, "CliffRock4_SnowField");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_SNOWFIELD, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));


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
	strcpy(stModelDesc.pModeltag, "Bld_TilingA");
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
	strcpy(stModelDesc.pModeltag, "CIty_Rock1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_CITY, pLayerTag, &stModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));

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


HRESULT CPlayerCreater::Ready_Layer_Deco_SnowField(const _tchar * pLayerTag)
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

HRESULT CPlayerCreater::Ready_Layer_SnowDecoObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	//hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/CliffRock2.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if (0 == hFile)
	//	return E_FAIL;

	///* 타일의 개수 받아오기 */
	//ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	//for (_uint i = 0; i < iNum; ++i)
	//{
	//	ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
	//		return E_FAIL;
	//	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));

	//}

	//CloseHandle(hFile);

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

HRESULT CPlayerCreater::Ready_Layer_RestaurantObject(const _tchar * pLayerTag)
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

HRESULT CPlayerCreater::Ready_Layer_BossMapObject(const _tchar * pLayerTag)
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
	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_CityMapObject(const _tchar * pLayerTag)
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

	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_LAWBATTLE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));

	ReadFile(hFile, &(PortalDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
	PortalDesc.iNextLevel = LEVEL_BOSS;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_CITY, TEXT("Layer_Portal"), &PortalDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Portal"));

	CloseHandle(hFile);


	hFile = 0;
	dwByte = 0;
	iNum = 0;
	CTrigger::TRIGGERDESC TriggerDesc;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Trigger.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(TriggerDesc.m_ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		TriggerDesc.eType = CTrigger::UI_TRIGGER;
		TriggerDesc.iIndex = i;
		TriggerDesc.m_ModelDesc.vScale = _float3(15.f, 15.f, 15.f);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Trigger"), LEVEL_CITY, TEXT("Layer_Trigger"), &TriggerDesc)))
			return E_FAIL;

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Trigger"));

	}

	CloseHandle(hFile);
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_WorkToolMapObject(const _tchar * pLayerTag)
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

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Road"), LEVEL_WORKTOOL, pLayerTag, &ModelDesc)))
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

HRESULT CPlayerCreater::Ready_Layer_LawBattleMapObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/LawBattle/Floor.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_LAWBATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);

	hFile = 0;
	dwByte = 0;
	iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/LawBattle/Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_LAWBATTLE, pLayerTag, &ModelDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Deco"));

	}

	CloseHandle(hFile);





	ModelDesc;
	strcpy(ModelDesc.pModeltag, "AILaw");
	ModelDesc.vPosition = _float3(64, 0.f, 64);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MonsterLaw"), LEVEL_LAWBATTLE, TEXT("Layer_Boss"), &ModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Boss"));

	ModelDesc;
	strcpy(ModelDesc.pModeltag, "AIRinwell");
	ModelDesc.vPosition = _float3(64, 0.f, 64);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AiRinwell"), LEVEL_LAWBATTLE, TEXT("Layer_Rinwell"), &ModelDesc)))
		return E_FAIL;
	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell"));

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_Interact_SnowField(const _tchar * pLayerTag)
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
		else if (!wcscmp(pModeltag, TEXT("SlimPlant")))
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

HRESULT CPlayerCreater::Ready_Layer_Interact_Restaurant(const _tchar * pLayerTag)
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
		if (!wcscmp(pModeltag, TEXT("Bread_Challah")) ||
			!wcscmp(pModeltag, TEXT("Bread_Croissant")) || 
			!wcscmp(pModeltag, TEXT("Bread_French")) || 
			!wcscmp(pModeltag, TEXT("Bread_French_Slice")) ||
			!wcscmp(pModeltag, TEXT("Bread_Loaf_Sourdough")) || 
			!wcscmp(pModeltag, TEXT("Bread_Muffin"))  )
		{
			ItemDesc.etype = CItem::BREAD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Bread_Pizza")))
		{
			ItemDesc.etype = CItem::PIZZA;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_RESTAURANT, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Steak")))
		{
			ItemDesc.etype = CItem::STEAK;
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

HRESULT CPlayerCreater::Ready_Layer_Interact_WorkTool(const _tchar * pLayerTag)
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
		if (!wcscmp(pModeltag, TEXT("Cigar")))
		{
			ItemDesc.etype = CItem::TABACO;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_WORKTOOL, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Chess")))
		{
			ItemDesc.etype = CItem::CHESS;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_WORKTOOL, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Interact"));

	}
	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_NpcSnowField(const _tchar * pLayerTag)
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

HRESULT CPlayerCreater::Ready_Layer_NpcCity(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNpc::NPCDESC NpcDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, NpcDesc.Modeldesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("NPC_NFC_SLV_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NFC_SLV_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_Child"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Child"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_GLD")))
		{
			NpcDesc.eNpcType = CCityNpc::MAN_GLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_Gld"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Gld"));

		}

		//else if (!wcscmp(pModeltag, TEXT("NPC_NMM_GLD")))
		//{
		//	NpcDesc.eNpcType = CCityNpc::MAN_GLD;
		//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag, &NpcDesc)))
		//		return E_FAIL;
		//	CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));

		//}
		else if (!wcscmp(pModeltag, TEXT("NPC_NFC_SLV_000_2th")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NFC_SLV_000_2th;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_Child"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Child"));


		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMY_PLC")))
		{
			NpcDesc.eNpcType = CCityNpc::MAN_PLC;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_Plc"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Plc"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NFM_SLV_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NFM_SLV_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_NFM"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NFM"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NFY_FIA_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NFY_FIA_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_FIA"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_FIA"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_BEF_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMM_BEF_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, pLayerTag);

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_BLS_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMM_BLS_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, pLayerTag);

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_DIM_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMM_DIM_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_DIM"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_DIM"));

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_MHB_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMM_MHB_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag , &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, pLayerTag);

		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_SLV_000")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMM_SLV_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_NMM_SLV"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NMM_SLV"));

		}

		else if (!wcscmp(pModeltag, TEXT("NPC_NMY_GNL")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NMY_GNL;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_GNL"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_GNL"));

		}

		else if (!wcscmp(pModeltag, TEXT("NPC_NFY_WAC")))
		{
			NpcDesc.eNpcType = CCityNpc::NPC_NFY_WAC;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, pLayerTag);

		}

		else if (!wcscmp(pModeltag, TEXT("Dog")))
		{
			NpcDesc.eNpcType = CCityNpc::DOG;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_NMM_SLV"));

		}

		else if (!wcscmp(pModeltag, TEXT("Duck")))
		{
			NpcDesc.eNpcType = CCityNpc::DUCK;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CityNpc"), LEVEL_CITY, TEXT("Layer_Duck"), &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Duck"));

		}

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_NpcMIniGame(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNpc::NPCDESC NpcDesc;

	_uint iNum = 0;
	
	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, NpcDesc.Modeldesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("NPC_NMM_GLD")))
		{
			NpcDesc.eNpcType = CMiniGameNpc::MAN_GLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MiniGameNpc"), LEVEL_WORKTOOL, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, pLayerTag);

		}
		if (!wcscmp(pModeltag, TEXT("NPC_NMM_BEF_000")))
		{
			NpcDesc.eNpcType = CMiniGameNpc::NPC_NMM_BEF_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MiniGameNpc"), LEVEL_WORKTOOL, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, pLayerTag);

		}
	}

	CloseHandle(hFile);

	 hFile = 0;
	 dwByte = 0;
	 iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, NpcDesc.Modeldesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("NPC_NMM_BLS_000")))
		{
			NpcDesc.eNpcType = CMiniGameNpc::NPC_NMM_BLS_000;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MiniGameNpc"), LEVEL_RESTAURANT, pLayerTag, &NpcDesc)))
				return E_FAIL;
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, pLayerTag);

		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayerCreater::Ready_Layer_NpcLawBattle(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNpc::NPCDESC NpcDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/LawBattle/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		strcpy(NpcDesc.Modeldesc.pModeltag, "NpcLaw");
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BattleNpc"), LEVEL_LAWBATTLE, TEXT("Layer_Npc"), &NpcDesc)))
			return E_FAIL;
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Npc"));
	}

	CloseHandle(hFile);



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;

}

CPlayerCreater * CPlayerCreater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, THREAD_ID eNextLevel)
{
	CPlayerCreater* pInstance = new CPlayerCreater(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerCreater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCreater::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}