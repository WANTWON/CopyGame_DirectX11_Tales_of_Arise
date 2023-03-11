#include "stdafx.h"
#include "..\Public\Level_BossZone.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "CameraManager.h"
#include "UI_RuneEffect.h"
#include "Level_Loading.h"
#include "BattleManager.h"
#include "Monster.h"
#include "AstralDoubt.h"
#include "UI_InterectMsg.h"
#include "UI_Dialogue.h"

CLevel_BossZone::CLevel_BossZone(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
	, m_pCollision_Manager(CCollision_Manager::Get_Instance())
{
	Safe_AddRef(m_pCollision_Manager);
}

HRESULT CLevel_BossZone::Initialize()
{
	
	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;
	dynamic_cast<CUI_InterectMsg*>(CUI_Manager::Get_Instance()->Get_System_msg())->Close_sysmsg();
	dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(16);
	if (CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Camera")) == false)
	{
		if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
			return E_FAIL;
	}

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));
	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Deco"));



	if (!m_bBattleMode)
	{
		g_bEnd = true;
		dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(19);
		if (FAILED(Ready_Layer_Player_NotBattle(TEXT("Layer_Player"))))
			return E_FAIL;

		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Ready_Camera(LEVEL::LEVEL_BOSS);
		m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
		m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_BOSSBATTLE);
		m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

		g_fSoundVolume = 0.f;
		CGameInstance::Get_Instance()->StopAll();

		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BossRoomEnding.dat"), XMMatrixIdentity());
		CGameInstance::Get_Instance()->PlayBGM(TEXT("BossKilledStroyBgm.wav"), g_fSoundVolume);
	}
	else
	{
		if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
			return E_FAIL;

		if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
			return E_FAIL;

		MONSTER_ID eMonsterID = CBattleManager::Get_Instance()->Get_MonsterType();

		CBattleManager::Get_Instance()->Set_BattleMode(true, ASTRAL_DOUBT, true);


		if (FAILED(Ready_Layer_Battle_UI(TEXT("Layer_UI"))))
			return E_FAIL;

		CCameraManager* pCameraManager = CCameraManager::Get_Instance();
		pCameraManager->Ready_Camera(LEVEL::LEVEL_BOSS);
		m_pCamera = dynamic_cast<CCamera_Dynamic*>(pCameraManager->Get_CurrentCamera());
		m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_BOSSBATTLE);
		m_pCamera->Set_Position(CPlayerManager::Get_Instance()->Get_ActivePlayer()->Get_TransformState(CTransform::STATE_TRANSLATION) + XMVectorSet(0.f, 20.f, -10.f, 0.f));

		g_fSoundVolume = 0.f;
		CGameInstance::Get_Instance()->StopAll();

		CGameInstance::Get_Instance()->PlayBGM(TEXT("Boss_Asu_BackGorundSound.wav"), g_fSoundVolume);
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BossRoomPlayerEnter.dat"), XMMatrixIdentity());
		CPlayerManager::Get_Instance()->Update_StrikePosition(TEXT("../../../Bin/Data/BattleZoneData/SnowPlane/Strike_Position.dat"));
	}
	
		
	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	if (pPlayer)
		pPlayer->Get_Renderer()->Set_Fog(false);
	return S_OK;
}

void CLevel_BossZone::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if ((CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC &&
		dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Get_CamMode() != CCamera_Dynamic::CAM_LOCKON) ||
		CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_ACTION)
	{
		g_fSoundVolume += 0.01f;
		if (g_fSoundVolume >= 0.15f)
			g_fSoundVolume = 0.15f;
	}

	CGameInstance::Get_Instance()->SetChannelVolume(SOUND_BGM, g_fSoundVolume);


	m_bBattleMode = CBattleManager::Get_Instance()->Get_IsBattleMode();

	Check_LevelChange();


	if (m_bBattleMode)
		BattleTick(fTimeDelta);
	else
	{
		if (!m_bFirstCutScene)
		{
			if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
			{
				m_bNextNevel = true;
				m_bFirstCutScene = true;
				
			}
		}
		

		if (m_bNextNevel)
		{
			CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
			if (pPlayer)
				pPlayer->Get_Renderer()->Set_ZoomBlur(false);

			m_pCollision_Manager->Clear_AllCollisionGroup();

			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Camera"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));
			CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Deco"));

			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

			LEVEL eNextLevel = LEVEL_CITY;

			pGameInstance->Set_DestinationLevel(eNextLevel);
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
				return;

			RELEASE_INSTANCE(CGameInstance);
		}
	}

}

void CLevel_BossZone::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	SetWindowText(g_hWnd, TEXT("BattleZoneLevel"));



	if (m_bBattleMode)
		BattleLateTick(fTimeDelta);


}

void CLevel_BossZone::Check_LevelChange()
{
	if (!m_bBattleMode && m_bFinal == true)
	{
		CUI_Manager::Get_Instance()->ReSet_Arrived_Count();

		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Camera"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Instancing"));
		CObject_Pool_Manager::Get_Instance()->Add_Pooling_Layer(LEVEL_BOSS, TEXT("Layer_Deco"));

		CGameInstance*		pGameInstance = CGameInstance::Get_Instance();
		Safe_AddRef(pGameInstance);

		LEVEL eNextLevel = LEVEL_BOSS;
		CBattleManager::Get_Instance()->Set_BattleMode(false);
		m_pCollision_Manager->Clear_AllCollisionGroup();
		pGameInstance->Set_DestinationLevel(eNextLevel);

		if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eNextLevel))))
			return;
		Safe_Release(pGameInstance);

	}
}

void CLevel_BossZone::BattleTick(_float fTimeDelta)
{

	if (!m_bFinal)
		LastAttackCheck();

	if (!m_bFirstCutScene)
		FirstCutScene();


	CBattleManager* pBattleManager = GET_INSTANCE(CBattleManager);
	if (pBattleManager->Get_LackonMonster() != nullptr && dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster())->Get_Stats().m_fLockonSmashGuage >= 4.f)
		CPlayerManager::Get_Instance()->Set_SmashAttack();


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

void CLevel_BossZone::BattleLateTick(_float fTimeDelta)
{
	if (!m_bFirstCutScene)
		FirstCutScene();

	_int iMonsterSize = CBattleManager::Get_Instance()->Get_BattleMonster().size();
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
		if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BOSS, TEXT("test"), &desc)))
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
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.7f);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_2))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::SION);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.7f);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_3))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::RINWELL);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.7f);
		}
		if (CGameInstance::Get_Instance()->Key_Down(DIK_4))
		{
			CPlayerManager::Get_Instance()->Set_ActivePlayer(CPlayer::LAW);
			CGameInstance::Get_Instance()->PlaySounds(TEXT("Chat_Next.wav"), SOUND_CROWD, 0.7f);
		}
		if (CGameInstance::Get_Instance()->Key_Down(DIK_Z))
		{
			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BOSS, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_Z);
		}

		if (CGameInstance::Get_Instance()->Key_Down(DIK_X))
		{
			CUI_RuneEffect::RUNEDESC desc;
			desc.position.x = 100.f;
			desc.position.y = 100.f;
			desc.m_etype = 1;
			if (FAILED(CGameInstance::Get_Instance()->Add_GameObject(TEXT("Prototype_GameObject_UI_Rune_Effect"), LEVEL_BOSS, TEXT("test"), &desc)))
				return;
			dynamic_cast<CCamera_Dynamic*>(CCameraManager::Get_Instance()->Get_CurrentCamera())->Change_LockOn(DIK_X);
		}


	}
	else
	{
		if (m_pCamera->Get_CamMode() == CCamera_Dynamic::CAM_LOCKON)
			m_pCamera->Set_CamMode(CCamera_Dynamic::CAM_LOCKOFF);
	}
}

void CLevel_BossZone::FirstCutScene()
{
	if (CCameraManager::Get_Instance()->Get_CamState() == CCameraManager::CAM_DYNAMIC)
	{
		CCameraManager::Get_Instance()->Play_ActionCamera(TEXT("BossEnter.dat"), XMMatrixIdentity());
		CBattleManager::Get_Instance()->Get_LackonMonster()->Set_IsActionMode(true);
		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
		pPlayer->Set_IsActionMode(true);
		pPlayer->Set_BattlePose(false);


		vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
		_int i = 0;
		for (auto& iter : pAIPlayers)
		{
			iter->Set_IsActionMode(true);
			iter->Set_BattlePose(false);
			i++;
		}

		m_bFirstCutScene = true;

	}

}

void CLevel_BossZone::LastAttackCheck()
{
	_int iMonsterSize = (_int)CBattleManager::Get_Instance()->Get_BattleMonster().size();

	if (iMonsterSize == 1 && m_bSecondCreated)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(CBattleManager::Get_Instance()->Get_LackonMonster());
		if (!pMonster)
			return;

		if (pMonster->Get_Stats().m_fCurrentHp <= pMonster->Get_Stats().m_fMaxHp *0.3f)
		{
			m_bFinal = true;
			dynamic_cast<CUI_Dialogue*>(CUI_Manager::Get_Instance()->Get_Dialogue())->Open_Dialogue(18);
			CGameInstance::Get_Instance()->StopAll();
			CGameInstance::Get_Instance()->PlayBGM(TEXT("forvictory.wav"), g_fSoundVolume);
			CPlayerManager* pPlayerManager = CPlayerManager::Get_Instance();

			HANDLE hFile = 0;
			_ulong dwByte = 0;
			NONANIMDESC Active1;
			NONANIMDESC Active2;
			NONANIMDESC AIplayer1;
			NONANIMDESC AIplayer2;
			_uint iNum = 0;

			hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (0 == hFile)
				return;

			ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

			ReadFile(hFile, &(Active1), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(Active2), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(AIplayer1), sizeof(NONANIMDESC), &dwByte, nullptr);
			ReadFile(hFile, &(AIplayer2), sizeof(NONANIMDESC), &dwByte, nullptr);
			CloseHandle(hFile);

			CPlayer* Alphen = pPlayerManager->Get_EnumPlayer(CPlayer::ALPHEN);
			CPlayer* Sion = pPlayerManager->Get_EnumPlayer(CPlayer::SION);
			CPlayer* Rinwell = pPlayerManager->Get_EnumPlayer(CPlayer::RINWELL);
			CPlayer* Law = pPlayerManager->Get_EnumPlayer(CPlayer::LAW);

			Alphen->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active1.vPosition), 1.f));
			Sion->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active2.vPosition), 1.f));
			Rinwell->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer1.vPosition), 1.f));
			Law->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer2.vPosition), 1.f));

			Alphen->Set_OverLimitState();
			Sion->Set_OverLimitState();
			Rinwell->Set_OverLimitState();
			Law->Set_OverLimitState();
		}
	}
}

HRESULT CLevel_BossZone::Ready_Lights()
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

HRESULT CLevel_BossZone::Ready_Layer_Player(const _tchar * pLayerTag)
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

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Kisara"), LEVEL_STATIC, TEXT("Layer_FPlayer"), nullptr)))
		return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Duohaelm"), LEVEL_STATIC, TEXT("Layer_FPlayer"), nullptr)))
		return E_FAIL;
	}

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC ModelDesc1;
	NONANIMDESC ModelDesc2;
	NONANIMDESC ModelDesc3;
	NONANIMDESC ModelDesc4;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/BattleZoneData/BossMap/PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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
	pPlayer->Change_Navigation(LEVEL_BOSS);
	pPlayer->Compute_CurrentIndex(LEVEL_BOSS);
	pPlayer->Check_Navigation();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_BOSS);
	pPlayer->Set_IsActionMode(true);
	pPlayer->Set_BattlePose(false);



	_vector vPosition[3] = { XMVectorSetW(XMLoadFloat3(&ModelDesc2.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc3.vPosition), 1.f),XMVectorSetW(XMLoadFloat3(&ModelDesc4.vPosition), 1.f) };
	
	CPlayer* pSion = CPlayerManager::Get_Instance()->Get_EnumPlayer(1);
	pSion->Set_State(CTransform::STATE_TRANSLATION, vPosition[0]);
	pSion->Change_Navigation(LEVEL_BOSS);
	pSion->Compute_CurrentIndex(LEVEL_BOSS);
	pSion->Check_Navigation();
	pSion->Change_Level(LEVEL_BOSS);
	pSion->Set_IsActionMode(true);
	pSion->Set_BattlePose(false);

	CPlayer* pRinwell = CPlayerManager::Get_Instance()->Get_EnumPlayer(2);
	pRinwell->Set_State(CTransform::STATE_TRANSLATION, vPosition[1]);
	pRinwell->Change_Navigation(LEVEL_BOSS);
	pRinwell->Compute_CurrentIndex(LEVEL_BOSS);
	pRinwell->Check_Navigation();
	pRinwell->Change_Level(LEVEL_BOSS);
	pRinwell->Set_IsActionMode(true);
	pRinwell->Set_BattlePose(false);

	CPlayer* pLaw = CPlayerManager::Get_Instance()->Get_EnumPlayer(3);
	pLaw->Set_State(CTransform::STATE_TRANSLATION, vPosition[2]);
	pLaw->Change_Navigation(LEVEL_BOSS);
	pLaw->Compute_CurrentIndex(LEVEL_BOSS);
	pLaw->Check_Navigation();
	pLaw->Change_Level(LEVEL_BOSS);
	pLaw->Set_IsActionMode(true);
	pLaw->Set_BattlePose(false);

	

	CPlayerManager::Get_Instance()->Get_EnumPlayer(4)->Set_IsActionMode(false);   //Set_IsActionMode
	CPlayerManager::Get_Instance()->Get_EnumPlayer(5)->Set_IsActionMode(false);

	CPlayerManager::Get_Instance()->Set_BattleMode(true);
	CPlayerManager::Get_Instance()->Set_Ai_Check();
	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CLevel_BossZone::Ready_Layer_Player_NotBattle(const _tchar * pLayerTag)
{
	CPlayerManager* pPlayerManager = CPlayerManager::Get_Instance();

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	NONANIMDESC Active1;
	NONANIMDESC Active2;
	NONANIMDESC AIplayer1;
	NONANIMDESC AIplayer2;
	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Ending/BossRoom_PlayerPosition.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	ReadFile(hFile, &(Active1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(Active2), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer1), sizeof(NONANIMDESC), &dwByte, nullptr);
	ReadFile(hFile, &(AIplayer2), sizeof(NONANIMDESC), &dwByte, nullptr);
	CloseHandle(hFile);

	CPlayer* Alphen = pPlayerManager->Get_EnumPlayer(CPlayer::ALPHEN);
	CPlayer* Sion = pPlayerManager->Get_EnumPlayer(CPlayer::SION);
	CPlayer* Rinwell = pPlayerManager->Get_EnumPlayer(CPlayer::RINWELL);
	CPlayer* Law = pPlayerManager->Get_EnumPlayer(CPlayer::LAW);

	Alphen->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active1.vPosition), 1.f));
	Alphen->Get_Transform()->Set_Rotation(Active1.vRotation);
	Sion->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&Active2.vPosition), 1.f));
	Sion->Get_Transform()->Set_Rotation(Active2.vRotation);
	Rinwell->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer1.vPosition), 1.f));
	Rinwell->Get_Transform()->Set_Rotation(AIplayer1.vRotation);
	Law->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMLoadFloat3(&AIplayer2.vPosition), 1.f));
	Law->Get_Transform()->Set_Rotation(AIplayer2.vRotation);

	Alphen->Set_IsActionMode(true);
	Sion->Set_IsActionMode(true);
	Rinwell->Set_IsActionMode(true);
	Law->Set_IsActionMode(true);

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	pPlayer->Off_IsFly();
	pPlayer->Change_Level(LEVEL_BOSS);
	pPlayer->Set_Overlimit(false);


	vector<CPlayer*> pAIPlayers = CPlayerManager::Get_Instance()->Get_AIPlayers();
	_int i = 0;
	for (auto& iter : pAIPlayers)
	{
		iter->Off_IsFly();
		iter->Set_Overlimit(false);
		iter->Change_Level(LEVEL_BOSS);
		i++;
	}

	CPlayerManager::Get_Instance()->Set_BattleMode(false);
	return S_OK;
}

HRESULT CLevel_BossZone::Ready_Layer_Monster(const _tchar * pLayerTag)
{
	CBattleManager*			pBattleManager = GET_INSTANCE(CBattleManager);
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);
	NONANIMDESC ModelDesc;

	vector<CBaseObj*>		vecAllMonster = pBattleManager->Get_AllMonster();

	for (auto& iter : vecAllMonster)
	{
		pBattleManager->Out_Monster(iter);
	}

	CObject_Pool_Manager::Get_Instance()->Reuse_Pooling_Layer(LEVEL_STATIC, TEXT("Layer_Boss"));
	CBaseObj* pBossMonsterFirst = dynamic_cast<CBaseObj*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Boss")));
	pBattleManager->Add_BattleMonster(pBossMonsterFirst);
	pBattleManager->Set_BossMonster(pBossMonsterFirst);


	pBossMonsterFirst->Set_State(CTransform::STATE_TRANSLATION, XMVectorSetW(XMVectorSet(50.f, 0.f, 60.f, 1.f), 1.f));
	dynamic_cast<CMonster*>(pBossMonsterFirst)->Compute_CurrentIndex(LEVEL_BOSS);
	dynamic_cast<CMonster*>(pBossMonsterFirst)->Set_BattleMode(true);
	dynamic_cast<CMonster*>(pBossMonsterFirst)->Set_IsActionMode(false);
	dynamic_cast<CAstralDoubt*>(pBossMonsterFirst)->Set_AmIFirstBoss(true);
	pBattleManager->Set_LackonMonster(pBossMonsterFirst);


	RELEASE_INSTANCE(CGameInstance);
	RELEASE_INSTANCE(CBattleManager);

	return S_OK;
}

HRESULT CLevel_BossZone::Ready_Layer_BackGround(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sky"), LEVEL_BOSS, pLayerTag, nullptr)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}


HRESULT CLevel_BossZone::Ready_Layer_Camera(const _tchar * pLayerTag)
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Camera_Dynamic"), LEVEL_BOSS, pLayerTag, &CameraDesc)))
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

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_CameraAction"), LEVEL_BOSS, pLayerTag, &ActionCameraDesc)))
		return E_FAIL;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}


HRESULT CLevel_BossZone::Ready_Layer_Battle_UI(const _tchar * pLayerTag)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Alpha_Screen"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	_int numcreate = (_int)(CPlayerManager::Get_Instance()->Get_AIPlayers().size() + 2);
	if (numcreate >= 5)
		numcreate = 5;
	for (int i = 0; i < numcreate; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPbar"), LEVEL_BOSS, pLayerTag, &i)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_HPfont"), LEVEL_BOSS, pLayerTag, &i)))
			return E_FAIL;

	}



	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Comboline"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Damagesfont"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_font"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;



	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_HITS_fontnum"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_DAMAGES_fontnum"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MPguage"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitback"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_top"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_right"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_bottom"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_left"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_sub1"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Combo_Portraitfront_sub2"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_button"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_SKill_change_button"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_LOCKON"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_Skillmsg"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_MonsterHP"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;


	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_ScreenFadeEffect"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage"), LEVEL_BOSS, pLayerTag)))
		return E_FAIL;

	for (int i = 0; i < 7; ++i)
	{
		_uint number = i;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_CPguage_font"), LEVEL_BOSS, pLayerTag, &i)))
			return E_FAIL;


	}


	if (CBattleManager::Get_Instance()->Get_IsBossBattle())
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_UI_BossMonsterHP"), LEVEL_BOSS, pLayerTag)))
			return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}



CLevel_BossZone * CLevel_BossZone::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_BossZone*	pInstance = new CLevel_BossZone(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		ERR_MSG(TEXT("Failed to Created : CLevel_BossZone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_BossZone::Free()
{
	__super::Free();

	//CBattleManager::Get_Instance()->Clear_Monster();
	Safe_Release(m_pCollision_Manager);

	//CGameInstance::Get_Instance()->StopSound(SOUND_SYSTEM);

}
