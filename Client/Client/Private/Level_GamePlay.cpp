#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Level_Loading.h"

/* Manager& UI*/
#include "CameraManager.h"
#include "UI_Manager.h"
#include "InvenTile.h"
#include "InvenItem.h"
#include "UIButton.h"
#include "UIScreen.h"
#include "UIName.h"

/* Object */
#include "NonAnim.h"
#include "Player.h"
#include "Door.h"
#include "TreasureBox.h"
#include "SquareBlock.h"
#include "Grass.h"
#include "Portal.h"
#include "Npc.h"
#include "FieldDecoObject.h"

_bool g_bUIMadefirst = false;

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_GamePlay::Initialize()
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

	if (FAILED(Ready_Layer_Npc(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Portal(TEXT("Layer_Portal"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Warp(TEXT("Layer_Warp"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;	

	if (g_bUIMadefirst == false)
	{

		if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
			return E_FAIL;

		CUI_Manager::Get_Instance()->Initialize_PlayerState();
		g_bUIMadefirst = true;
	}

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();

	pCameraManager->Ready_Camera(LEVEL::LEVEL_GAMEPLAY);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	CUI_Manager::Get_Instance()->Set_NextLevel(false);


	g_fBGMVolume = 0.f;

	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("1_0 Field (Normal).mp3"), g_fBGMVolume);
	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
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

void CLevel_GamePlay::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	//SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));
	SetWindowText(g_hWnd, TEXT("GamePlaye Level."));

	m_pCollision_Manager->Update_Collider();
	m_pCollision_Manager->CollisionwithBullet();


	Setting_Light();
}

void CLevel_GamePlay::Setting_Light()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));

	_float fPositionZ = XMVectorGetZ(pPlayer->Get_TransformState(CTransform::STATE_POSITION));
	LIGHTDESC			LightDesc = *pGameInstance->Get_LightDesc(0);

	if (fPositionZ < 30.f)
	{
		if (m_eMusic == FIELD)
		{
			g_fBGMVolume -= 0.005f;
			if (g_fBGMVolume <= 0.f)
			{
				pGameInstance->StopSound(SOUND_BGM);
				pGameInstance->PlayBGM(TEXT("1-10 Mysterious Forest.mp3"), g_fBGMVolume);
				CUI_Manager::Get_Instance()->Set_NextLevel(false);
				m_eMusic = MYSTERY;

				if (!m_bMakeLight)
				{
					/* For.Point */
					LIGHTDESC PointLightDesc;
					ZeroMemory(&PointLightDesc, sizeof(LIGHTDESC));
					PointLightDesc.eType = LIGHTDESC::TYPE_POINT;
					XMStoreFloat4(&PointLightDesc.vPosition, pPlayer->Get_TransformState(CTransform::STATE_POSITION));
					PointLightDesc.vPosition.y += 3.f;
					PointLightDesc.fRange = 5.f;
					PointLightDesc.vDiffuse = _float4(0.f, 0.f, 0.f, 1.f);
					PointLightDesc.vAmbient = _float4(0.3f, 0.3f, 0.3f, 1.f);
					PointLightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

					if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, PointLightDesc)))
						return;

					m_bMakeLight = true;
				}
			}


		}


		LightDesc.vDiffuse.x -= 0.005f;
		LightDesc.vDiffuse.y -= 0.005f;

		if (LightDesc.vDiffuse.x <= 0.3f)
		{
			LightDesc.vDiffuse.x = 0.3f;
			LightDesc.vDiffuse.y = 0.3f;
		}

		LightDesc.vSpecular.x -= 0.005f;
		LightDesc.vSpecular.y -= 0.005f;
		if (LightDesc.vSpecular.x <= 0.3f)
		{
			LightDesc.vSpecular.x = 0.3f;
			LightDesc.vSpecular.y = 0.3f;
		}

		LightDesc.vDiffuse.z -= 0.005f;
		if (LightDesc.vDiffuse.z <= 0.5f)
			LightDesc.vDiffuse.z = 0.5f;

		pGameInstance->Set_LightDesc(0, &LightDesc);


		if (m_bMakeLight)
		{
			LIGHTDESC PointLightDesc = *pGameInstance->Get_LightDesc(1);
			XMStoreFloat4(&PointLightDesc.vPosition, pPlayer->Get_TransformState(CTransform::STATE_POSITION));
			PointLightDesc.vPosition.y += 3.f;
			PointLightDesc.vDiffuse.x += 0.05f;
			PointLightDesc.vDiffuse.y += 0.05f;
			PointLightDesc.vDiffuse.z += 0.05f;

			if (PointLightDesc.vDiffuse.x >= 1.f)
			{
				PointLightDesc.vDiffuse.x = 1.f;
				PointLightDesc.vDiffuse.y = 1.f;
				PointLightDesc.vDiffuse.z = 1.f;
			}
			pGameInstance->Set_LightDesc(1, &PointLightDesc);
		}

	}
	else
	{

		if (m_bMakeLight)
		{
			LIGHTDESC PointLightDesc = *pGameInstance->Get_LightDesc(1);
			PointLightDesc.vDiffuse.x -= 0.005f;
			PointLightDesc.vDiffuse.y -= 0.005f;
			PointLightDesc.vDiffuse.z -= 0.005f;
			pGameInstance->Set_LightDesc(1, &PointLightDesc);

			if (PointLightDesc.vDiffuse.x <= 0.f)
			{
				pGameInstance->Clear_Light(1);
				m_bMakeLight = false;
			}
		}


		if (m_eMusic == MYSTERY)
		{
			g_fBGMVolume -= 0.005f;
			if (g_fBGMVolume <= 0.f)
			{
				pGameInstance->StopSound(SOUND_BGM);
				pGameInstance->PlayBGM(TEXT("1_0 Field (Normal).mp3"), g_fBGMVolume);
				CUI_Manager::Get_Instance()->Set_NextLevel(false);
				m_eMusic = FIELD;
			}


		}


		LightDesc.vDiffuse.x += 0.005f;
		LightDesc.vDiffuse.y += 0.005f;
		if (LightDesc.vDiffuse.x >= 1.f)
		{
			LightDesc.vDiffuse.x = 1.f;
			LightDesc.vDiffuse.y = 1.f;
		}

		LightDesc.vSpecular.x += 0.005f;
		LightDesc.vSpecular.y += 0.005f;

		if (LightDesc.vSpecular.x >= 0.7f)
		{
			LightDesc.vSpecular.x = 0.7f;
			LightDesc.vSpecular.y = 0.7f;

		}


		LightDesc.vDiffuse.z += 0.005f;
		if (LightDesc.vDiffuse.z >= 0.8f)
		{
			LightDesc.vDiffuse.z = 0.8f;
		}


		pGameInstance->Set_LightDesc(0, &LightDesc);
	}




	RELEASE_INSTANCE(CGameInstance);

	return;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Clear_AllLight();

	LIGHTDESC			LightDesc;

	/* For.Directional*/
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));

	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(-0.3f, -1.f, -0.3f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 0.8f, 1.f);
	LightDesc.vAmbient = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vSpecular = _float4(0.7f, 0.7f, 0.7f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(40, 100.f, 73, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(39, 0, 71, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);


	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Player"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;
		CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		LEVEL ePastLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_PastLevelIndex();
		pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(36.3f, 0.f, 44.8f, 1.f));
		//pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(16.3f, 0.f, 18.8f, 1.f));
		//pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(55.8f, 4.2f, 10.3f, 1.f));
		//pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.33f, 3.f, 66.8f, 1.f));
		pPlayer->Change_Navigation(LEVEL_GAMEPLAY);
		pPlayer->Compute_CurrentIndex(LEVEL_GAMEPLAY);

	}
	else
	{
		CPlayer* pPlayer = (CPlayer*)pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player"));
		pPlayer->Change_Navigation(LEVEL_GAMEPLAY);
		LEVEL ePastLevel = (LEVEL)CLevel_Manager::Get_Instance()->Get_PastLevelIndex();
		switch (ePastLevel)
		{
		case Client::LEVEL_TAILCAVE:
			pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(55.8f, 4.2f, 10.3f, 1.f));
			break;
		case Client::LEVEL_ROOM:
		{
			CUI_Manager::ROOMTYPE eRoomType = CUI_Manager::Get_Instance()->Get_RoomType();
			if (eRoomType == CUI_Manager::MARINHOUSE)
				pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(36.3f, 0.f, 46.8f, 1.f));
			else if (eRoomType == CUI_Manager::SHOP)
				pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(52.0f, 0.f, 60.f, 1.f));
			else if (eRoomType == CUI_Manager::CRANEGAME)
				pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(53.9f, 0.f, 34.38f, 1.f));
			else if (eRoomType == CUI_Manager::TELEPHONE)
				pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(36.45f, 0.f, 33.82f, 1.f));
			break;
		}
		case Client::LEVEL_TOWER:
			pPlayer->Set_State(CTransform::STATE_POSITION, XMVectorSet(2.33f, 3.f, 66.8f, 1.f));
			break;
		}

		pPlayer->Compute_CurrentIndex(LEVEL_GAMEPLAY);
	}

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	LEVEL eLevel = LEVEL_GAMEPLAY;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), &eLevel)))
		return E_FAIL;



	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Filed_Map.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 20; ++i)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Effect"), LEVEL_GAMEPLAY, pLayerTag, nullptr)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.iTest = 10;

	CameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -6.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.2f;
	CameraDesc.CameraDesc.fFar = 500.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_GAMEPLAY, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_GAMEPLAY, pLayerTag)))
		//return E_FAIL;


	CUIScreen::BACKGROUNDESC BackgroundDesc;
	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_PLAYGAME;
	BackgroundDesc.pTextureTag = TEXT("Prototype_Component_Texture_GamePlayScreen_UI");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_STATIC, pLayerTag,
		&BackgroundDesc)))
		return E_FAIL;



	BackgroundDesc.eVisibleScreen = CUIScreen::VISIBLE_EFFECT;
	BackgroundDesc.pTextureTag = nullptr;
	if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_BackGround_UI"), LEVEL_STATIC, pLayerTag,
		&BackgroundDesc)))
		return E_FAIL;


	CInvenTile::INVENDESC InvenDesc;
	InvenDesc.eTileType = CInvenTile::EQUIP_TILE;
	InvenDesc.eState = CInvenTile::STATE_EQUIP;
	InvenDesc.eEquipKey = CInvenTile::EQUIP_X;
	InvenDesc.vPosition = _float2(1100, 50);

	CUIButton::BUTTONDESC ButtonDesc;
	ButtonDesc.eButtonType = CUIButton::BTN_FIX;
	ButtonDesc.iTexNum = CUIButton::BTN_BLACK;
	ButtonDesc.eState = CUIButton::BTN_X;
	ButtonDesc.vPosition = _float2(InvenDesc.vPosition.x - 20, InvenDesc.vPosition.y + 20);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InvenTile_UI"), LEVEL_STATIC, pLayerTag, &InvenDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, pLayerTag, &ButtonDesc)))
		return E_FAIL;

	InvenDesc.vPosition = _float2(1200, 100);
	InvenDesc.eEquipKey = CInvenTile::EQUIP_Y;
	ButtonDesc.eState = CUIButton::BTN_Y;
	ButtonDesc.vPosition = _float2(InvenDesc.vPosition.x - 20, InvenDesc.vPosition.y + 20);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InvenTile_UI"), LEVEL_STATIC, pLayerTag, &InvenDesc)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, pLayerTag, &ButtonDesc)))
		return E_FAIL;


	ButtonDesc.eButtonType = CUIButton::BTN_INTERACT;
	ButtonDesc.eState = CUIButton::BTN_A;
	ButtonDesc.vPosition = _float2(0, 0);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, pLayerTag, &ButtonDesc)))
		return E_FAIL;



	for (_int i = 0; i < 3; ++i)
	{
		for (_int j = 0; j < 4; ++j)
		{

			CInvenTile::INVENDESC InvenDesc;
			InvenDesc.eTileType = CInvenTile::INVEN_TILE;
			InvenDesc.eState = CInvenTile::STATE_DEFAULT;
			InvenDesc.eEquipKey = CInvenTile::EQUIP_NONE;
			InvenDesc.vPosition = _float2(_float(780 + j * 110), _float(260 + i * 120));

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_InvenTile_UI"), LEVEL_STATIC, TEXT("Layer_Ineventile"), &InvenDesc)))
				return E_FAIL;
		}
	}


	ButtonDesc.eButtonType = CUIButton::BTN_INVEN;
	ButtonDesc.iTexNum = CUIButton::BTN_GREEN;
	ButtonDesc.eState = CUIButton::BTN_X;
	ButtonDesc.vPosition = _float2(780 - 30, 260 + 30);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, pLayerTag, &ButtonDesc)))
		return E_FAIL;

	ButtonDesc.eState = CUIButton::BTN_Y;
	ButtonDesc.vPosition = _float2(890 - 30, 260 + 30);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CUIButton"), LEVEL_STATIC, pLayerTag, &ButtonDesc)))
		return E_FAIL;


	for (int i = 0; i < 5; ++i)
	{
		CInvenItem::ITEMDESC ItemDesc;
		ItemDesc.eItemType = CInvenItem::ITEM_EQUIP;
		ItemDesc.m_iTextureNum = (CInvenItem::EQUIP_TEXLIST)(i + 1);
		ItemDesc.vPosition = _float2(165.f, 245.f + 70 * i);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_EquipItem"), &ItemDesc)))
			return E_FAIL;
	}

	for (int i = 0; i < 5; ++i)
	{
		CInvenItem::ITEMDESC ItemDesc;
		ItemDesc.eItemType = CInvenItem::ITEM_QUEST;
		ItemDesc.m_iTextureNum = (CInvenItem::QUEST_TEXLIST)(0);
		ItemDesc.vPosition = _float2(645.f, 245.f + 70 * i);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_QuestItem"), &ItemDesc)))
			return E_FAIL;
	}


	for (int i = 0; i < 5; ++i)
	{
		CInvenItem::ITEMDESC ItemDesc;
		ItemDesc.eItemType = CInvenItem::ITEM_DGNKEY;
		ItemDesc.m_iTextureNum = 0;
		ItemDesc.vPosition = _float2(285.f + 55 * i, 545.f);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_DungeonKey"), &ItemDesc)))
			return E_FAIL;
	}

	for (int i = 0; i < 4; ++i)
	{
		_float2 vPosition = _float2(0.f, 0.f);

		vPosition.x = (i == 1 || i == 2) ? 400 + 95.f : 400 + 40.f;

		vPosition.y = 230.f + i * 70.f;

		CInvenItem::ITEMDESC ItemDesc;
		ItemDesc.eItemType = CInvenItem::ITEM_COLLECT;
		ItemDesc.m_iTextureNum = CInvenItem::COLLECT_NONE;
		ItemDesc.vPosition = vPosition;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_Collect"), &ItemDesc)))
			return E_FAIL;

		vPosition.x = (i == 1 || i == 2) ? 400 - 95.f : 400 - 40.f;
		ItemDesc.m_iTextureNum = CInvenItem::COLLECT_NONE;
		ItemDesc.vPosition = vPosition;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CInvenItem"), LEVEL_STATIC, TEXT("Layer_Collect"), &ItemDesc)))
			return E_FAIL;
	}

	CUIName::NAMEDESC NameDesc;
	NameDesc.iTexNum = CUIName::NAME_FIELD;
	NameDesc.vPosition = _float2(0.f, 0.f);
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UIName"), LEVEL_STATIC, TEXT("Layer_Name"), &NameDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Filed_Monster.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("Octorock.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Octorock"), LEVEL_GAMEPLAY, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("MoblinSword.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_MoblinSword"), LEVEL_GAMEPLAY, pLayerTag, &ModelDesc.vPosition)))
				return E_FAIL;
		}

	}




	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Filed_Object.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("TailCaveShutter.fbx")))
		{

			CDoor::DOORDESC DoorDesc;
			DoorDesc.eType = CDoor::DOOR_TAIL;
			DoorDesc.InitPosition = ModelDesc.vPosition;
			DoorDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Door"), LEVEL_GAMEPLAY, TEXT("Layer_Shutter"), &DoorDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("TailStatue.fbx")))
		{

			CSquareBlock::BLOCKDESC BlockDesc;
			BlockDesc.eType = CSquareBlock::TAIL_STATUE;
			BlockDesc.vInitPosition = ModelDesc.vPosition;
			BlockDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SquareBlock"), LEVEL_GAMEPLAY, pLayerTag, &BlockDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("Lawn.fbx")))
		{

			CGrass::GRASSDESC GrassDesc;
			GrassDesc.eType = CGrass::LAWN;
			GrassDesc.vPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("Grass.fbx")))
		{

			CGrass::GRASSDESC GrassDesc;
			GrassDesc.eType = CGrass::GRASS;
			GrassDesc.vPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("Grass2x2.fbx")))
		{

			CGrass::GRASSDESC GrassDesc;
			GrassDesc.eType = CGrass::GRASS2x2;
			GrassDesc.vPosition = ModelDesc.vPosition;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Grass"), LEVEL_GAMEPLAY, pLayerTag, &GrassDesc)))
				return E_FAIL;

		}

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Portal(const _tchar * pLayerTag)
{
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;


	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Portal.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	CPortal::PORTALDESC PortalDesc;
	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_TOWER;
	PortalDesc.bConnectPortal2D = true;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_ROOM;
	PortalDesc.eRoomType = CPortal::MARINHOUSE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_ROOM;
	PortalDesc.eRoomType = CPortal::SHOP;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;


	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_ROOM;
	PortalDesc.eRoomType = CPortal::CRANE_GAME;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_ROOM;
	PortalDesc.eRoomType = CPortal::TELEPHONE;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;

	ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

	PortalDesc.ePortalType = CPortal::PORTAL_LEVEL;
	PortalDesc.vInitPos = ModelDesc.vPosition;
	PortalDesc.eConnectLevel = LEVEL_TAILCAVE;
	PortalDesc.eRoomType = CPortal::ROOM_END;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Portal"), LEVEL_GAMEPLAY, pLayerTag, &PortalDesc)))
		return E_FAIL;

	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Npc(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Filed_Deco.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
		if (!wcscmp(pModeltag, TEXT("CuccoKeeper.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::CUCCO_KEEPER;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CuccoKeeper"), LEVEL_GAMEPLAY, TEXT("Layer_CuccoKeeper"), &NpcDesc)))
				return E_FAIL;

		}
		if (!wcscmp(pModeltag, TEXT("MadamMeowMeow.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::MADAM;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CuccoKeeper"), LEVEL_GAMEPLAY, pLayerTag, &NpcDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("QuadrupletGreen.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::CHILD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CuccoKeeper"), LEVEL_GAMEPLAY, TEXT("Layer_Children"), &NpcDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("GrandmaUlrira.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::GRANDMA;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CuccoKeeper"), LEVEL_GAMEPLAY, pLayerTag, &NpcDesc)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("Cucco.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Cucco"), LEVEL_GAMEPLAY, TEXT("Layer_Cucco"), &ModelDesc.vPosition)))
				return E_FAIL;

		}
		else if (!wcscmp(pModeltag, TEXT("Butterfly.fbx")))
		{

			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::BUTTERFLY;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("BirdGreen.fbx")))
		{

			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::BIRD_GREEN;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("BirdOrange.fbx")))
		{

			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::BIRD_ORANGE;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("WeatherClock.fbx")))
		{
			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::WEATHER_CLOCK;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Moosh.fbx")))
		{

			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::MOOSH;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Fox.fbx")))
		{

			CFieldDecoObject::DECODESC DecoDesc;
			DecoDesc.eDecoType = CFieldDecoObject::FOX;
			DecoDesc.vInitPos = ModelDesc.vPosition;
			DecoDesc.fAngle = ModelDesc.m_fAngle;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_FieldDecoObject"), LEVEL_GAMEPLAY, pLayerTag, &DecoDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("QuadrupletsMother.fbx")))
		{
			CNpc::NPCDESC NpcDesc;
			NpcDesc.vInitPos = ModelDesc.vPosition;
			NpcDesc.eNpcType = CNpc::MOTHER;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CuccoKeeper"), LEVEL_GAMEPLAY, pLayerTag, &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("EaglesTower.fbx")))
		{
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}

	}

	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Warp(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNonAnim::NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Filed_Warp.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(CNonAnim::NONANIMDESC), &dwByte, nullptr);

		CSquareBlock::BLOCKDESC BlockDesc;
		BlockDesc.eType = CSquareBlock::WARP_HOLE;
		BlockDesc.vInitPosition = ModelDesc.vPosition;
		BlockDesc.fAngle = ModelDesc.m_fAngle;
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SquareBlock"), LEVEL_GAMEPLAY, pLayerTag, &BlockDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*	pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	Safe_Release(m_pCollision_Manager);

}
