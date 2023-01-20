#include "stdafx.h"
#include "..\Public\Terrain_Manager.h"
#include "GameInstance.h"
#include "PickingMgr.h"
IMPLEMENT_SINGLETON(CTerrain_Manager)

CTerrain_Manager::CTerrain_Manager()
{

}

CVIBuffer_Terrain* CTerrain_Manager::Create_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, HANDLE hFile, _ulong& dwByte)
{
	return CVIBuffer_Terrain::Create(pDevice, pContext, hFile, dwByte);
}

CVIBuffer_Terrain * CTerrain_Manager::Create_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pHeightMapFilePath)
{
	return CVIBuffer_Terrain::Create(pDevice, pContext, pHeightMapFilePath);
}

void CTerrain_Manager::Out_DebugTerrain()
{
	if (m_pTerrain == nullptr)
		return;

	CPickingMgr::Get_Instance()->Out_PickingGroup(m_pTerrain);
	m_pTerrain->Set_Dead(true);
	m_pTerrain = nullptr;
}

HRESULT CTerrain_Manager::Create_Terrain(LEVEL eLevel, const _tchar* pLayerTag, const _tchar* pTerrainTag)
{

	CGameInstance*			pGameInstance = CGameInstance::Get_Instance();
	Safe_AddRef(pGameInstance);

	TERRAINDESC TerrainDesc = m_pTerrainDesc;
	TerrainDesc.m_eDebugTerrain = DEBUG_NONE;

	if (pTerrainTag == nullptr)
	{
		if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_Terrain"), eLevel, pLayerTag, &TerrainDesc)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(pGameInstance->Add_GameObjectLoad(TEXT("Prototype_GameObject_Terrain"), eLevel, pLayerTag, pTerrainTag, nullptr)))
			return E_FAIL;
	}
		
	Safe_Release(pGameInstance);

	return S_OK;
}

void CTerrain_Manager::Add_PrototypeTag(_tchar * TempTag)
{
	_tchar*			pModeltag = new _tchar[MAX_PATH];
	wsprintf(pModeltag, TempTag);
	m_pPrototypeTags.push_back(pModeltag);
}

void CTerrain_Manager::Free()
{
	m_pPrototypeTags.clear();

	m_pCloneTerrainTags.clear();
}
