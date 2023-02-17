#include "stdafx.h"
#include "..\Public\Level_SnowField.h"

#include "GameInstance.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "Level_Loading.h"
#include "PlayerManager.h"
#include "BattleManager.h"

#include "Item.h"
#include "TreasureBox.h"
#include "SnowFieldNpc.h"
#include "PlayerCreater.h"
#include "Object_Pool_Manager.h"
#include "Monster.h"

extern bool		g_bUIMade = false;

CLevel_SnowField::CLevel_SnowField(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_SnowField::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	cout << " Initialize start" << endl;
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}

	
		cout << " Player Clone start" << endl;
		m_pPlayerLoader = CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_PLAYER);
		if (nullptr == m_pPlayerLoader)
			return E_FAIL;

		cout << " Player Clone start2" << endl;
		m_pPlayer2Loader = CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_PLAYER2);
		if (nullptr == m_pPlayer2Loader)
				return E_FAIL;

		cout << " Monster Group1 Clone start" << endl;
		m_pMonsterLoader1 = CPlayerCreater::Create(m_pDevice, m_pContext, CLONE_MONSTER1);
		if (nullptr == m_pMonsterLoader1)
			return E_FAIL;



	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund")) == false)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Backgorund"))))
			return E_FAIL;
	}

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Insteract")) == false)
	{
		if (FAILED(Ready_Layer_Interact_Object(TEXT("Layer_Insteract"))))
			return E_FAIL;
	}

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));


	if (m_pPlayerLoader != nullptr)
	{
		DWORD dwTime = GetTickCount();
		while (false == m_pPlayerLoader->Get_Finished() || false == m_pPlayer2Loader->Get_Finished() ||
			false == m_pMonsterLoader1->Get_Finished())
		{
			if (dwTime + 1000 < GetTickCount())
			{
				if (m_pPlayerLoader->Get_Finished() == true)
					cout << "Finished Player Clone" << endl;

				if (m_pPlayer2Loader->Get_Finished() == true)
					cout << "Finished Player Clone2" << endl;


				if (m_pMonsterLoader1->Get_Finished() == true)
					cout << "Finished Monster Grounp1 Clone" << endl;

				dwTime = GetTickCount();
			}
		}
	}
	


	if(m_pPlayerLoader != nullptr)
		Safe_Release(m_pPlayerLoader);
	if (m_pPlayer2Loader != nullptr)
		Safe_Release(m_pPlayer2Loader);
	if (m_pMonsterLoader1 != nullptr)
		Safe_Release(m_pMonsterLoader1);

	if (!g_bUIMade)
	{
		if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
			return E_FAIL;
		g_bUIMade = true;
	}

	CPlayerManager::Get_Instance()->Set_BattleMode(false);

	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_SNOWFIELD);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_Position(XMVectorSet(10.f, 20.f, -10.f, 1.f));


	g_fSoundVolume = 0.f;
	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("SnowFiledSong.wav"), g_fSoundVolume);


	vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
	if (vecFightedMonster.size() != 0)
	{
		_float exp = rand() % 500 + 500;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_BattleResult"), LEVEL_SNOWFIELD, TEXT("sss"), &exp)))
			return S_OK;
	}

	return S_OK;
}

void CLevel_SnowField::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	g_fSoundVolume += 0.001f;
	if (g_fSoundVolume >= 0.3f)
		g_fSoundVolume = 0.3f;
	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);



	if (CGameInstance::Get_Instance()->Key_Up(DIK_MINUS))
	{
		CBattleManager::Get_Instance()->Set_BattleMode(true, RINWELL, true);
	}


	if (CBattleManager::Get_Instance()->Get_IsBattleMode())
	{
		CPlayerManager::Get_Instance()->Save_LastPosition();
		m_pCollision_Manager->Clear_AllCollisionGroup();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Insteract"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = LEVEL_BATTLE;
		pGameInstance->Set_DestinationLevel(eNextLevel);
		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;

		RELEASE_INSTANCE(CGameInstance);
	}
}

void CLevel_SnowField::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("SnowFieldLevel"));

	/* Fog Shader */
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (pPlayer)
	{
		pPlayer->Get_Renderer()->Set_Fog(true);

		CShader* pShaderPostProcessing = pPlayer->Get_Renderer()->Get_ShaderPostProcessing();
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_ViewMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
			return;
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_ProjMatrixInv", &pGameInstance->Get_TransformFloat4x4_Inverse_TP(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
			return;

		_float3 vPlayerPosition;
		XMStoreFloat3(&vPlayerPosition, pPlayer->Get_TransformState(CTransform::STATE::STATE_TRANSLATION));

		if (FAILED(pShaderPostProcessing->Set_RawValue("g_vPlayerPosition", &vPlayerPosition, sizeof(_float3))))
			return;
	}

	RELEASE_INSTANCE(CGameInstance);
}

HRESULT CLevel_SnowField::Ready_Lights()
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);
	pGameInstance->Clear_AllLight();
	LIGHTDESC			LightDesc;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Light.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

	XMStoreFloat4(&vLightEye, XMVectorSet(36, 300, 300, 1.f));
	XMStoreFloat4(&vLightAt, XMVectorSet(36, 0, 200, 1.f));

	pGameInstance->Set_ShadowLightView(vLightEye, vLightAt);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Alphen"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;

		/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sion"), LEVEL_STATIC, pLayerTag, nullptr)))
			return E_FAIL;*/

		//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, pLayerTag, nullptr)))
		//	return E_FAIL;

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(44, 0, 22, 1.f));
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
		pPlayer->Change_Level(LEVEL_SNOWFIELD);
	}
	else
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());
		pPlayer->Change_Navigation(LEVEL_SNOWFIELD);
		pPlayer->Compute_CurrentIndex(LEVEL_SNOWFIELD);
		pPlayer->Check_Navigation();
		pPlayer->Change_Level(LEVEL_SNOWFIELD);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc;
	vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Monster.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		_bool m_bNotCreate = false;
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("Ice_Wolf")))
		{
			for (auto& iter : vecFightedMonster)
			{
				if (iter == ICE_WOLF)
					m_bNotCreate = true;
			}
			if (m_bNotCreate)
				continue;

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ice_Wolf"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Hawk")))
		{
			for (auto& iter : vecFightedMonster)
			{
				if (iter == HAWK)
					m_bNotCreate = true;
			} 
			if (m_bNotCreate)
				continue;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hawk"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Berserker")))
		{
			for (auto& iter : vecFightedMonster)
			{
				if (iter == BERSERKER)
					m_bNotCreate = true;
			}
			if (m_bNotCreate)
				continue;
				if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Berserker"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
					return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Slime")))
		{
			for (auto& iter : vecFightedMonster)
			{
				if (iter == SLIME)
					m_bNotCreate = true;
			}
			if (m_bNotCreate)
				continue;
			//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Slime"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc.vScale)))
			//	return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("Rinwell")))
		{
			for (auto& iter : vecFightedMonster)
			{
				if (iter == RINWELL)
					m_bNotCreate = true;
			}
			if (m_bNotCreate)
				continue;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AiRinwell"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
				return E_FAIL;
		}

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_SNOWFIELD, pLayerTag, TEXT("Prototype_SnowField_Terrain"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_SNOWFIELD, pLayerTag, nullptr)))
		return E_FAIL;

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Water.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Water"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



HRESULT CLevel_SnowField::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_SNOWFIELD, pLayerTag, &CameraDesc)))
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_SNOWFIELD, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	CUI_Manager::Get_Instance()->Set_Mainmenuon(false);
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI"), LEVEL_SNOWFIELD, pLayerTag)))
		return E_FAIL;*/



	/*_int numcreate = (_int)(CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 2);

	for (int i = 0; i < numcreate; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar"), LEVEL_STATIC, pLayerTag, &i)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont"), LEVEL_STATIC, pLayerTag, &i)))
			return E_FAIL;

	}
*/


//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Comboline"), LEVEL_STATIC, pLayerTag)))
//		return E_FAIL;
//
//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"), LEVEL_STATIC, pLayerTag)))
//		return E_FAIL;
//
//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_font"), LEVEL_STATIC, pLayerTag)))
//		return E_FAIL;
//
//	/*for (int i = 0; i < 6; ++i)
//	{
//		_uint number = i;
//*/
//		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_STATIC, pLayerTag)))
//			return E_FAIL;
	
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/


	//}


	//	for (int i = 0; i < 3; ++i)
	//	{
	//		_uint number = i;

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"), LEVEL_STATIC, pLayerTag)))
	//	return E_FAIL;

	////	}

	//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage"), LEVEL_STATIC, pLayerTag)))
	//	return E_FAIL;

	//for (int i = 0; i < 7; ++i)
	//{
	//	_uint number = i;

	//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage_font"), LEVEL_STATIC, pLayerTag, &i)))//, &i)))
	//		return E_FAIL;

	//}



	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MPguage"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar_World"), LEVEL_SNOWFIELD, pLayerTag)))
		return E_FAIL;*/

		//	for (int i = 0; i < 4; ++i)
		//	{
		//		_uint number = i;

		//	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont_World"), LEVEL_SNOWFIELD, pLayerTag, &i)))
		//			return E_FAIL;
				/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont_World"), LEVEL_SNOWFIELD, pLayerTag)))
					return E_FAIL;*/

					//	}
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitback"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_button"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_change_button"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_LOCKON"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MENU_BACK"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Dialogue"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Dialoguepopup"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_INTERECTMSG"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Sidepopup"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Dialogue_Caption"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
	


	/*_float exp = 500.f;
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_BattleResult"), LEVEL_STATIC, pLayerTag , &exp)))
		return E_FAIL;*/

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Levelup"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/
	
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Skillmsg"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/
	

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_itemmsgbox"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/
	

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTSTART"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_QUESTMESSAGE"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;
*/
	
	
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_GetITEMPOPUP"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;*/



	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_Interact_Object(const _tchar * pLayerTag)
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
		ReadFile(hFile, &(ItemDesc.ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ItemDesc.ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);
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
			ItemDesc.etype = CItem::SLIMPLANT;
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
	}
	CloseHandle(hFile);


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_DecoObject(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/CliffRock2.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
	}

	CloseHandle(hFile);

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
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;
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
	}
	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_Trigger(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Triggerbox.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Trigger"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_SnowField::Ready_Layer_Test(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC  ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Boss.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("Astral_Doubt")))
		{
			////여기서 몬스터 생성하세요
			//if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AstralDoubt"), LEVEL_SNOWFIELD, pLayerTag, &ModelDesc)))
			//	return E_FAIL;
		}
	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

CLevel_SnowField * CLevel_SnowField::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_SnowField*	pInstance = new CLevel_SnowField(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_SnowField"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_SnowField::Free()
{
	

	__super::Free();

	Safe_Release(m_pCollision_Manager);
	
	

}
