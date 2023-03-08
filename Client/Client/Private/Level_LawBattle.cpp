#include "stdafx.h"
#include "..\Public\Level_LawBattle.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "Level_Loading.h"
#include "BattleManager.h"
#include "Monster.h"
#include "PlayerState.h"

CLevel_LawBattle::CLevel_LawBattle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_LawBattle::Initialize()
{
	MONSTER_ID eMonsterID = CBattleManager::Get_Instance()->Get_MonsterType();
	_bool isBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}
	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Background")) == false)
	{
		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Background"))))
			return E_FAIL;
	}
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Deco"));
	


	if (isBattleMode)
	{
		if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_Battle_UI(TEXT("Layer_UI"))))
			return E_FAIL;

		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Ready_Camera(LEVEL::LEVEL_LAWBATTLE);
		m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
		m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LAWBATTLE);
		m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

		g_fSoundVolume = 0.f;
		CGameInstance::Get_Instance()->StopSound(SOUND_NATURE);
		CGameInstance::Get_Instance()->StopAll();
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BGM_FightWith_Law.wav"), g_fSoundVolume);
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("LawBattleEnter.dat"), XMMatrixIdentity());

		CPlayerManager::Get_Instance()->Update_StrikePosition(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Strike_Position.dat"));
	}
	else
	{
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Npc"));


		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);

		if (CGameInstance::Get_Instance() ->Get_PastLevelIndex() == LEVEL_CITY)
			pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(60.f, 0.f, 20.f, 1.f));
		else
			pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());
		pPlayer->Change_Navigation(LEVEL_LAWBATTLE);
		pPlayer->Compute_CurrentIndex(LEVEL_LAWBATTLE);
		pPlayer->Check_Navigation();
		pPlayer->Off_IsFly();
		pPlayer->Change_Level(LEVEL_LAWBATTLE);


		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Ready_Camera(LEVEL::LEVEL_LAWBATTLE);
		m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
		m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_PLAYER);
		m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, -10.f, 20.f, 0.f));

	}

	
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (pPlayer)
		pPlayer->Get_Renderer()->Set_Fog(false);
	return S_OK;
}

void CLevel_LawBattle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	_bool isBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();
	if (isBattleMode)
	{
		BattleTick(fTimeDelta);
	}
	else
	{
		NotBattleTick(fTimeDelta);
		
	}

}

void CLevel_LawBattle::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("BattleZoneLevel"));

	_bool isBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();
	if (isBattleMode)
	{
		BattleLateTick(fTimeDelta);
	}
	else
	{
		NotBattleLateTick(fTimeDelta);
	}

}

void CLevel_LawBattle::BattleTick(_float fTimeDelta)
{
	CBattleManager* pBattleManager = GET_INSTANCE(CBattleManager);
	if (pBattleManager->Get_LackonMonster() != nullptr && dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage >= 4.f)
		CPlayerManager::Get_Instance()->Set_SmashAttack();



	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() != CCamera_Dynamic::CAM_LOCKON)
	{
		g_fSoundVolume += 0.001f;
		if (g_fSoundVolume >= 0.15f)
			g_fSoundVolume = 0.15f;
	}

	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);


	if (CBattleManager::Get_Instance()->Get_IsBattleMode() == false)
	{

		CUI_Manager::Get_Instance()->ReSet_Arrived_Count();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Deco"));

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		LEVEL eNextLevel = LEVEL_LAWBATTLE;

		m_pCollision_Manager->Clear_AllCollisionGroup();
		pGameInstance->Set_DestinationLevel(eNextLevel);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;
		Safe_Release(pGameInstance);

	}


	if (CBattleManager::Get_Instance()->Get_IsHitLeg() == true)
	{
		m_fHitLegTime += CGameInstance::Get_Instance()->Get_TimeDelta(TEXT("Timer_60"));
		if (m_fHitLegTime > 0.1f)
		{
			CGameInstance::Get_Instance()->Set_TimeSpeedOffset(TEXT("Timer_Object"), 1.f);
			m_fHitLegTime = 0.f;
		}
	}



	RELEASE_INSTANCE(CBattleManager);
}

void CLevel_LawBattle::NotBattleTick(_float fTimeDelta)
{
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

			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Camera"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Background"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Deco"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Npc"));
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			LEVEL eNextLevel = (LEVEL)m_iNextLevelIndex;
			if (eNextLevel == LEVEL_LAWBATTLE)
			{
				CBattleManager::Get_Instance()->Set_BattleMode(true);
				CBattleManager::Get_Instance()->Set_OneonOneMode(true);
			}

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

void CLevel_LawBattle::BattleLateTick(_float fTimeDelta)
{
	_int iMonsterSize = (_int)CBattleManager::Get_Instance()->Get_BattleMonster().size();

	if (iMonsterSize == 0)
	{
		CBattleManager::Get_Instance()->Set_BattleMode(false);

	}

	if (CGameInstance::Get_Instance()->Key_Down(DIK_CAPSLOCK))
	{
		CUI_RuneEffect::RUNEDESC desc;
		desc.position.x = 80.f;
		desc.position.y = 35.f;
		desc.m_etype = 1;
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_LAWBATTLE, TEXT("test"), &desc)))
			return;
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(DIK_CAPSLOCK))
	{


		CBaseObj* pLockOn = CBattleManager::Get_Instance()->Get_LackonMonster();
		if (pLockOn != nullptr)
		{
			m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LOCKON);
			m_pCamera->Set_TargetPosition(pLockOn->Get_TransformState(CTransform::STATE_TRANSLATION));
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_1))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::ALPHEN);
			if (!m_bZumIn)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("ZumIn.wav"), SOUND_EFFECT, 1.0f);
				m_bZumIn = true;
			}
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_2))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::SION);
			if (!m_bZumIn)
			{
				CGameInstance::Get_Instance()->PlaySounds(TEXT("ZumIn.wav"), SOUND_EFFECT, 1.0f);
				m_bZumIn = true;
			}
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_3))
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::RINWELL);
		if (CGameInstance::Get_Instance()->Key_Down(DIK_4))
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::LAW);
		if (CGameInstance::Get_Instance()->Key_Down(DIK_Z))
		{
			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_Z);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_X))
		{
			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BATTLE, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_X);
		}
	}
	else
	{
		if (m_pCamera->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
		{
			if (CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_IsFly())
				CPlayerManager::Get_Instance()->Get_ActivePlayer()->Off_IsFly();

			m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LOCKOFF);
		}
	}

	if (CGameInstance::Get_Instance()->Key_Up(DIK_K))
	{
		CPlayer * pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Take_Damage(10000, nullptr);
	}



	if (CGameInstance::Get_Instance()->Key_Up(DIK_O))
	{
		CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell"));
		CBaseObj* pObject = dynamic_cast<CBaseObj*>(CGameInstance::Get_Instance()->Get_Object(LEVEL_LAWBATTLE, TEXT("Layer_Rinwell")));
		dynamic_cast<CMonster*>(pObject)->Change_Navigation(LEVEL_LAWBATTLE);
		dynamic_cast<CMonster*>(pObject)->Compute_CurrentIndex(LEVEL_LAWBATTLE);
		dynamic_cast<CMonster*>(pObject)->Set_BattleMode(true);
		CBattleManager::Get_Instance()->Add_BattleMonster(pObject);

	}

}

void CLevel_LawBattle::NotBattleLateTick(_float fTimeDelta)
{

}

HRESULT CLevel_LawBattle::Ready_Lights()
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

HRESULT CLevel_LawBattle::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::RINWELL) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
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

	
	CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::ALPHEN);
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc1.vPosition), 1.f));
	pPlayer->Change_Navigation(LEVEL_LAWBATTLE);
	pPlayer->Compute_CurrentIndex(LEVEL_LAWBATTLE);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_LAWBATTLE);

	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_vector vPosition[3] = { XMVectorSetW(XMLoadFloat3(&ModelDesc2.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc3.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc4.vPosition), 1.f) };
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Set_State(CTransform::STATE_TRANSLATION, vPosition[i]);
		iter->Change_Navigation(LEVEL_LAWBATTLE);
		iter->Compute_CurrentIndex(LEVEL_LAWBATTLE);
		iter->Check_Navigation();
		iter->Change_Level(LEVEL_LAWBATTLE);
		i++;
	}
	
	CPlayerManager::Get_Instance()->Set_BattleMode(true);
	CPlayerManager::Get_Instance()->Set_Ai_Check();
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_LawBattle::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CBattleManager*			pBattleManager = GET_INSTANCE(CBattleManager);
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_LAWBATTLE, TEXT("Layer_Boss"));
	CBaseObj* pBossMonster = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_LAWBATTLE, TEXT("Layer_Boss")));
	pBattleManager->Add_BattleMonster(pBossMonster);
	pBattleManager->Set_BossMonster(pBossMonster);


	MONSTER_ID eMonsterID = pBattleManager->Get_MonsterType();
	vector<CBaseObj*>		vecAllMonster = pBattleManager->Get_AllMonster();
	
	for (auto& iter : vecAllMonster)
	{
		dynamic_cast<CMonster*>(iter)->Save_LastPosition();
		pGameInstance->Out_GameObject(LEVEL_STATIC, TEXT("Layer_Monster"), iter);
	}

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/MonsterPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	vector<CBaseObj*>		vecBattleMonster = pBattleManager->Get_BattleMonster();
	for (auto& iter : vecBattleMonster)
	{
		ReadFile(hFile, &(ModelDesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, ModelDesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		iter->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&ModelDesc.vPosition), 1.f));
		dynamic_cast<CMonster*>(iter)->Change_Navigation(LEVEL_LAWBATTLE);
		dynamic_cast<CMonster*>(iter)->Compute_CurrentIndex(LEVEL_LAWBATTLE);
		dynamic_cast<CMonster*>(iter)->Set_BattleMode(true);
		pBattleManager->Set_LackonMonster(iter);
	}
	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CBattleManager);

	return S_OK;
}

HRESULT CLevel_LawBattle::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_LAWBATTLE, pLayerTag, nullptr)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_LawBattle::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_LAWBATTLE, pLayerTag, &CameraDesc)))
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_LAWBATTLE, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_LawBattle::Ready_Layer_Battle_UI(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Alpha_Screen"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	_int numcreate = (_int)(CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 2);

	for (int i = 0; i < numcreate; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar"), LEVEL_LAWBATTLE, pLayerTag, &i)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont"), LEVEL_LAWBATTLE, pLayerTag, &i)))
			return E_FAIL;

	}



	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Comboline"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_font"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	/*for (int i = 0; i < 6; ++i)
	{
	_uint number = i;

	*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MPguage"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitback"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;*/

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_button"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_change_button"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_LOCKON"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Skillmsg"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MonsterHP"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;

	
	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;


	if (CBattleManager::Get_Instance()->Get_IsBossBattle())
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_BossMonsterHP"), LEVEL_LAWBATTLE, pLayerTag)))
			return E_FAIL;
	}
	/*if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_PartyMessage"), LEVEL_LAWBATTLE, pLayerTag)))
		return E_FAIL;*/

	/**/
	

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_LawBattle * CLevel_LawBattle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_LawBattle*	pInstance = new CLevel_LawBattle(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_LawBattle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_LawBattle::Free()
{
	__super::Free();
	Safe_Release(m_pCollision_Manager);

}
