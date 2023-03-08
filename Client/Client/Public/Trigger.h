#pragma once

#include "BaseObj.h"

BEGIN(Client)

class CTrigger : public CBaseObj
{
public:
	enum TYPE { MONSTER_TRIGGER, UI_TRIGGER};
	typedef struct Triggertag
	{
		NONANIMDESC m_ModelDesc;
		TYPE eType = MONSTER_TRIGGER;
		_uint iIndex = 0;

	}TRIGGERDESC; 
	

private:
	CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources();


private:
	TRIGGERDESC m_TriggerDesc;

public:
	static CTrigger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

	//Sound
	_bool m_bCrowd;
};

END