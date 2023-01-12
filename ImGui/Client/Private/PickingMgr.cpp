#include "PickingMgr.h"
#include "GameInstance.h"
#include "Transform.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CPickingMgr)


CPickingMgr::CPickingMgr()
{
}


void CPickingMgr::Clear_PickingMgr()
{
	m_GameObjects.clear();
}


void CPickingMgr::Add_PickingGroup(CGameObject * pGameObject)
{
	if (nullptr == pGameObject)
		return;
	m_GameObjects.push_back(pGameObject);
}

void CPickingMgr::Out_PickingGroup(CGameObject * pGameObject)
{
	auto& iter = m_GameObjects.begin();
	while (iter != m_GameObjects.end())
	{
		if (*iter == pGameObject)
			iter = m_GameObjects.erase(iter);
		else
			++iter;
	}
}

void CPickingMgr::Set_PickedObj(CGameObject * pGameObject)
{
	if (pGameObject == nullptr)
	{
		if(m_pPickedObj != nullptr)	
			m_pPickedObj->Set_Picked(false);
		m_pPickedObj = nullptr;
		return;
	}
		
	m_pPickedObj = pGameObject;

	for (auto& pGameObject : m_GameObjects)
		pGameObject->Set_Picked(false);

	m_pPickedObj->Set_Picked(true);
}

_bool CPickingMgr::Picking()
{
	if (m_bMouseInUI)
		return false;

	list<CGameObject*> vecPicked;
	vector<_float3>		vecPos;
	_float3 vPosition;

	for (auto& pGameObject : m_GameObjects)
	{
		if (pGameObject->Picking(&vPosition) == true)
		{
			vecPicked.push_back(pGameObject);
			vecPos.push_back(vPosition);
			m_vPickingPos = vPosition;

			return true;
		}
	}

	if (!vecPicked.empty()) 
	{
		vecPicked.sort([](CGameObject* pSour, CGameObject* pDest)
		{
			return pSour->Get_CamDistance() < pDest->Get_CamDistance();
		});

		vecPicked.front()->PickingTrue();
		return true;
	}
	return false;
}



void CPickingMgr::Free()
{
	m_GameObjects.clear();
}
