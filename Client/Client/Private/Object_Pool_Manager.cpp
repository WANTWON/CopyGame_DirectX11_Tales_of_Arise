#include "stdafx.h"
#include "..\Public\Object_Pool_Manager.h"

#include "GameInstance.h"
#include "Model.h"
#include <fstream>
#include <string.h>
#include "GameObject.h"


IMPLEMENT_SINGLETON(CObject_Pool_Manager)

CObject_Pool_Manager::CObject_Pool_Manager()
{
	m_pObjects = new POOLINGOJECTS[LEVEL_END];
}

void CObject_Pool_Manager::Add_Pooling_Object(LEVEL iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	//무조건 반환값을 OBJ_POOL로 할 것
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

	list< class CGameObject*>* pLayer = Find_PoolingObjects(iLevelIndex, pLayerTag);
	if(pLayer == nullptr)
	{
		pLayer = new LAYERS;
	}
	
	for (auto& iter : *pLayerList)
		pLayer->push_back(iter);

	m_pObjects[iLevelIndex].emplace(pLayerTag, pLayer);
	pGameInstance->Out_GameObjectList(iLevelIndex, pLayerTag);
	RELEASE_INSTANCE(CGameInstance);
}

_bool CObject_Pool_Manager::Reuse_Pooling_Layer(LEVEL iLevelIndex, const _tchar * pLayerTag, void* pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	list< class CGameObject*>* pLayer = Find_PoolingObjects(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}
		

	for (auto& iter = pLayer->begin(); iter != pLayer->end();)
	{
		if (pArg != nullptr)
			(*iter)->ReUse_Setting(pArg);
		pGameInstance->ReAdd_GameObject(iLevelIndex, pLayerTag, *iter);

		iter = pLayer->erase(iter);
	}

	RELEASE_INSTANCE(CGameInstance);
	return true;
}

_bool CObject_Pool_Manager::Reuse_Pooling_Object(LEVEL iLevelIndex, const _tchar * pLayerTag, void * pArg)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	list< class CGameObject*>* pLayer = Find_PoolingObjects(iLevelIndex, pLayerTag);
	if (pLayer == nullptr || pLayer->size() == 0)
	{
		RELEASE_INSTANCE(CGameInstance);
		return false;
	}



	for (auto& iter = pLayer->begin(); iter != pLayer->end();)
	{
		if (pArg != nullptr)
			(*iter)->ReUse_Setting(pArg);
		pGameInstance->ReAdd_GameObject(iLevelIndex, pLayerTag, *iter);

		iter = pLayer->erase(iter);
		break;
	}
	

	RELEASE_INSTANCE(CGameInstance);
	return true;
}

_bool CObject_Pool_Manager::Clear_AllPooling_Layer()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	for (int i = 0; i < LEVEL_END; ++i)
	{
		for (auto& iter : m_pObjects[i])
		{
			for (auto& vec = iter.second->begin(); vec != iter.second->end();)
			{
				Safe_Release(*vec);
				vec = iter.second->erase(vec);
			}
			iter.second->clear();
			Safe_Delete(iter.second);
		}
		m_pObjects[i].clear();	
	}
	Safe_Delete_Array(m_pObjects);
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



}
