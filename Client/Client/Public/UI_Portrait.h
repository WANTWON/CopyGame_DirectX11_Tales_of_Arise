#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Portrait final : public CUI_Base
{
private:
	CUI_Portrait(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Portrait(const CUI_Portrait& rhs);
	virtual ~CUI_Portrait() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CUI_Portrait* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END