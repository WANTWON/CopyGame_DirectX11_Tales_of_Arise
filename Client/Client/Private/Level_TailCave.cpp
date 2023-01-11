#include "stdafx.h"
#include "..\Public\Level_TailCave.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "CameraManager.h"

#include "UI_Manager.h"
#include "UIButton.h"
#include "InvenTile.h"
#include "InvenItem.h"

#include "UIScreen.h"
#include "NonAnim.h"
#include "Player.h"

#include "DgnKey.h"
#include "TreasureBox.h"
#include "TailBoss.h"
#include "Door.h"
#include "Tail.h"
#include "Portal.h"
#include "SquareBlock.h"
#include "PrizeItem.h"
#include "CollapseTile.h"
#include "FireEffect.h"

CLevel_TailCave::CLevel_TailCave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_TailCave::Initialize()
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

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Object"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;
	
	CCameraManager* pCameraManager = CCameraManager::Get_Instance();

	pCameraManager->Ready_Camera(LEVEL::LEVEL_TAILCAVE);

	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_TERRAIN);
	CUI_Manager::Get_Instance()->Set_NextLevel(false);

	g_fBGMVolume = 0.f;

	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("1-20 Level 1 - Tail Cave.mp3"), g_fBGMVolume);
	return S_OK;
}

void CLevel_TailCave::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);	

	CUI_Manager::Get_Instance()->Tick_UI(fTimeDelta);

	g_fBGMVolume += 0.001f;
	if (g_fBGMVolume >= 0.2f)
		g_fBGMVolume = 0.2f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fBGMVolume);

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

void CLevel_TailCave::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	SetWindowText(g_hWnd, TEXT("TailCave Level."));

	CCollision_Manager::Get_Instance()->Update_Collider();
	CCollision_Manager::Get_Instance()->CollisionwithBullet();
	
	
}

HRESULT CLevel_TailCave::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i< iNum; ++i)
	{
		ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc);
	}
	CloseHandle(hFile);


	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(36, 50, 70, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(36, 0, 20, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
	LEVEL ePastLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_PastLevelIndex();
	pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(54.f, 0.1f, 2.8f, 1.f));
	pPlayer->Change_Navigation(LEVEL_TAILCAVE);
	//pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(23.f, 0.1f, 63.f, 1.f));
	pPlayer->Set_JumpingHeight(0.1f);
	pPlayer->Compute_CurrentIndex(LEVEL_TAILCAVE);

	Safe_Release(pGameInstance);

	
	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);



	LEVEL eLevel = LEVEL_TAILCAVE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_TAILCAVE, TEXT("Layer_Terrain"), &eLevel)))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Map.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

#pragma region Camera Dynamic
	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc_Dynamic;
	ZeroMemory(&CameraDesc_Dynamic, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc_Dynamic.iTest = 10;

	CameraDesc_Dynamic.CameraDesc.vEye = _float4(0.f, 12.f, -3.5f, 1.f);
	CameraDesc_Dynamic.CameraDesc.vAt = _float4(0.f, 0.f, 0.2f, 1.f);
	CameraDesc_Dynamic.InitPostion = _float4(54.f, 10.1f, -2.8f, 1.f);

	CameraDesc_Dynamic.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc_Dynamic.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc_Dynamic.CameraDesc.fNear = 0.2f;
	CameraDesc_Dynamic.CameraDesc.fFar = 500.f;

	CameraDesc_Dynamic.CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	CameraDesc_Dynamic.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_TAILCAVE, pLayerTag, &CameraDesc_Dynamic)))
		return E_FAIL;
#pragma endregion Camera Dynamic

#pragma region Camera 2D


	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_float3 vInitPos = _float3(0.f,0.f,0.f);
	_float3 vMaxXPos = _float3(0.f,0.f,0.f);
	_float3 vMinXPos = _float3(0.f,0.f,0.f);
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Camera2D.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);
	ReadFile(hFile, &(vInitPos), sizeof(_float3), &dwByte, nullptr);
	ReadFile(hFile, &(vMaxXPos), sizeof(_float3), &dwByte, nullptr);
	ReadFile(hFile, &(vMinXPos), sizeof(_float3), &dwByte, nullptr);

	CCamera_2D::CAMERADESC_DERIVED				CameraDesc_2D;
	ZeroMemory(&CameraDesc_2D, sizeof(CCamera_2D::CAMERADESC_DERIVED));

	XMStoreFloat4(&CameraDesc_2D.InitPostion, XMVectorSetW(XMLoadFloat3(&vInitPos), 1.f));
	CameraDesc_2D.fMaxXPos = vMaxXPos.x;
	CameraDesc_2D.fMinXPos = vMinXPos.x;

	CameraDesc_2D.CameraDesc.vEye = _float4(0.f, 11.8f, -3.5f, 1.f);
	CameraDesc_2D.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc_2D.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc_2D.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc_2D.CameraDesc.fNear = 0.2f;
	CameraDesc_2D.CameraDesc.fFar = 500.f;

	CameraDesc_2D.CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	CameraDesc_2D.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_2D"), LEVEL_TAILCAVE, pLayerTag, &CameraDesc_2D)))
		return E_FAIL;

	CloseHandle(hFile);
#pragma endregion Camera 2D

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_TailCave::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Monster.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Pawn.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Pawn"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Rola.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rola"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("BuzzBlob.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BuzzBlob"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("ZolRed.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_RedZol"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Keese.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Keese"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Togezo.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Togezo"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("TailBoss1.fbx")))
		{
			CTailBoss::TAILDESC TailDesc;
			TailDesc.eTailType = CTailBoss::TAIL1;
			TailDesc.InitPostion = XMLoadFloat3(&ModelDesc.vPosition);
			TailDesc.InitPostion = XMVectorSetW(TailDesc.InitPostion, 1.f);
			TailDesc.pParent = nullptr;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TailBoss"), LEVEL_TAILCAVE, pLayerTag, &TailDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Tail1.fbx")))
		{
			CTail::TAILDESC TailDesc;
			TailDesc.eTailType = CTail::TAIL1;
			TailDesc.InitPostion = XMLoadFloat3(&ModelDesc.vPosition);
			TailDesc.InitPostion = XMVectorSetW(TailDesc.InitPostion, 1.f);
			TailDesc.pParent = nullptr;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Tail"), LEVEL_TAILCAVE, pLayerTag, &TailDesc)))
				return E_FAIL;
		}
		

	}

	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("SmallKey.fbx")))
		{

			CDgnKey::DGNKEYDESC DgnKeyDesc;
			DgnKeyDesc.eType = CDgnKey::SMALL_KEY;
			DgnKeyDesc.vPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_DgnKey"), LEVEL_TAILCAVE, pLayerTag, &DgnKeyDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("FootSwitch.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FootSwitch"), LEVEL_TAILCAVE, TEXT("Layer_FootSwitch"), &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("CollapseTile.fbx")))
		{
			CCollapseTile::TILEDESC TileDesc;
			TileDesc.eTileType = CCollapseTile::COLLAPSE;
			TileDesc.vInitPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CollapeTile"), LEVEL_TAILCAVE, pLayerTag, &TileDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("SquareBlock.fbx")))
		{
			CSquareBlock::BLOCKDESC BlockDesc;
			BlockDesc.eType = CSquareBlock::SQUARE_BLOCK;
			BlockDesc.vInitPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SquareBlock"), LEVEL_TAILCAVE, pLayerTag, &BlockDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("LockBlock.fbx")))
		{
			CSquareBlock::BLOCKDESC BlockDesc;
			BlockDesc.eType = CSquareBlock::LOCK_BLOCK;
			BlockDesc.vInitPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SquareBlock"), LEVEL_TAILCAVE, pLayerTag, &BlockDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("BladeTrap.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BladeTrap"), LEVEL_TAILCAVE, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("ClosedDoor.fbx")))
		{
			CDoor::DOORDESC DoorDesc;
			DoorDesc.eType = CDoor::DOOR_CLOSED;
			DoorDesc.InitPosition = ModelDesc.vPosition;
			DoorDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Door"), LEVEL_TAILCAVE, TEXT("Layer_ColsedDoor"), &DoorDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("LockDoor.fbx")))
		{
			CDoor::DOORDESC DoorDesc;
			DoorDesc.eType = CDoor::DOOR_KEY;
			DoorDesc.InitPosition = ModelDesc.vPosition;
			DoorDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Door"), LEVEL_TAILCAVE, pLayerTag, &DoorDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("BossDoor.fbx")))
		{
			CDoor::DOORDESC DoorDesc;
			DoorDesc.eType = CDoor::DOOR_BOSS;
			DoorDesc.InitPosition = ModelDesc.vPosition;
			DoorDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Door"), LEVEL_TAILCAVE, pLayerTag, &DoorDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("FullMoonCello.fbx")))
		{
			CPrizeItem::ITEMDESC ItemDesc;
			ItemDesc.vPosition = ModelDesc.vPosition;
			ItemDesc.eType = CPrizeItem::CELLO;
			ItemDesc.eInteractType = CPrizeItem::PRIZE;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_PrizeItem"), LEVEL_TAILCAVE, pLayerTag, &ItemDesc)))
				return E_FAIL;
		}

	}
	
	CloseHandle(hFile);


	hFile = 0;
	dwByte = 0;
	CTreasureBox::BOXTAG  TreasureBoxDesc;
	iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_TreasureBox.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(TreasureBoxDesc), sizeof(CTreasureBox::BOXTAG), &dwByte, nullptr);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBox"), LEVEL_TAILCAVE, pLayerTag, &TreasureBoxDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_Portal(const _tchar * pLayerTag)
{
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc1;
	CNonAnim::NONANIMDESC  ModelDesc2;
	_uint iNum = 0;
	
	CPortal::PORTALDESC PortalDesc;


	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Portal.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	
		ReadFile(hFile, &(ModelDesc1), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		ReadFile(hFile, &(ModelDesc2), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		PortalDesc.vInitPos = ModelDesc1.vPosition;
		PortalDesc.vConnectPos = ModelDesc2.vPosition;
		PortalDesc.bConnectPortal2D = true;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TAILCAVE, pLayerTag, &PortalDesc)))
			return E_FAIL;

		PortalDesc.vInitPos = ModelDesc2.vPosition;
		PortalDesc.vConnectPos = ModelDesc1.vPosition;
		PortalDesc.bConnectPortal2D = false;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TAILCAVE, pLayerTag, &PortalDesc)))
			return E_FAIL;

	
		ReadFile(hFile, &(ModelDesc1), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		ReadFile(hFile, &(ModelDesc2), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		PortalDesc.vInitPos = ModelDesc1.vPosition;
		PortalDesc.vConnectPos = ModelDesc2.vPosition;
		PortalDesc.bConnectPortal2D = false;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TAILCAVE, pLayerTag, &PortalDesc)))
			return E_FAIL;

		PortalDesc.vInitPos = ModelDesc2.vPosition;
		PortalDesc.vConnectPos = ModelDesc1.vPosition;
		PortalDesc.bConnectPortal2D = true;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_TAILCAVE, pLayerTag, &PortalDesc)))
			return E_FAIL;



	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_TailCave::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	CFireEffect::EFFECTDESC EffectDesc;
	

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_Fire.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		EffectDesc.vInitPositon = XMVectorSetW(XMLoadFloat3(&ModelDesc.vPosition), 1.f);

		EffectDesc.eEffectType = CEffect::MODEL;
		EffectDesc.eEffectID = CFireEffect::FIRE_SMALL;
		EffectDesc.vInitScale = _float3(3.f, 3.f, 3.f);
		EffectDesc.iTextureNum = 0;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FireEffect"), LEVEL_TAILCAVE, pLayerTag, &EffectDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	hFile = CreateFile(TEXT("../../../Bin/Data/TailCave_FireSmall.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		EffectDesc.vInitPositon = XMVectorSetW(XMLoadFloat3(&ModelDesc.vPosition), 1.f);

		EffectDesc.eEffectType = CEffect::MODEL;
		EffectDesc.eEffectID = CFireEffect::FIRE_SMALL;
		EffectDesc.vInitScale = _float3(2.f, 2.f, 2.f);
		EffectDesc.iTextureNum = 0;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FireEffect"), LEVEL_TAILCAVE, pLayerTag, &EffectDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}



CLevel_TailCave * CLevel_TailCave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_TailCave*	pInstance = new CLevel_TailCave(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_TailCave"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_TailCave::Free()
{
	__super::Free();
	Safe_Release(m_pCollision_Manager);


}
