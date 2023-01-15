#pragma once

#include "UIBase.h"


BEGIN(Client)

class CScreenFadeEffect final : public CUI_Base
{
private:
	CScreenFadeEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreenFadeEffect(const CScreenFadeEffect& rhs);
	virtual ~CScreenFadeEffect() = default;


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

	_float m_ftimer = 0.f;

	//_float m_falpha = 0.f;
	_bool m_bin = false;

	

public:

	typedef struct tagScreenfadedesc
	{
		_bool bin = false;

	}SCREENFADE_DESC;

private:
	SCREENFADE_DESC m_ScreenEffectdesc;


public:
	static CScreenFadeEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END