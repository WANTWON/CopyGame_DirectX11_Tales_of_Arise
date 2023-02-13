#include "stdafx.h"
#include "PlayerCreater.h"

#include "GameInstance.h"
#include "Camera_Dynamic.h"
#include "Player.h"
#include "SnowFieldNpc.h"
#include "BattleManager.h"
#include "TreasureBox.h"
#include "Item.h"

CPlayerCreater::CPlayerCreater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
} 

unsigned int APIENTRY Thread_Clone(void* pArg)
{
	CPlayerCreater* pLoader = (CPlayerCreater*)pArg;

	EnterCriticalSection(&pLoader->Get_CriticalSection());

	switch (pLoader->Get_CreateActorType())
	{
	case CLONE_PLAYER:
		pLoader->Cloning_ForNpc();
		pLoader->Cloning_ForPlayer();
		break;
	case CLONE_MONSTER1:
		pLoader->Cloning_ForMonster();
		break;
	case CLONE_NPC:
		pLoader->Cloning_ForNpc();
		break;
	case CLONE_NONANIM:
		pLoader->Cloning_ForNonAnim();
		break;
	}

	LeaveCriticalSection(&pLoader->Get_CriticalSection());

	return 0;
}

HRESULT CPlayerCreater::Initialize(THREAD_ID eThreadID)
{
	CoInitializeEx(nullptr, 0);

	m_eCloneType = eThreadID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Clone, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;
		
	return S_OK;
}


HRESULT CPlayerCreater::Cloning_ForPlayer()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
	if (pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")) == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Alphen"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Sion"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
			return E_FAIL;

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Get_Object(LEVEL_STATIC, TEXT("Layer_Player")));
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(44, 0, 22, 1.f));
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(150, 0, 150, 1.f));
		CPlayerManager::Get_Instance()->Set_ActivePlayer(pPlayer);
		pPlayer->Change_Level(LEVEL_SNOWFIELD);
	}
	else
	{
		/*if (CPlayerManager::Get_Instance()->Get_PlayerEnum(CPlayerManager::RINWELL) == nullptr)
		{
			vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
			for (auto& iter : vecFightedMonster)
			{
				if (iter == RINWELL)
				{
					if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Rinwell"), LEVEL_STATIC, TEXT("Layer_Player"), nullptr)))
						return E_FAIL;
				}

			}
		}*/
		
		

		CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
		pPlayer->Set_State(CTransform::STATE_TRANSLATION, CPlayerManager::Get_Instance()->Get_LastPosition());
		pPlayer->Change_Navigation(LEVEL_SNOWFIELD);
		pPlayer->Compute_CurrentIndex(LEVEL_SNOWFIELD);
		pPlayer->Check_Navigation();
		pPlayer->Change_Level(LEVEL_SNOWFIELD);
	}

	m_isFinished = true;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CPlayerCreater::Cloning_ForNpc()
{
	CGameInstance*			pGameInstance = GET_INSTANCE(CGameInstance);

	HANDLE hFile = 0;
	_ulong dwByte = 0;
	CNpc::NPCDESC NpcDesc;

	_uint iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Npc.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; ++i)
	{
		ReadFile(hFile, &(NpcDesc.Modeldesc), sizeof(NONANIMDESC), &dwByte, nullptr);
		_tchar pModeltag[MAX_PATH];
		MultiByteToWideChar(CP_ACP, 0, NpcDesc.Modeldesc.pModeltag, MAX_PATH, pModeltag, MAX_PATH);

		if (!wcscmp(pModeltag, TEXT("NpcFemaleYoung")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::FEMALE_YOUNG;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, TEXT("Layer_Npc"), &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMM_GLD")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_GLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, TEXT("Layer_Npc"), &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMO_DOK")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_OLD;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, TEXT("Layer_Npc"), &NpcDesc)))
				return E_FAIL;
		}
		else if (!wcscmp(pModeltag, TEXT("NPC_NMY_PLC")))
		{
			NpcDesc.eNpcType = CSnowFieldNpc::MAN_PLC;
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_SnowFieldNpc"), LEVEL_SNOWFIELD, TEXT("Layer_Npc"), &NpcDesc)))
				return E_FAIL;
		}


	}

	CloseHandle(hFile);

	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayerCreater::Cloning_ForMonster()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	if (nullptr == pGameInstance)
		return E_FAIL;
	
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

		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Trigger"), LEVEL_SNOWFIELD, TEXT("Layer_Trigger"), &ModelDesc)))
			return E_FAIL;

	}

	CloseHandle(hFile);


	 hFile = 0;
	 dwByte = 0;
	 ModelDesc;
	vector<MONSTER_ID> vecFightedMonster = CBattleManager::Get_Instance()->Get_FightedMonster();
	 iNum = 0;

	hFile = CreateFile(TEXT("../../../Bin/Data/Field_Data/Monster.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	/* 타일의 개수 받아오기 */
	ReadFile(hFile, &(iNum), sizeof(_uint), &dwByte, nullptr);

	for (_uint i = 0; i < iNum; i++)
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

			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Ice_Wolf"), LEVEL_SNOWFIELD, TEXT("Layer_Monster"), &ModelDesc)))
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
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Hawk"), LEVEL_SNOWFIELD, TEXT("Layer_Monster"), &ModelDesc)))
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
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Berserker"), LEVEL_SNOWFIELD, TEXT("Layer_Monster"), &ModelDesc)))
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
			if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_AiRinwell"), LEVEL_SNOWFIELD, TEXT("Layer_Monster"), &ModelDesc)))
				return E_FAIL;
		}

	}

	CloseHandle(hFile);


	m_isFinished = true;
	RELEASE_INSTANCE(CGameInstance);
	return S_OK;
}

HRESULT CPlayerCreater::Cloning_ForNonAnim()
{
	
	return S_OK;
}


CPlayerCreater * CPlayerCreater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, THREAD_ID eNextLevel)
{
	CPlayerCreater* pInstance = new CPlayerCreater(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		ERR_MSG(TEXT("Failed to Created : CPlayerCreater"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerCreater::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}