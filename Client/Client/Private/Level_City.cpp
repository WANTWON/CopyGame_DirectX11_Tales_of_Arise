#include "stdafx.h"
#include "..\Public\Level_BossZone.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Level_City.h"
#include "Level_Loading.h"

CLevel_City::CLevel_City(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_City::Initialize()
{
	CBattleManager::Get_Instance()->Set_BattleMode(false);
	
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_CITY);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
	m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

	g_fSoundVolume = 0.f;
	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("SnowFiledSong.wav"), g_fSoundVolume);
	
	return S_OK;
}

void CLevel_City::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	g_fSoundVolume += 0.001f;
	if (g_fSoundVolume >= 0.3f)
		g_fSoundVolume = 0.3f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);

	if (CGameInstance::Get_Instance()->Key_Up(DIK_0))
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
			pPlayer->Get_Renderer()->Set_ZoomBlur(false);

		CPlayerManager::Get_Instance()->Save_LastPosition();
		m_pCollision_Manager->Clear_AllCollisionGroup();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Insteract"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = LEVEL_CITY;
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);

		m_fBlurTimer = 0.f;
	}

	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		_float fDuration = 1.5f;
		_float fBlurDuration = fDuration / 3;

		if (m_fBlurTimer >= fDuration)
		{
			m_fBlurTimer = 0.f;

			CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
			if (pPlayer)
				pPlayer->Get_Renderer()->Set_ZoomBlur(false);

			CPlayerManager::Get_Instance()->Save_LastPosition();
			m_pCollision_Manager->Clear_AllCollisionGroup();

			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Camera"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Backgorund"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Insteract"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Instancing"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Deco"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_CITY, TEXT("Layer_Npc"));

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			LEVEL eNextLevel = LEVEL_BATTLE;
			pGameInstance->Set_DestinationLevel(eNextLevel);
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
				return;

			RELEASE_INSTANCE(CGameInstance);
		}
		else
		{
			CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
			if (pPlayer)
			{
				_float fFocusPower = 15.f;

				_float fInterpFactor = m_fBlurTimer / fBlurDuration;
				if (fInterpFactor > 1.f)
					fInterpFactor = 1.f;

				_int iFocusDetailLerp = 1 + fInterpFactor * (10 - 1);

				pPlayer->Get_Renderer()->Set_ZoomBlur(true, fFocusPower, iFocusDetailLerp);
			}

			m_fBlurTimer += fTimeDelta;
		}
	}
}

void CLevel_City::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("Level_City"));


	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLevel_City::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

	XMStoreFloat4(&vLightEye, XMVectorSet(50, 100, 100, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(50, 0, 0, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);

	
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_City::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::RINWELL) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;
	
	}
	if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::LAW) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Law"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;
	}

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc1;
	NONANIMDESC ModelDesc2;
	NONANIMDESC ModelDesc3;
	NONANIMDESC ModelDesc4;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);


	ReadFile(hFile, &(ModelDesc1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc2), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc3), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(ModelDesc4), sizeof(NONANIMDESC), &dwByte, nullptr);

	CloseHandle(hFile);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc1.vPosition), 1.f));
	pPlayer->Change_Navigation(LEVEL_CITY);
	pPlayer->Compute_CurrentIndex(LEVEL_CITY);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_CITY);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_vector vPosition[3] = { XMVectorSetW(XMLoadFloat3(&ModelDesc2.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc3.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc4.vPosition), 1.f) };
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, vPosition[i]);
		iter->Change_Navigation(LEVEL_CITY);
		iter->Compute_CurrentIndex(LEVEL_CITY);
		iter->Check_Navigation();
		iter->Change_Level(LEVEL_CITY);
		i++;
	}
	
	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	CPlayerManager::Get_Instance()->Set_Ai_Check();
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_City::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_City::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_CITY, pLayerTag, &CameraDesc)))
		return E_FAIL;


	CCamera_Action::ACTIONCAMDESC				ActionCameraDesc;
	ZeroMemory(&ActionCameraDesc, sizeof(CCamera_Action::ACTIONCAMDESC));

	ActionCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	ActionCameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	ActionCameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	ActionCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	ActionCameraDesc.CameraDesc.fNear = 0.1f;
	ActionCameraDesc.CameraDesc.fFar = 1000.f;

	ActionCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	ActionCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_CITY, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_City * CLevel_City::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_City*	pInstance = new CLevel_City(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_City"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_City::Free()
{
	__super::Free();

	//CBattleManager::Get_Instance()->Clear_Monster();
	Safe_Release(m_pCollision_Manager);

	//CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);

}
