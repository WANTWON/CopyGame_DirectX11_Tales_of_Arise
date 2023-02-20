#pragma once

#include "UIBase.h"


BEGIN(Client)


class CUI_LOCKON final : public CUI_Base
{
private:
	CUI_LOCKON(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_LOCKON(const CUI_LOCKON& rhs);
	virtual ~CUI_LOCKON() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Glow();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

									 //CVIBuffer_Point*			m_pVIBufferCom1 = nullptr;

private:
	_float4 m_vRight;
	_float4 m_vUp;

public:
	static CUI_LOCKON* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_itexnum = 0;
	_float m_fcurrentmp = 0.f;
	_float m_fmaxmp = 4.f;

	_float m_fNext = 50.f;


	_float m_fcurrent_render_slot_mp = 0.f;

	_float2 Sprite = { 4.f,2.f };
	_float speed = 0.f;
	_float time = 0.f;

	_bool m_bStrikeon = false;
	_bool m_bStrikefonton = false;

	_bool m_bStrikeSmallonetime = true;

	_bool m_bRenderDiamond = true;

	_float m_bStrikeScaler = 10.f;
	_float m_bStrikeFakeScaler = 1.f;
	_float m_fLockonScaler = 10.f;

	_float m_fDiamondShooter = 1.f;

	_float m_fAlphaDiamond = 1.f;

	_float m_fAlphaNomal = 1.f;

	_bool m_bEffectSpawned = false;

	_float m_fTimeOffset = 1.f;

	_float2 m_fGlowPosition[2] = { { 0.f, 0.f }  , {0.f,0.f} };
	_float2 m_fGlowSize[2] = { { 0.f, 0.f }  ,{ 0.f,0.f } };

	_float m_fGlowtimer = 0.f;




};

END