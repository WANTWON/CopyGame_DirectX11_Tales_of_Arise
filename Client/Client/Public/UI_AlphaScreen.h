#pragma once

#include "UIBase.h"

BEGIN(Client)
class CUI_AlphaScreen final : public CUI_Base
{
private:
	CUI_AlphaScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_AlphaScreen(const CUI_AlphaScreen& rhs);
	virtual ~CUI_AlphaScreen() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources();

private:
	_float2 m_vSprite = { 4.f, 4.f };
	_float m_fTimer = 0.f;
	_float m_fDuration = .75f;

public:
	static CUI_AlphaScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END