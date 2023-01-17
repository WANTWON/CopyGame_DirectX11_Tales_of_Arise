#pragma once

#include "UIBase.h"


BEGIN(Client)

class CCP_Guage_font final : public CUI_Base
{
private:
	CCP_Guage_font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCP_Guage_font(const CCP_Guage_font& rhs);
	virtual ~CCP_Guage_font() = default;


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
	static CCP_Guage_font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void sizedown() { m_fSize.x -= 1.f;	m_fSize.y -= 1.f; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iCurrentcp = 100;
	_uint m_iMaxcp = 200;

	_bool m_bsizedown = false;

	_bool m_bRender = true;
};

END