#pragma once

#include "NonAnim.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CCollapseTile final : public CNonAnim
{
public:
	enum STATE { COLLAPSE, CRANE_TILE };

	typedef struct TileTag
	{
		_float3 vInitPosition = _float3(0.f, 0.f, 0.f);
		STATE  eTileType = COLLAPSE;
	}TILEDESC;

private:
	CCollapseTile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollapseTile(const CCollapseTile& rhs);
	virtual ~CCollapseTile() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	CBaseObj* Get_CollisionObj() { return m_pCollisionObj; }
	void Set_Check(_bool type) { m_bCheck = type; }


private:
	virtual HRESULT Ready_Components(void* pArg) override;
	void CollapseTile_Tick(_float fTimeDelta);
	void CraneTile_Tick(_float fTimeDelta);

private:
	TILEDESC m_TileDesc;
	_vector					m_vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	CNavigation*			m_pNavigationCom = nullptr;
	CBaseObj*				m_pCollisionObj = nullptr;
	_bool					m_bCheck = false;
	_bool					m_bFirst = false;
public:
	static CCollapseTile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END