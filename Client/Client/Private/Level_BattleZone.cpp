#include "stdafx.h"
#include "..\Public\Level_BattleZone.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "Level_Loading.h"

CLevel_BattleZone::CLevel_BattleZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_BattleZone::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Instancing(TEXT("Layer_Instancing"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_DecoObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Interact_Object(TEXT("Layer_Interact_Object"))))
	//	return E_FAIL;




	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_BATTLE);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_Position(XMVectorSet(10.f, 20.f, -10.f, 1.f));

	return S_OK;
}

void CLevel_BattleZone::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (CGameInstance::Get_Instance()->Key_Up(DIK_MINUS))
	{
		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		LEVEL eNextLevel = LEVEL_SNOWFIELD;

		CPlayerManager::Get_Instance()->Save_LastPosition();
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

void CLevel_BattleZone::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("BattleZoneLevel"));


	CBaseObj* pPlayer = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
	_float4		vLightEye, vLightAt;

	XMStoreFloat4(&vLightEye, pPlayer->Get_TransformState(CTransform::STATE_TRANSLATION));
	vLightEye.y = 0.f;
	vLightAt = vLightEye;
	vLightEye.y += 50.f;
	vLightEye.z += 50.f;

	CGameInstance::Get_Instance()->Set_ShadowLightView(vLightEye, vLightAt);

}

HRESULT CLevel_BattleZone::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Light2.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

	XMStoreFloat4(&vLightEye, XMVectorSet(36, 50, 70, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(36, 0, 20, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Alphen"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;
			
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(44, 0.f, 22, 1.f));
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sion"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;
	}
	else
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(64.f, 0.f, 64.f, 1.f));
		pPlayer->Change_Navigation(LEVEL_BATTLE);
		pPlayer->Compute_CurrentIndex(LEVEL_BATTLE);
		pPlayer->Check_Navigation();

		vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
		_vector vPosition[3] = {{ 58.f, 0.f, 60.f, 1.f },{ 70.f, 0.f, 60.f, 1.f },{ 64.f, 0.f, 56.f, 1.f } };
		_int i = 0;
		for (auto& iter : pAIPlayers)
		{
			iter->Set_State(CTransform::STATE_TRANSLATION, vPosition[i]);
			iter->Change_Navigation(LEVEL_BATTLE);
			iter->Compute_CurrentIndex(LEVEL_BATTLE);
			iter->Check_Navigation();
			i++;
		}
	}

	CPlayerManager::Get_Instance()->Set_BattleMode(true);
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 1; ++i)
	{
		_vector vPosition = { 48.f, 0.f, 48.f, 1.f };
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Berserker"), LEVEL_BATTLE, pLayerTag, &vPosition)))

			return E_FAIL;
	}

	//for (_uint i = 0; i < 1; ++i)
	//{
	//	_vector vPosition = { 51.f, 0.f, 51.f, 1.f };
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hawk"), LEVEL_BATTLE, pLayerTag, &vPosition)))
	//		return E_FAIL;
	//}

	//for (_uint i = 0; i < 1; ++i)
	//{
	//	_vector vPosition = { 54.f, 0.f, 54.f, 1.f };
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ice_Wolf"), LEVEL_BATTLE, pLayerTag, &vPosition)))
	//		return E_FAIL;
	//}

	//for (_uint i = 0; i < 1; ++i)
	//{
	//	_vector vPosition = { 48.f, 0.f, 48.f, 1.f };
	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Slime"), LEVEL_BATTLE, pLayerTag, &vPosition)))
	//		return E_FAIL;
	//}



	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_BATTLE, TEXT("Layer_Terrain"), TEXT("Prototype_BattleZone_Terrain"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_BATTLE, pLayerTag, nullptr)))
		return E_FAIL;
	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Water"), LEVEL_SNOWFIELD, pLayerTag, nullptr)))
		//return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Effect(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Particle_Rect"), LEVEL_BATTLE, pLayerTag, nullptr)))
		return E_FAIL;
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Particle_Point"), LEVEL_SNOWFIELD, pLayerTag, nullptr)))
		return E_FAIL;*/

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_BATTLE, pLayerTag, &CameraDesc)))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevel_BattleZone::Ready_Layer_Interact_Object(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	for (_uint i = 0; i < 1; ++i)
	{
		_vector vPosition = { 128, 5.f, 128, 1.f };
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_TreasureBox"), LEVEL_BATTLE, pLayerTag, &vPosition)))
			return E_FAIL;
	}

	for (_uint i = 0; i < 1; ++i)
	{
		_vector vPosition = { 128, 5.f, 128, 1.f };
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Item"), LEVEL_BATTLE, pLayerTag, &vPosition)))
			return E_FAIL;
	}


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_Instancing(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	NONANIMDESC stModelDesc;
	strcpy(stModelDesc.pModeltag, "Conifer3");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Dead_Grass");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Dead_Tree1");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Bush");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	strcpy(stModelDesc.pModeltag, "Broken_Tree");
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim_Instance"), LEVEL_BATTLE, pLayerTag, &stModelDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BattleZone::Ready_Layer_DecoObject(const _tchar * pLayerTag)
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
	}

	CloseHandle(hFile);

	Safe_Release(pGameInstance);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_BattleZone * CLevel_BattleZone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_BattleZone*	pInstance = new CLevel_BattleZone(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_BattleZone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BattleZone::Free()
{
	__super::Free();

	Safe_Release(m_pCollision_Manager);
}
