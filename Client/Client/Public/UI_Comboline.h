#pragma once

#include "UIBase.h"


BEGIN(Client)

class CUI_Comboline final : public CUI_Base
{
private:
	CUI_Comboline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Comboline(const CUI_Comboline& rhs);
	virtual ~CUI_Comboline() = default;


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
	static CUI_Comboline* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	void setline();


	//void moveleft() { m_fPosition.x -= 10.f; }

private:
	_bool m_bfadein = false;
	_bool m_bfadeout = false;

	_float m_fdietimer = false;

	_float m_fOffsetX = 0.f;
	_float m_fDietimer = 0.f;


};

END