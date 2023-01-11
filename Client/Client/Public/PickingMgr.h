#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
class CPickingMgr : public CBase
{
	DECLARE_SINGLETON(CPickingMgr)
private:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	void	Clear_PickingMgr();

public:
	void	Add_PickingGroup(CGameObject* pGameObject);
	void	Out_PickingGroup(CGameObject* pGameObject);
	void	Mouse_Intersect_UI(_bool Data) {m_bMouseInUI = Data;}

public: /*Get*/
	_float3 Get_PickingPos() { return m_vPickingPos; }

public:
	_bool	Picking();

private:
	list<CGameObject*>	m_GameObjects;
	_bool				m_bMouseInUI = false;
	_float3				m_vPickingPos;

public:
	virtual void Free() override;
};
END
