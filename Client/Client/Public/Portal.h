#pragma once

#include "NonAnim.h"


BEGIN(Client)

class CPortal final : public CNonAnim
{
public:
	enum TYPE { PORTAL_POSITION , PORTAL_LEVEL};
	enum ROOMTYPE { MARINHOUSE, SHOP, CRANE_GAME , TELEPHONE, ROOM_END};

	typedef struct Portaltag
	{
		TYPE	ePortalType = PORTAL_POSITION;
		_float3 vInitPos = _float3(0.f, 0.f, 0.f);
		_float3 vConnectPos = _float3(0.f, 0.f, 0.f);
		LEVEL   eConnectLevel = LEVEL_END;
		ROOMTYPE eRoomType = ROOM_END;
		_bool bConnectPortal2D = false;
	}PORTALDESC;

private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& rhs);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	void Portal_Position_Tick(_float fTimeDelta);
	void Portal_Level_Tick(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg) override;

private:
	PORTALDESC  m_PortalDesc;


public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END