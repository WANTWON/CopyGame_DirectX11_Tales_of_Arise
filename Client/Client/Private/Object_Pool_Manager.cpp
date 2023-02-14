#include "stdafx.h"
#include "..\Public\Object_Pool_Manager.h"

#include "GameInstance.h"
#include "Model.h"
#include <fstream>
#include <string.h>


IMPLEMENT_SINGLETON(CObject_Pool_Manager)



CObject_Pool_Manager::CObject_Pool_Manager()
{
	m_pObjects = new POOLINGOJECTS[LEVEL_END];
}

void CObject_Pool_Manager::Add_Pooling_Object(LEVEL iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	CGameInstance::Get_Instance()->Out_GameObject(iLevelIndex, pLayerTag, pGameObject);

	list< class CGameObject*>* pLayer = Find_PoolingObjects(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
	{
		pLayer = new LAYERS;
		pLayer->push_back(pGameObject);
		m_pObjects[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		m_pObjects[iLevelIndex].find(pLayerTag)->second->push_back(pGameObject);
	}
	
}

void CObject_Pool_Manager::Add_Pooling_Layer(LEVEL iLevelIndex, const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	list< class CGameObject*>* pLayerList = pGameInstance->Get_ObjectList(iLevelIndex, pLayerTag);
	if (pLayerList == nullptr)
	{
		RELEASE_INSTANCE(CGameInstance);
		return;
	}

	list< class CGameObject*>* pLayer = new LAYERS;
	
	for (auto& iter : *pLayerList)
		pLayer->push_back(iter);

	m_pObjects[iLevelIndex].emplace(pLayerTag, pLayer);
	pGameInstance->Out_GameObjectList(iLevelIndex, pLayerTag);
	RELEASE_INSTANCE(CGameInstance);
}

_bool CObject_Pool_Manager::Reuse_Pooling_Layer(LEVEL iLevelIndex, const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	list< class CGameObject*>* pLayer = Find_PoolingObjects(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
		
	
	if (pLayer != nullptr)
	{
		for (auto& iter : *pLayer)
		{
			pGameInstance->ReAdd_GameObject(iLevelIndex, pLayerTag, iter);
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return true;
}

_bool CObject_Pool_Manager::Reuse_AllPooling_Layer()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (int i = 0; i < LEVEL_END; ++i)
	{
		for (auto& iter : m_pObjects[i])
		{
			for (auto& vec : *iter.second)
			{
				pGameInstance->ReAdd_GameObject(LEVEL_STATIC, TEXT("layer_Pooling"), vec);
			}
	
		}
	}

	RELEASE_INSTANCE(CGameInstance);
	return true;
}


list< class CGameObject*>* CObject_Pool_Manager::Find_PoolingObjects(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= LEVEL_END)
		return nullptr;

	auto	iter = find_if(m_pObjects[iLevelIndex].begin(), m_pObjects[iLevelIndex].end(), CTag_Finder(pLayerTag));
	if (iter == m_pObjects[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

_bool CObject_Pool_Manager::Find_LayerTag(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= LEVEL_END)
		return false;

	auto	iter = find_if(m_pObjects[iLevelIndex].begin(), m_pObjects[iLevelIndex].end(), CTag_Finder(pLayerTag));
	if (iter == m_pObjects[iLevelIndex].end())
		return nullptr;

	return true;
}

void CObject_Pool_Manager::Free()
{

	for (int i = 0; i < LEVEL_END; ++i)
	{
		for (auto& iter : m_pObjects[i])
		{
			for (auto& index = iter.second->begin(); index != iter.second->end();)
			{
				index = iter.second->erase(index);
			}
			iter.second->clear();
			Safe_Delete(iter.second);
		}
		m_pObjects[i].clear();
	}
	m_pObjects->clear();
	Safe_Delete_Array(m_pObjects);

}
