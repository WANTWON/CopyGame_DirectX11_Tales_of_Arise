#pragma once

#include "UIBase.h"
BEGIN(Engine)
class CVIBuffer_Point;
END

BEGIN(Client)


class CMP_Guage final : public CUI_Base
{
private:
	CMP_Guage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMP_Guage(const CMP_Guage& rhs);
	virtual ~CMP_Guage() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg) override;
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

	//CVIBuffer_Point*			m_pVIBufferCom1 = nullptr;

private:
	_float4 m_vRight;
	_float4 m_vUp;

public:
	static CMP_Guage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;

private:
	_uint m_itexnum = 0;
	_float m_fcurrentmp = 0.f;
	_float m_fmaxmp = 10.f;

	_float m_fNext = 13.f;

	
	_tchar					m_szFPS[MAX_PATH] = TEXT("");


	_float m_fcurrent_render_slot_mp = 0.f;



};

END