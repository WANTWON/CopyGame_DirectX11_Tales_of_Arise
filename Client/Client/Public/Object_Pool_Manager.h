#pragma once
#include "Client_Defines.h"
#include "Base.h"


BEGIN(Engine)

class CModel;
class CGameObject;

END

BEGIN(Client)
class CObject_Pool_Manager final : public CBase
{
	DECLARE_SINGLETON(CObject_Pool_Manager)

public:
	CObject_Pool_Manager();
	virtual ~CObject_Pool_Manager() = default;

public:
	void Add_Pooling_Object(LEVEL iLevelIndex, const _tchar* pLayerTag, CGameObject* pgameObject);
	void Add_Pooling_Layer(LEVEL iLevelIndex, const _tchar* pLayerTag);

	_bool Reuse_Pooling_Layer(LEVEL iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);
	_bool Reuse_Pooling_Object(LEVEL iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);
	_bool Clear_AllPooling_Layer();
	
private:
	list< class CGameObject*>*		m_pLayers;
	typedef list< class CGameObject*> LAYERS;

	map<const _tchar*, LAYERS*>*			m_pObjects = nullptr;
	typedef map<const _tchar*, LAYERS*>		POOLINGOJECTS;

private:
	list< class CGameObject*>* Find_PoolingObjects(_uint iLevelIndex, const _tchar * pLayerTag);
	_bool Find_LayerTag(_uint iLevelIndex, const _tchar * pLayerTag);

public:
	virtual void Free() override;
};

END

