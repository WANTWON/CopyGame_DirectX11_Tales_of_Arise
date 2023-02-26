#include "stdafx.h"
#include "..\Public\Level_WorkTool.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "BattleManager.h"
#include "Monster.h"
#include "Level_Loading.h"

CLevel_WorkTool::CLevel_WorkTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_WorkTool::Initialize()
{
	CBattleManager::Get_Instance()->Set_BattleMode(false);
	
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Deco"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Interact"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Portal"));

		

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_WORKTOOL);
	m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
	m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_ROOM);
	m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

	g_fSoundVolume = 0.f;
	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("SnowFiledSong.wav"), g_fSoundVolume);
	
	return S_OK;
}

void CLevel_WorkTool::Tick(_float fTimeDelta)
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

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Deco"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Portal"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_WORKTOOL, TEXT("Layer_Interact"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = (LEVEL)m_iNextLevelIndex;
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}

}

void CLevel_WorkTool::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("LEVEL_WORKTOOL"));

	if (CGameInstance::Get_Instance()->Key_Up(DIK_SPACE))
	{
		m_bMinigameStart = !m_bMinigameStart;

		if (m_bMinigameStart)
		{
			CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_MINIGAME);
			dynamic_cast<CCamera_MiniGame*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Set_CamMode(CCamera_MiniGame::MINIGAME_SHOOTING);
		}
		else
		{
			CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
		}
	}

}

HRESULT CLevel_WorkTool::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/MiniGameRoom_Data/ShootingGame/Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

HRESULT CLevel_WorkTool::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);


	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
	pPlayer->Change_Navigation(LEVEL_WORKTOOL);
	pPlayer->Compute_CurrentIndex(LEVEL_WORKTOOL);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_WORKTOOL);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(20, 0.f, 3.f, 1.f));
		iter->Change_Navigation(LEVEL_WORKTOOL);
		iter->Compute_CurrentIndex(LEVEL_WORKTOOL);
		iter->Check_Navigation();
		iter->Change_Level(LEVEL_WORKTOOL);
		i++;
	}
	
	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_WorkTool::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_WORKTOOL, pLayerTag, nullptr)))
		return E_FAIL;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_WorkTool::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_WORKTOOL, pLayerTag, &CameraDesc)))
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_WORKTOOL, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;


	CCamera_MiniGame::CAMERADESC_DERIVED				MiniGameCameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera_MiniGame::CAMERADESC_DERIVED));

	MiniGameCameraDesc.InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
	MiniGameCameraDesc.vDistance = _float4(0, 10, -10, 0.f);

	MiniGameCameraDesc.CameraDesc.vEye = _float4(0.f, 10.0f, -10.f, 1.f);
	MiniGameCameraDesc.CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);

	MiniGameCameraDesc.CameraDesc.fFovy = XMConvertToRadians(60.0f);
	MiniGameCameraDesc.CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	MiniGameCameraDesc.CameraDesc.fNear = 0.1f;
	MiniGameCameraDesc.CameraDesc.fFar = 1000.f;

	MiniGameCameraDesc.CameraDesc.TransformDesc.fSpeedPerSec = 10.f;
	MiniGameCameraDesc.CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_MiniGame"), LEVEL_WORKTOOL, pLayerTag, &MiniGameCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_WorkTool * CLevel_WorkTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_WorkTool*	pInstance = new CLevel_WorkTool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_WorkTool"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_WorkTool::Free()
{
	__super::Free();
	Safe_Release(m_pCollision_Manager);
}
