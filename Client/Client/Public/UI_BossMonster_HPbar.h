#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_BossMonster_HPbar final : public CUI_Base
{
private:
	CUI_BossMonster_HPbar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BossMonster_HPbar(const CUI_BossMonster_HPbar& rhs);
	virtual ~CUI_BossMonster_HPbar() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT RenderCurrenthpfont();

	HRESULT RenderMaxhpfont();

	HRESULT RenderCurrenthpfont1();

	HRESULT RenderMaxhpfont1();

	void calculateboss1();
	void calculateboss2();



private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	CTexture*				m_pTextureCom1 = nullptr;
	CTexture*				m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;

	_float 	m_fcurrenthp = 10000.f;
	_float m_fmaxhp = 10000.f;

	_uint m_iIndex = 0;
	_uint m_itexnum = 0;

	_bool m_bforMainPlayer = false;
	_float m_fNext = 20.f;

	_float m_fbrightpos_hp[6] = { 0.f , -0.5f, -1.f, -1.5f , -2.f,-2.5f };

	_uint m_iMonstername = 0;

	_uint m_iHPbarindex = 8;

	_float m_fHPbarvalue[8] = { 0.f,0.f,0.f,0.f,0.f,0.f,0.f,0.f };

	_float m_fHPcurbarvalue[4] = { 0.f,0.f,0.f,0.f };


	_float m_fMinusHp = 0.f;
	_float m_fCurrentbar = 0.f;

	_float m_fYOffset = 0.f;
	_bool m_bChangetoboss = false;
	_bool m_bLastsecondboss = false;
	_float 	m_fcurrenthp1 = 10000.f;
	_float m_fmaxhp1 = 10000.f;
	_uint m_iMonstername1 = 0;
	_float m_fMinusHp1 = 0.f;
	_float m_fCurrentbar1 = 0.f;
	_int m_iHPbarindex1 = 0;
	_bool m_bnomoreupdate = false;


public:
	static CUI_BossMonster_HPbar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END