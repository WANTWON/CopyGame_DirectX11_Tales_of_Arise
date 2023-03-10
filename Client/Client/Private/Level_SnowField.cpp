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
#include "Effect.h"

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

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}
	CCameraManager* pCameraManager = CCameraManager::Get_Instance();
	pCameraManager->Ready_Camera(LEVEL::LEVEL_SNOWFIELD);
	CCamera* pCamera = pCameraManager->Get_CurrentCamera();
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
	dynamic_cast<CCamera_Dynamic*>(pCamera)->Set_Position(XMVectorSet(10.f, 20.f, -10.f, 1.f));



	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund")) == false)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Backgorund"))))
			return E_FAIL;
	}


	vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
	for (auto& iter : vecFightedMonster)
	{
		if (iter == RINWELL)
		{
			CUI_Manager::Get_Instance()->Set_Dialogue_section(5);
			CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("LayerNpcRinwell"));
			break;
		}
	}

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
	CPlayerManager::Get_Instance()->Set_BattleMode(false);

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (!g_bUIMade)
	{
		if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
			return E_FAIL;

		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Monster"));
		

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
		list<CGameObject*>* MonsterList = pGameInstance->Get_ObjectList(LEVEL_STATIC, TEXT("Layer_Monster"));

		for (auto& iter : *MonsterList)
		{
			CBaseObj* pMonster = dynamic_cast<CBaseObj*>(iter);
			CBattleManager::Get_Instance()->Add_Monster(pMonster);
		}

		vector<CBaseObj*> vecAllMonster = CBattleManager::Get_Instance()->Get_AllMonster();
		for (auto& iter : vecAllMonster)
		{
			dynamic_cast<CMonster*>(iter)->Change_Navigation(LEVEL_SNOWFIELD);
			dynamic_cast<CMonster*>(iter)->Compute_CurrentIndex(LEVEL_SNOWFIELD);
			dynamic_cast<CMonster*>(iter)->Set_BattleMode(false);

		}
		RELEASE_INSTANCE(CGameInstance);
		g_bUIMade = true;
	}
	else
	{
		if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
			return E_FAIL;
	}

	g_bIsSoundOn = false;
	g_fSoundVolume = 0.f;

	CGameInstance::Get_Instance()->StopAll();
	CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_SnowField_2.wav"), g_fSoundVolume);
	CGameInstance::Get_Instance()->PlaySounds(TEXT("Nature_blizzard.wav"), SOUND_NATURE, 0.3f);
	CPlayerManager::Get_Instance()->Get_ActivePlayer()->Set_StrikeAttack(false);

	if (vecFightedMonster.size() != 0)
	{
		_float exp = rand() % 500 + 500.f;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_BattleResult"), LEVEL_SNOWFIELD, TEXT("sss"), &exp)))
			return S_OK;
	}

	return S_OK;
}

void CLevel_SnowField::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Effects")) == false)
	{
		if (!m_bFirst)
		{
			HANDLE hFile = 0;
			_ulong dwByte = 0;
			NONANIMDESC  ModelDesc;
			_uint iNum = 0;
			hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/FogPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			/* 타일의 개수 받아오기 */
			ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

			for (_uint i = 0; i < iNum; ++i)
			{
				ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);

				_matrix mWorldMatrix = XMLoadFloat4x4(&ModelDesc.WorldMatrix);
				CEffect::PlayEffectAtLocation(TEXT("Fog.dat"), mWorldMatrix);
			}
			CloseHandle(hFile);

		
			_matrix mWorldMatrix = XMMatrixTranslation(128.f, 60.f, 128.f);
			CEffect::PlayEffectAtLocation(TEXT("Snow_Particles_1.dat"), mWorldMatrix);
			CEffect::PlayEffectAtLocation(TEXT("Snow_Particles_2.dat"), mWorldMatrix);
			CEffect::PlayEffectAtLocation(TEXT("Snow_Particles_2.dat"), mWorldMatrix);
			mWorldMatrix = XMMatrixTranslation(128.f, 60.f, 256.f);

		m_bFirst = true;

		}

	}
	
	
	if (!g_bIsSoundOn)
	{
		g_fSoundVolume += 0.001f;
		if (g_fSoundVolume >= 0.3f)
		{
			g_fSoundVolume = 0.3f;
			g_bIsSoundOn = true;
		}

		CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_MINUS))
	{
		CBattleManager::Get_Instance()->Set_BattleMode(true, RINWELL, true);
	}


	if (m_bNextNevel)
	{
		_float fDuration = 1.5f;
		_float fBlurDuration = fDuration / 3;

		if (m_fBlurTimer >= fDuration)
		{
			m_fBlurTimer = 0.f;

			CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
			if (pPlayer)
				pPlayer->Get_Renderer()->Set_ZoomBlur(false);

			m_pCollision_Manager->Clear_AllCollisionGroup();

			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Effects"));
			CGameInstance::Get_Instance()->Clear_Layer(LEVEL_STATIC, TEXT("Layer_Monster"));
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			LEVEL eNextLevel = LEVEL_CITY;
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

	if (CGameInstance::Get_Instance()->Key_Up(DIK_0))
	{
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		if (pPlayer)
			pPlayer->Get_Renderer()->Set_ZoomBlur(false);

		CPlayerManager::Get_Instance()->Save_LastPosition();
		m_pCollision_Manager->Clear_AllCollisionGroup();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Effects"));
		CGameInstance::Get_Instance()->Clear_Layer(LEVEL_STATIC, TEXT("Layer_Monster"));

		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		LEVEL eNextLevel = LEVEL_BOSS;
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
			

			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Camera"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Backgorund"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Interact"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Instancing"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Deco"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Npc"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Trigger"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_SNOWFIELD, TEXT("Layer_Effects"));

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

		_float3 FogColor = _float3(0.2f, 0.4f, 0.9f);
		if (FAILED(pShaderPostProcessing->Set_RawValue("g_vFogColor", &FogColor, sizeof(_float3))))
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
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Player"));


	if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::RINWELL) == nullptr)
	{
		vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
		vecFightedMonster.push_back(RINWELL);
		for (auto& iter : vecFightedMonster)
		{
			if (iter == RINWELL)
			{
				CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Player"));
				break;
			}
		}
	}
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());
	CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);

	pPlayer->Change_Navigation(LEVEL_SNOWFIELD);
	pPlayer->Compute_CurrentIndex(LEVEL_SNOWFIELD);
	pPlayer->Check_Navigation();
	pPlayer->Change_Level(LEVEL_SNOWFIELD);
	
	if (pPlayer->Get_IsFly() == true)
		pPlayer->Off_IsFly();

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;

	vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
	vector<CBaseObj*> vecAllMonster = CBattleManager::Get_Instance()->Get_AllMonster();

	for (auto& iter : vecAllMonster)
	{
		pGameInstance->ReAdd_GameObject(LEVEL_STATIC, TEXT("Layer_Monster"), iter);
		iter->Set_State(CTransform::STATE_TRANSLATION, dynamic_cast<CMonster*>(iter)->Get_LastPosition());
		dynamic_cast<CMonster*>(iter)->Change_Navigation(LEVEL_SNOWFIELD);
		dynamic_cast<CMonster*>(iter)->Compute_CurrentIndex(LEVEL_SNOWFIELD);
		dynamic_cast<CMonster*>(iter)->Set_BattleMode(false);

	}

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CLevel_SnowField::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), LEVEL_SNOWFIELD, pLayerTag, TEXT("Prototype_SnowField_WaterTerrain"))))
		return E_FAIL;

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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SystemMessagebox"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Skillmsg"), LEVEL_STATIC, pLayerTag)))
		return E_FAIL;

	

	
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_SnowField::Ready_Layer_Deco_SnowField(const _tchar * pLayerTag)
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
	
	CGameInstance::Get_Instance()->StopSound(SOUND_NATURE);

}
