#pragma once

#include "BaseObj.h"

BEGIN(Client)
class CEffect;
class CPortal : public CBaseObj
{
public:
	typedef struct Portaltag
	{
		NONANIMDESC m_ModelDesc;
		_uint iNextLevel = 0;
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

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources();
private:
	PORTALDESC m_PortalDesc;
	vector<class CEffect*> m_pEffects;
	_bool m_bFirst = false;

public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END