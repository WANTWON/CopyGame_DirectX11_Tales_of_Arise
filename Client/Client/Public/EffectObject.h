#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
class CTexture;
END


BEGIN(Client)

class CEffectObject : public CBaseObj
{
public:
	enum TYPE { APPLE_QUARTER, BREAD_SLASH };

	typedef struct Bullettag
	{
		_float	   fDeadTime = 10.f;
		_float	   fVelocity = 5.f;
		_vector	   vTargetDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_vector	   vTargetPosition = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		NONANIMDESC				m_ModelDesc;

	}EFFECTDESC;

protected:
	CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectObject(const CEffectObject& rhs);
	virtual ~CEffectObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;
public:
	const char* Get_Modeltag() { return m_EffectDesc.m_ModelDesc.pModeltag; }
	const NONANIMDESC Get_ModelDesc() {return m_EffectDesc.m_ModelDesc;}
	_float Check_CullingRadius();

public:
	void Tick_Direction(_float fTimeDelta);


protected:
	CModel*					m_pModelCom = nullptr;
	EFFECTDESC				m_EffectDesc;
	_float					m_fTime = 0.f;

protected:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources() override; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

public:
	static CEffectObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END