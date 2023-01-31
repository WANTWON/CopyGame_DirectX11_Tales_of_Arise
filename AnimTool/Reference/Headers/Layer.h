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
	_uint Get_GameObjectsLength(void) { return (_uint)m_GameObjects.size(); }
	list<class CGameObject*> Get_GameObjects(void) { return m_GameObjects; }

public:
	HRESULT Initialize(void);
	HRESULT Add_GameObject(class CGameObject* pGameObject);
	void Tick(_float fTimeDelta);
	void Late_Tick(_float fTimeDelta);

private:
	list<class CGameObject*> m_GameObjects;
	typedef list<class CGameObject*> GAMEOBJECTS;

public:
	static CLayer* Create(void);
	virtual void Free(void) override;
};

END