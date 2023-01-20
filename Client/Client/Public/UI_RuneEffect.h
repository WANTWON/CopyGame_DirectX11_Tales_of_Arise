#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_RuneEffect final : public CUI_Base
{
private:
	CUI_RuneEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_RuneEffect(const CUI_RuneEffect& rhs);
	virtual ~CUI_RuneEffect() = default;


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
	static CUI_RuneEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

//public:
//	void moveleft() { m_fPosition.x -= 10.f; }

private:
	CTexture*				m_pTextureCom1 = nullptr;
	_bool m_bfadein = false;
	_bool m_bfadeout = false;
	_bool m_bfadein1 = false;
	_bool m_bfadeout1 = false;
	_bool m_bfadein2 = false;
	_bool m_bfadeout2 = false;

	_float m_fDeadtimer = 0.f;

	_float  m_fAlpha1 = 1.f;
	_float  m_fAlpha2 = 1.f;

	/*_bool m_bfirst = true;
	_bool m_bsecond = true;*/

public:
	typedef struct tagRuneEffectdesc
	{
		_float2 position = { 0.f,0.f };
		_uint m_etype = 0;

	}RUNEDESC;

private:
	RUNEDESC runedesc;

};

END