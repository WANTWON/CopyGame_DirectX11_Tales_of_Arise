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
	virtual HRESULT Render_Glow();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources();

private:
	_float2 m_vSprite = { 4.f, 4.f };
	_float m_fTimer = 0.f;
	_float m_fDuration = .75f;

public:
	static CScreenFadeEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END