#pragma once

#include "UIBase.h"


BEGIN(Client)

class CHP_Font final : public CUI_Base
{
private:
	CHP_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHP_Font(const CHP_Font& rhs);
	virtual ~CHP_Font() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	CTexture*				m_pTextureCom1 = nullptr;


public:
	static CHP_Font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iCurrenthp = 2000;
	_bool m_bfadein = true;

	_uint m_iCharacternum = 0;

	_float m_fStart_timer = 0.f;
	_float m_fnumberY = 0.f;
	_float m_fNext = 0.f;

	_bool m_bRenderoff = false;

	_bool m_bforMainPlayer = false;

	_uint m_iCharactername = 0;

};

END