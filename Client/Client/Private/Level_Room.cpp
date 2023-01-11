#include "stdafx.h"
#include "..\Public\Level_Room.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "UI_Manager.h"
#include "UIScreen.h"
#include "NonAnim.h"
#include "Player.h"
#include "Level_Loading.h"
#include "CameraManager.h"

#include "Grass.h"
#include "Portal.h"
#include "PrizeItem.h"
#include "Npc.h"
#include "SquareBlock.h"
#include "CollapseTile.h"

CLevel_Room::CLevel_Room(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_Room::Initialize()
{
	CGameInstance::Get_Instance()->Clear_Layer(LEVEL_GAMEPLAY, TEXT("UI_ICON"));
	CUI_Manager::Get_Instance()->Clear_WarpGroup();

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;



	CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
	CGameInstance::Get_Instance()->StopAll();

	switch (eRoomType)
	{
	case Client::CUI_Manager::MARINHOUSE:
		if (FAILED(Ready_Layer_MarinObject(TEXT("Layer_Object"))))
			return E_FAIL;
		CGameInstance::Get_Instance()->PlayBGM(TEXT("1-17 Inside a House.mp3"), g_fBGMVolume);
		break;
	case Client::CUI_Manager::SHOP:
		if (FAILED(Ready_Layer_ShopObject(TEXT("Layer_Object"))))
			return E_FAIL;
		CGameInstance::Get_Instance()->PlayBGM(TEXT("1-12 Shop (Items, Witch).mp3"), g_fBGMVolume);
		break;
	case Client::CUI_Manager::CRANEGAME:
		if (FAILED(Ready_Layer_CraneGameObject(TEXT("Layer_Object"))))
			return E_FAIL;
		CGameInstance::Get_Instance()->PlayBGM(TEXT("1-14 Game Shop.mp3"), g_fBGMVolume);
		break;
	case Client::CUI_Manager::TELEPHONE:
		if (FAILED(Ready_Layer_TelephoneObject(TEXT("Layer_Object"))))
			return E_FAIL;
		CGameInstance::Get_Instance()->PlayBGM(TEXT("1-29 Telephone Booth.mp3"), g_fBGMVolume);
		break;
	default:
		break;
	}
	

	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();

	pCameraManager->Ready_Camera(LEVEL::LEVEL_ROOM);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_ROOM);

	g_fBGMVolume = 0.f;
	
	CUI_Manager::Get_Instance()->Set_NextLevel(false);
	return S_OK;
}

void CLevel_Room::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	g_fBGMVolume += 0.001f;
	if (g_fBGMVolume >= 0.2f)
		g_fBGMVolume = 0.2f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fBGMVolume);

	CUI_Manager::Get_Instance()->Tick_UI(fTimeDelta);

	if (CUI_Manager::Get_Instance()->Get_NextLevelFinished())
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		LEVEL eNextLevel = CUI_Manager::Get_Instance()->Get_NextLevelIndex();

		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_MONSTER);
		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_BLOCK);
		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_INTERACT);
		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_TRAP);
		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_MBULLET);
		m_pCollision_Manager->Clear_CollisionGroup(CCollision_Manager::COLLISION_ITEM);
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;
		Safe_Release(pGameInstance);
	}

}

void CLevel_Room::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	SetWindowText(g_hWnd, TEXT("Room Level."));

	m_pCollision_Manager->Update_Collider();
	m_pCollision_Manager->CollisionwithBullet();

	if (CGameInstance::Get_Instance()->Key_Up(DIK_6))
	{
		CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);

		pUI_Manager->Set_NextLevelIndex(LEVEL_GAMEPLAY);
		pUI_Manager->Set_NextLevel(true);

		RELEASE_INSTANCE(CUI_Manager);
	}
}

HRESULT CLevel_Room::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(0.f, -1.f, 0.0f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 0.7f);	

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(0, 20, 10, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(0, 0, 0, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

	///* For.Point */
	//ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	//LightDesc.eType = LIGHTDESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(0.f, 1.f, 0.f, 1.f);
	//LightDesc.fRange = 2.f;	
	//LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	//LightDesc.vAmbient = _float4(0.0f, 0.0f, 0.0f, 1.f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	//if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
	//	return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;

		CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		LEVEL ePastLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_PastLevelIndex();
		pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.0f, -3.5f, 1.f));
	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		LEVEL ePastLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_PastLevelIndex();

		//CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
		//if (eRoomType == CUI_Manager::CRANEGAME)
		//	pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(-1.f, 0.0f, -0.5f, 1.f));
		//else
			pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 0.0f, -3.5f, 1.f));
		pPlayer->Change_Navigation(LEVEL_ROOM);
	
		pPlayer->Set_JumpingHeight(0.1f);
		pPlayer->Compute_CurrentIndex(LEVEL_ROOM);
	}

	
	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	LEVEL eLevel = LEVEL_ROOM;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_ROOM, TEXT("Layer_Terrain"), &eLevel)))
		return E_FAIL;


	CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
	CNonAnim::NONANIMDESC  ModelDesc;
	ModelDesc.vPosition = _float3(0.f, 0.f, 0.f);
	ModelDesc.vScale = _float3(3.f, 3.f, 3.f);
	

	switch (eRoomType)
	{
	case Client::CUI_Manager::MARINHOUSE:
		strcpy(ModelDesc.pModeltag, "MarinHouse.fbx");
		break;
	case Client::CUI_Manager::SHOP:
		strcpy(ModelDesc.pModeltag, "Shop.fbx");
		break;
	case Client::CUI_Manager::CRANEGAME:
		ModelDesc.vPosition = _float3(1.82f, 0.f, -1.86f);
		strcpy(ModelDesc.pModeltag, "CraneGame.fbx");
		break;
	case Client::CUI_Manager::TELEPHONE:
		strcpy(ModelDesc.pModeltag, "TelephoneBox.fbx");
		break;
	default:
		break;
	}

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_ROOM, pLayerTag, &ModelDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLevel_Room::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.iTest = 10;
	
	CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
	if (eRoomType == CUI_Manager::CRANEGAME)
	{
		CameraDesc.InitPostion = _float4(0.f, 11.8f, -5.5f, 1.f);
		CameraDesc.CameraDesc.vEye = _float4(0.f, 11.8f, -5.5f, 1.f);
	}
	else
	{
		CameraDesc.InitPostion = _float4(0.f, 11.8f, -5.5f, 1.f);
		CameraDesc.CameraDesc.vEye = _float4(0.f, 11.8f, -5.5f, 1.f);
	}
		
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_ROOM, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_Room::Ready_Layer_ShopObject(const _tchar * pLayerTag)
{
	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Shop_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Bow.fbx")))
		{

			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::BOW;
			ItemDesc.eInteractType = CPrizeItem::CARRYABLE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("MagicRod.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::MAGIC_ROD;
			ItemDesc.eInteractType = CPrizeItem::CARRYABLE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("HeartContainer.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::HEART_CONTAINER;
			ItemDesc.eInteractType = CPrizeItem::CARRYABLE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("DogFood.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::DOGFOOD;
			ItemDesc.eInteractType = CPrizeItem::CARRYABLE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Arrow.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::ARROW;
			ItemDesc.eInteractType = CPrizeItem::CARRYABLE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("ShopNpc.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::SHOP;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ShopNpc"), LEVEL_ROOM, pLayerTag, &NpcDesc)))
				return E_FAIL;
		}

	}


	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);
	
	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_MarinObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Marin_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Tarin.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::TARIN;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TarinNpc"), LEVEL_ROOM, pLayerTag, &NpcDesc)))
				return E_FAIL;
		}
		if (!wcscmp(pModeltag, TEXT("Marin.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::MARIN;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MarinNpc"), LEVEL_ROOM, pLayerTag, &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Bed.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_ROOM, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Pot.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_ROOM, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}

	}


	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_CraneGameObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/CraneGame_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Necklace.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::NECKLACE;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("YoshiDoll.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::YOSHIDOLL;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("RubyPurple.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::RUBYPURPLE;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("HeartContainer.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::HEART_CONTAINER;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("MagicPowder.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::MAGICPOWDER;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("CraneFence.fbx")))
		{
			CSquareBlock::BLOCKDESC BlockDesc;
			BlockDesc.eType = CSquareBlock::CRANEGAME_FENCE;
			BlockDesc.vInitPosition = ModelDesc.vPosition;
			BlockDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SquareBlock"), LEVEL_ROOM, pLayerTag, &BlockDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("GameShopOwner.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::CRANE_GAME;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CraneGameNpc"), LEVEL_ROOM, pLayerTag, &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("MovingFloorCrane.fbx")))
		{
			CCollapseTile::TILEDESC TileDesc;
			TileDesc.eTileType = CCollapseTile::CRANE_TILE;
			TileDesc.vInitPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CollapeTile"), LEVEL_ROOM, pLayerTag, &TileDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("CraneGameButton.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CraneButton"), LEVEL_ROOM, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Crane.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Crane"), LEVEL_ROOM, TEXT("Layer_Crane"), &ModelDesc.vPosition)))
				return E_FAIL;
		}

	}
	
	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_TelephoneObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Telephone_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Telephone.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.eType = CPrizeItem::TELEPHONE;
			ItemDesc.eInteractType = CPrizeItem::TELL;
			ItemDesc.vPosition = ModelDesc.vPosition;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_ROOM, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}

	}


	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_Room::Ready_Layer_Portal(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CPortal::PORTALDESC PortalDesc;
	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = _float3(0.f, 0.f, -5.5f);
	PortalDesc.eConnectLevel = LEVEL_GAMEPLAY;
	PortalDesc.eRoomType = (CPortal::ROOMTYPE)CUI_Manager::Get_Instance()->Get_RoomType();

	//CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
	//if(eRoomType == CUI_Manager::CRANEGAME)
	//	PortalDesc.vInitPos = _float3(0.f, 0.f, -3.5f);


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_ROOM, pLayerTag, &PortalDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_Room * CLevel_Room::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Room*	pInstance = new CLevel_Room(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Room"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Room::Free()
{
	__super::Free();

	Safe_Release(m_pCollision_Manager);

}
