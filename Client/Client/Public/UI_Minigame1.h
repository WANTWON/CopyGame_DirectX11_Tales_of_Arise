#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Minigame1 final : public CUI_Base
{
private:
	CUI_Minigame1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame1(const CUI_Minigame1& rhs);
	virtual ~CUI_Minigame1() = default;


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
	CTexture*				m_pTextureCom2 = nullptr;

	CTexture*               m_pTextureCom3 = nullptr;
	CTexture*               m_pTextureCom4 = nullptr;






public:
	static CUI_Minigame1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iScore = 0;
	_float m_fGametime = 0.f;

	_bool m_bfadein = true;


	_float m_fStart_timer = 0.f;
	_float m_fNext = 20.f;

	_bool m_bRenderoff = false;

	_bool m_bGameStart = false;
	
	_bool m_bGameStartUI = false;

	_float m_fScaler = 0.1f;
	_float m_fStopTimer = 0.f;

	_int m_iStartEndtexnum = 0;
	_float m_fbrightpos_hp[4] = { 0.f , -0.5f, -1.f, -1.5f };
	
	//Sound
	_bool m_bMiniGameSound_End = false;

};

END