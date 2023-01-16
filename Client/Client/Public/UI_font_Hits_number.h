#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_font_Hits_number final : public CUI_Base
{
private:
	CUI_font_Hits_number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_font_Hits_number(const CUI_font_Hits_number& rhs);
	virtual ~CUI_font_Hits_number() = default;


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
	static CUI_font_Hits_number* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void sizedown() { m_fSize.x -= 1.f;	m_fSize.y -= 1.f; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iCurrenthit = 0;

	_bool m_bsizedown = false;

	_bool m_bRender = true;
};

END