#pragma once

#include "GameObject.h"

/* 객체들을 모아 놓는다. */
BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CComponent* Get_Component(const _tchar* pComponentTag, _uint iIndex = 0);
	class CGameObject* Get_Object(_uint iIndex = 0);
	list<class CGameObject*>*		Get_ObjectList() { return &m_GameObjects; };

public:
	HRESULT Initialize(_uint iLevelIndex);
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	HRESULT Out_GameObject(class CGameObject* pGameObject);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);
	
private:
	list<class CGameObject*>			m_GameObjects;
	typedef list<class CGameObject*>	GAMEOBJECTS;
	_uint m_iLevel = 0;

public:
	static CLayer* Create(_uint iLevelIndex);
	virtual void Free() override;
};

END