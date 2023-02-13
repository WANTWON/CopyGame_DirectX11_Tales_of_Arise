#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_font_Damage_number final : public CUI_Base
{
private:
	CUI_font_Damage_number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_font_Damage_number(const CUI_font_Damage_number& rhs);
	virtual ~CUI_font_Damage_number() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */


public:
	static CUI_font_Damage_number* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:

	void updatedamage(_uint damage);
	
	void sizedown() { m_fSize.x -= 0.4f;	m_fSize.y -= 0.4f; }



private:
	CTexture*				m_pTextureCom1 = nullptr;
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iCurrentDamage = 0;

	_bool m_bsizedown = false;

	_bool m_bRender = true;

	_float m_fbrightpos_damagefont[6] = { 0.f , -0.5f, -1.f, -1.5f , -2.f , - 2.5f }; 
	_float m_fOffsetX = 0.f;

	_bool m_bfadein = true;
	_bool m_bfadeout = false;
	_float m_fDietimer = 0.f;

	
};

END