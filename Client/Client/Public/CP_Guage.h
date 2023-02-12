#pragma once

#include "UIBase.h"


BEGIN(Client)

class CCP_Guage final : public CUI_Base
{
private:
	CCP_Guage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCP_Guage(const CCP_Guage& rhs);
	virtual ~CCP_Guage() = default;


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

	_uint 	m_fcurrentcp = 200;
	_uint   m_fmaxcp = 200;

	_uint m_iIndex = 0;

public:
	static CCP_Guage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END