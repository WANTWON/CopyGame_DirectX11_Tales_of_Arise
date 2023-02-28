#include "stdafx.h"
#include "..\Public\Level_Restaurant.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Level_Loading.h"
#include "ThrowingObject.h"

CLevel_Restaurant::CLevel_Restaurant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_Restaurant::Initialize()
{
	CBattleManager::Get_Instance()->Set_BattleMode(false);

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Deco"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Interact"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Portal"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Npc"));



	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_RESTAURANT);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
	m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_ROOM);
	m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

	g_fSoundVolume = 0.f;
	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_Restaurant.wav"), g_fSoundVolume);

	return S_OK;
}

void CLevel_Restaurant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	g_fSoundVolume += 0.001f;
	if (g_fSoundVolume >= 0.3f)
		g_fSoundVolume = 0.3f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);

	if (m_bNextNevel)
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
			pPlayer->Get_Renderer()->Set_ZoomBlur(false);

		m_pCollision_Manager->Clear_AllCollisionGroup();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Deco"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Portal"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Interact"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_RESTAURANT, TEXT("Layer_Npc"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = (LEVEL)m_iNextLevelIndex;
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}

	if (m_bMinigameStart)
	{
		if (m_fLimitTime >= m_fTotalTime)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			CThrowingObject::THROWDESC ThrowDesc;

			for (_int i = 0; i < 4; ++i)
			{
				if (m_fSpwanTime < m_fCreateTime[i])
				{
					_int iPhase = _int(floor(m_fTotalTime) / 20) + 1;
					_int iNumCreate = rand() % (iPhase + 1);

					for (_int j = 0; j < iNumCreate;)
					{
						_int iIndex = rand() % 3;

						if (m_bIsSpwan[i][iIndex])
							continue;

						ZeroMemory(&ThrowDesc, sizeof(CThrowingObject::THROWDESC));

						_int iModel;
						
						switch (iPhase)
						{
						case 1:
							iModel = rand() % 12;
							break;
						case 2:
							iModel = rand() % 17;
							break;
						case 3:
							iModel = rand() % 18;
							break;
						default:
							iModel = 0;
							break;
						}
						
						switch (iModel)
						{
						case 0:
						case 1:
						case 2:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "Fruit_Pineapple");
							ThrowDesc.eType = CThrowingObject::TYPE_PINEAPPLE;
							ThrowDesc.tNonDesc.vScale = _float3(2.f, 2.f, 2.f);
							break;
						case 3:
						case 4:
						case 5:
						case 6:
						case 7:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "Food_Potato");
							ThrowDesc.eType = CThrowingObject::TYPE_POTATO;
							ThrowDesc.tNonDesc.vScale = _float3(3.f, 3.f, 3.f);
							break;
						case 8:
						case 9:
						case 10:
						case 11:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "Apple");
							ThrowDesc.eType = CThrowingObject::TYPE_APPLE;
							ThrowDesc.tNonDesc.vScale = _float3(5.f, 5.f, 5.f);
							break;
						case 12:
						case 13:
						case 14:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "RedOnion");
							ThrowDesc.eType = CThrowingObject::TYPE_REDONION;
							ThrowDesc.tNonDesc.vScale = _float3(2.f, 2.f, 2.f);
							break;
						case 15:
						case 16:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "Bread_Croissant");
							ThrowDesc.eType = CThrowingObject::TYPE_BREAD;
							ThrowDesc.tNonDesc.vScale = _float3(3.f, 3.f, 3.f);
							break;
						case 17:
							strcpy_s(ThrowDesc.tNonDesc.pModeltag, "Fruit_Mango");
							ThrowDesc.eType = CThrowingObject::TYPE_MANGO;
							ThrowDesc.tNonDesc.vScale = _float3(3.f, 3.f, 3.f);
							break;
						}

						switch (i)
						{
						case 0:
							switch (iIndex)
							{
							case 0:
								ThrowDesc.tNonDesc.vPosition = _float3(8.f, 5.f, 17.f);
								break;
							case 1:
								ThrowDesc.tNonDesc.vPosition = _float3(18.f, 5.f, 17.f);
								break;
							case 2:
								ThrowDesc.tNonDesc.vPosition = _float3(28.f, 5.f, 17.f);
								break;
							}
							break;
						case 1:
							switch (iIndex)
							{
							case 0:
								ThrowDesc.tNonDesc.vPosition = _float3(35.f, 5.f, 40.f);
								break;
							case 1:
								ThrowDesc.tNonDesc.vPosition = _float3(35.f, 5.f, 33.f);
								break;
							case 2:
								ThrowDesc.tNonDesc.vPosition = _float3(35.f, 5.f, 26.f);
								break;
							}
							break;
						case 2:
							switch (iIndex)
							{
							case 0:
								ThrowDesc.tNonDesc.vPosition = _float3(1.f, 5.f, 40.f);
								break;
							case 1:
								ThrowDesc.tNonDesc.vPosition = _float3(1.f, 5.f, 33.f);
								break;
							case 2:
								ThrowDesc.tNonDesc.vPosition = _float3(1.f, 5.f, 26.f);
								break;
							}
							break;
						case 3:
							switch (iIndex)
							{
							case 0:
								ThrowDesc.tNonDesc.vPosition = _float3(8.f, 5.f, 49.f);
								break;
							case 1:
								ThrowDesc.tNonDesc.vPosition = _float3(18.f, 5.f, 49.f);
								break;
							case 2:
								ThrowDesc.tNonDesc.vPosition = _float3(28.f, 5.f, 49.f);
								break;
							}
							break;
						}

						ThrowDesc.fTime = ((rand() % 15) + 5) * 0.1f;

						if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_ThrowingObject"), LEVEL_RESTAURANT, TEXT("Layer_ThrowingObject"), &ThrowDesc)))
						{
							ERR_MSG(TEXT("Failed Create MiniGame Object"));
							return;
						}

						++j;
					}

					m_fCreateTime[i] = 0.f;

					for (_int j = 0; j < 3; ++j)
						m_bIsSpwan[i][j] = false;
				}
				else
					m_fCreateTime[i] += fTimeDelta + ((rand() % 100) * 0.001f);
			}

			RELEASE_INSTANCE(CGameInstance);
		}

		m_fTotalTime += fTimeDelta;
	}
}

void CLevel_Restaurant::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("LEVEL_RESTAURANT"));

	
		

	if (m_fTotalTime >= m_fLimitTime)
	{
		m_bMinigameStart = false;

		CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);

		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
		pPlayer->Change_Navigation(LEVEL_RESTAURANT);

		pPlayer->Compute_CurrentIndex(LEVEL_RESTAURANT);
		pPlayer->Check_Navigation();
		pPlayer->Change_Level(LEVEL_RESTAURANT);
		if (pPlayer->Get_IsFly())
			pPlayer->Off_IsFly();

		m_fTotalTime = 0.f;
	}
}

HRESULT CLevel_Restaurant::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/FoodSliceGame/Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &LightDesc, sizeof(LIGHTDESC), &dwByte, nullptr);
		//if (0 == dwByte)
			//break;

		if (LightDesc.eType == LIGHTDESC::TYPE_POINT)
			LightDesc.vPosition.w = 1.f;
		pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc);
	}
	CloseHandle(hFile);

	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, XMVectorSet(20, 30, 15, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(20, 0, 15, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Restaurant::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);


	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
	pPlayer->Change_Navigation(LEVEL_RESTAURANT);
	pPlayer->Compute_CurrentIndex(LEVEL_RESTAURANT);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_RESTAURANT);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
		iter->Change_Navigation(LEVEL_RESTAURANT);
		iter->Compute_CurrentIndex(LEVEL_RESTAURANT);
		iter->Check_Navigation();
		iter->Change_Level(LEVEL_RESTAURANT);
		i++;
	}

	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_Restaurant::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_RESTAURANT, pLayerTag, nullptr)))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/City_Data/Water.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Water"), LEVEL_RESTAURANT, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_Restaurant::Ready_Layer_Camera(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CCamera_Dynamic::CAMERADESC_DERIVED				CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_Dynamic::CAMERADESC_DERIVED));

	CameraDesc.InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.vDistance = _float4(0, 10, -10, 0.f);

	CameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	CameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	CameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.CameraDesc.fNear = 0.1f;
	CameraDesc.CameraDesc.fFar = 1000.f;

	CameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	CameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_RESTAURANT, pLayerTag, &CameraDesc)))
		return E_FAIL;


	CCamera_Action::ACTIONCAMDESC				ActionCameraDesc;
	ZeroMemory(&ActionCameraDesc, sizeof(CCamera_Action::ACTIONCAMDESC));

	ActionCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	ActionCameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	ActionCameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	ActionCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	ActionCameraDesc.CameraDesc.fNear = 0.1f;
	ActionCameraDesc.CameraDesc.fFar = 1000.f;

	ActionCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 3.f;
	ActionCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_RESTAURANT, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;


	CCamera_MiniGame::CAMERADESC_DERIVED				MiniGameCameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_MiniGame::CAMERADESC_DERIVED));

	MiniGameCameraDesc.InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
	MiniGameCameraDesc.vDistance = _float4(0, 10, -10, 0.f);

	MiniGameCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	MiniGameCameraDesc.CameraDesc.vAt = _float4(0.f, 1.f, 0.f, 1.f);

	MiniGameCameraDesc.CameraDesc.fFovy = XMConvertToRadians(50.0f);
	MiniGameCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	MiniGameCameraDesc.CameraDesc.fNear = 0.1f;
	MiniGameCameraDesc.CameraDesc.fFar = 1000.f;

	MiniGameCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	MiniGameCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_MiniGame"), LEVEL_RESTAURANT, pLayerTag, &MiniGameCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Restaurant::Set_MiniGameStart(_bool tof)
{
	 m_bMinigameStart = tof;
	if (m_bMinigameStart)
	{
		CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_MINIGAME);
		dynamic_cast<CCamera_MiniGame*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_CamMode(CCamera_MiniGame::MINIGAME_SLASH);

		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(18, 0.f, 31.f, 1.f));
		pPlayer->Change_Navigation(LEVEL_RESTAURANT);
		pPlayer->Compute_CurrentIndex(LEVEL_RESTAURANT);
		pPlayer->Check_Navigation();
		pPlayer->Change_Level(LEVEL_RESTAURANT);
		if (pPlayer->Get_IsFly())
			pPlayer->Off_IsFly();

		m_iScore = 0;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_minigame1"), LEVEL_RESTAURANT, TEXT("score"))))
			return;

	}
	else
	{
		CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);

		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
		pPlayer->Change_Navigation(LEVEL_RESTAURANT);

		pPlayer->Compute_CurrentIndex(LEVEL_RESTAURANT);
		pPlayer->Check_Navigation();
		pPlayer->Change_Level(LEVEL_RESTAURANT);
		if (pPlayer->Get_IsFly())
			pPlayer->Off_IsFly();
	}
	
}



CLevel_Restaurant * CLevel_Restaurant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Restaurant*	pInstance = new CLevel_Restaurant(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_Restaurant"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Restaurant::Free()
{
	__super::Free();

	//CBattleManager::Get_Instance()->Clear_Monster();
	Safe_Release(m_pCollision_Manager);

	//CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);

}
