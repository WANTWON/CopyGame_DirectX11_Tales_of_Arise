 #pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Monster_HPbar final : public CUI_Base
{
private:
	CUI_Monster_HPbar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Monster_HPbar(const CUI_Monster_HPbar& rhs);
	virtual ~CUI_Monster_HPbar() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT RenderCurrenthpfont();

	HRESULT RenderMaxhpfont();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;

	_float 	m_fcurrenthp = 2000.f;
	_float m_fmaxhp = 2000.f;

	_uint m_iIndex = 0;
	_uint m_itexnum = 0;

	_bool m_bforMainPlayer = false;
	_float m_fNext = 20.f;

	_float m_fbrightpos_hp[6] = { 0.f , -0.5f, -1.f, -1.5f , -2.f,-2.5f};

	_uint m_iMonstername = 0;

public:
	static CUI_Monster_HPbar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END