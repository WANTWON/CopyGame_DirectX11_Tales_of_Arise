
#pragma once

#include "UIBase.h"


BEGIN(Client)

class CHP_Font_World final : public CUI_Base
{
private:
	CHP_Font_World(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHP_Font_World(const CHP_Font_World& rhs);
	virtual ~CHP_Font_World() = default;


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
	static CHP_Font_World* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_iIndex = 0;
	_uint m_iYIndex = 0;

	_uint m_itexnum = 0;
	_uint m_iCurrenthp = 2357;
};

END