#pragma once

#include "UIBase.h"

BEGIN(Client)
class CScreenDistortion final : public CUI_Base
{
private:
	CScreenDistortion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreenDistortion(const CScreenDistortion& rhs);
	virtual ~CScreenDistortion() = default;

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
	void Copy_BackBuffer();

private:
	ID3D11Texture2D* m_pBackBufferTextureCopy = nullptr;
	ID3D11ShaderResourceView* m_pBackBufferSRV = nullptr;

	_float m_fTimer = 0.f;

public:
	static CScreenDistortion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};
END