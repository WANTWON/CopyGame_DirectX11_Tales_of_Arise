#pragma once

#include "UIBase.h"


BEGIN(Client)

class CHPbar_World final : public CUI_Base
{
private:
	CHPbar_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHPbar_World(const CHPbar_World& rhs);
	virtual ~CHPbar_World() = default;


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
	//CTexture*				m_pTextureCom2 = nullptr;

	_float 	m_fcurrenthp = 200.f;
	_float m_fmaxhp = 200.f;

	_uint m_iIndex = 0;

	_float m_fWorldsizeX, m_fWorldsizeY = 0.f;

public:
	static CHPbar_World* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END