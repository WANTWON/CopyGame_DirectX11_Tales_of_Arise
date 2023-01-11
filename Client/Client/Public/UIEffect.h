#pragma once
#include "Obj_UI.h"

BEGIN(Client)

class CUIEffect final : public CObj_UI
{
public:
	enum EFFECTTID { COMPOSS_RING};

	typedef struct EffectTag
	{
		EFFECTTID	   eEffectID = COMPOSS_RING;
		_uint	   iTextureNum = 0;

		_float2	   vInitPositon = _float2(0.f, 0.f);
		_float2	   vInitScale = _float2(0.f, 0.f);
		_vector	   vColor = XMVectorSet(255.f, 255.f, 255.f, 255.f);

		_float	   fDeadTime = 0.f;
		_float	   fStartTime = 0.f;

		CObj_UI*  pTarget = nullptr;
	}EFFECTDESC;

private:
	CUIEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIEffect(const CUIEffect& rhs);
	virtual ~CUIEffect() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components(void * pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources()override;  /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

private:
	void Tick_CompossRing(_float fTimeDelta);

private:
	EFFECTDESC  m_EffectDesc;
	_float		m_fAlpha = 0.0f;
	_float		m_fDeadtime = 0.f;

	_vector					m_vColorBack = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	_vector					m_vColorFront = XMVectorSet(1.f, 1.f, 1.f, 1.f);
	
public:
	static CUIEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END