#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_StrikeFinish final : public CUI_Base
{
private:
	CUI_StrikeFinish(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_StrikeFinish(const CUI_StrikeFinish& rhs);
	virtual ~CUI_StrikeFinish() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	_uint m_itexnum = 0;

	_uint m_isprite_texnum = 0;

	_float m_ftimer = 0.f;

	_float m_fshowtimer = 0.f;

	_bool m_bfirst = true;

	_bool m_bfadein = false;

	_bool m_bspritemode = false;



public:
	static CUI_StrikeFinish* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
