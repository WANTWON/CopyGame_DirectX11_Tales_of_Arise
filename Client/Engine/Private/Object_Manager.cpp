#include "..\Public\Object_Manager.h"
#include "Layer.h"


IMPLEMENT_SINGLETON(CObject_Manager)

CObject_Manager::CObject_Manager()
{
}

CComponent * CObject_Manager::Get_Component(_uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pComponentTag, _uint iIndex)
{
	CLayer*		pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(pComponentTag, iIndex);	
}

HRESULT CObject_Manager::Reserve_Container(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_pLayers = new LAYERS[iNumLevels];

	m_iNumLevels = iNumLevels;

	return S_OK;
}

HRESULT CObject_Manager::Add_Prototype(const _tchar * pPrototypeTag, CGameObject * pPrototype)
{
	if (nullptr != Find_Prototype(pPrototypeTag))
		return E_FAIL;

	m_Prototypes.emplace(pPrototypeTag, pPrototype);	

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, void* pArg)
{
	CGameObject*		pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*		pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*			pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(iLevelIndex);
		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}



	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_Out(const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, OUT CGameObject *& pGameObjectOut, void * pArg)
{
	CGameObject* pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*	pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (!pLayer)
	{
		pLayer = CLayer::Create(iLevelIndex);
		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	pGameObjectOut = pGameObject;

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObjectLoad(const _tchar * pPrototypeTag, _uint iLevelIndex, const _tchar * pLayerTag, const _tchar * pVIBufferTag, void * pArg)
{
	CGameObject*		pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject*		pGameObject = pPrototype->Clone_Load(pVIBufferTag, pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer*			pLayer = Find_Layer(iLevelIndex, pLayerTag);

	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(iLevelIndex);
		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
	}
	else
	{
		pLayer->Add_GameObject(pGameObject);
	}



	return S_OK;
}

CGameObject * CObject_Manager::Clone_GameObject(const _tchar * pPrototypeTag, void* pArg)
{
	CGameObject*		pPrototype = Find_Prototype(pPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	return pPrototype->Clone(pArg);	
}

void CObject_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);			
		}
	}
}

void CObject_Manager::Late_Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Tick(fTimeDelta);
		}
	}
}

CGameObject * CObject_Manager::Get_Objects(_uint iLevelIndex, const _tchar * pLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
		return nullptr;

	return pLayer->Get_Object(iIndex);
}

list<CGameObject*>* CObject_Manager::Get_ObjectList(_uint iSceneID, const _tchar * pLayerTag)
{
	CLayer* pLayer = Find_Layer(iSceneID, pLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_ObjectList();
}

HRESULT CObject_Manager::Out_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	return pLayer->Out_GameObject(pGameObject);
}

HRESULT CObject_Manager::Out_GameObjectList(_uint iLevelIndex, const _tchar * pLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
		return E_FAIL;

	return pLayer->Out_GameObjectList();
}



HRESULT CObject_Manager::ReAdd_GameObject(_uint iLevelIndex, const _tchar * pLayerTag, CGameObject * pGameObject)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(iLevelIndex);
		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(pLayerTag, pLayer);
		return S_OK;
	}

	return pLayer->Add_GameObject(pGameObject);
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels || 
		nullptr == m_pLayers)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])	
		Safe_Release(Pair.second);

	m_pLayers[iLevelIndex].clear();
	
}

void CObject_Manager::Clear_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, pLayerTag);
	if (pLayer == nullptr)
		return;

	pLayer->Free();
}

CGameObject * CObject_Manager::Find_Prototype(const _tchar * pPrototypeTag)
{
	auto	iter = find_if(m_Prototypes.begin(), m_Prototypes.end(), CTag_Finder(pPrototypeTag));

	if (iter == m_Prototypes.end())
		return nullptr;

	return iter->second;
}

CLayer * CObject_Manager::Find_Layer(_uint iLevelIndex, const _tchar * pLayerTag)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = find_if(m_pLayers[iLevelIndex].begin(), m_pLayers[iLevelIndex].end(), CTag_Finder(pLayerTag));
	if (iter == m_pLayers[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

void CObject_Manager::Free()
{
	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pLayers[i])	
			Safe_Release(Pair.second);

		m_pLayers[i].clear();	
	}

	for (auto& Pair : m_Prototypes)
		Safe_Release(Pair.second);

	m_Prototypes.clear();


	Safe_Delete_Array(m_pLayers);

}
