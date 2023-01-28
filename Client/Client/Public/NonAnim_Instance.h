#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
class CTexture;
END


BEGIN(Client)

class CNonAnim_Instance : public CBaseObj
{
public:
	enum SHADER_INSTANCEID {DEFAULT, SHADOW};
protected:
	CNonAnim_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNonAnim_Instance(const CNonAnim_Instance& rhs);
	virtual ~CNonAnim_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;
public:
	const char* Get_Modeltag() { return m_ModelDesc.pModeltag; }
	const NONANIMDESC Get_ModelDesc() {return m_ModelDesc;}

private:
	_bool IsRenderShadow();

protected:
	CModel*					m_pModelCom = nullptr;
	CTexture*				m_pFogtexture = nullptr;
	NONANIMDESC				m_ModelDesc;
	_float					m_fAlpha = 1.f;


protected:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources() override; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

public:
	static CNonAnim_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END