#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)

class CModel;

END


BEGIN(Client)

class CMonster final : public CBaseObj
{
private:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components(void * pArg);
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	CModel*					m_pModelCom = nullptr;

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END