#include "..\Public\Layer.h"


CLayer::CLayer()
{
}

CComponent * CLayer::Get_Component(const _tchar * pComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Find_Component(pComponentTag);	
}

CGameObject * CLayer::Get_Object(_uint iIndex)
{
	if (m_GameObjects.size() <= iIndex)
		return nullptr;

	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; ++i)
		++iter;

	return *iter;
}

HRESULT CLayer::Initialize(_uint iIndex)
{
	m_iLevel = iIndex;
	return S_OK;
}

HRESULT CLayer::Add_GameObject(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_GameObjects.push_back(pGameObject);

	return S_OK;
}

HRESULT CLayer::Out_GameObject(CGameObject * pGameObject)
{
	for (auto& iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if ((*iter) == pGameObject)
		{
			iter = m_GameObjects.erase(iter);
			return S_OK;
		}
		else
			iter++;
	}

	return S_OK;
}

void CLayer::Tick(_float fTimeDelta)
{
	for (auto& iter = m_GameObjects.begin(); iter != m_GameObjects.end();)
	{
		if ((*iter) != nullptr)
		{
			int iEvent = (*iter)->Tick(fTimeDelta);
			if (iEvent == OBJ_DEAD)
			{
				Safe_Release(*iter);
				iter = m_GameObjects.erase(iter);
			}
			else
				iter++;
		}
	}
}

void CLayer::Late_Tick(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Tick(fTimeDelta);
	}
}

CLayer * CLayer::Create(_uint iIndex)
{
	CLayer*	pInstance = new CLayer();

	if (FAILED(pInstance->Initialize(iIndex)))
	{
		ERR_MSG(TEXT("Failed to Created : CLayer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}

