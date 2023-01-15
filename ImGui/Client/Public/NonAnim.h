#pragma once

#include "BaseObj.h"


BEGIN(Engine)
class CModel;
END


BEGIN(Client)

class CNonAnim final : public CBaseObj
{
public:
	typedef struct NonAnimModelTag
	{
		char pModeltag[MAX_PATH] = "";
		_float3 vPosition = _float3(0.f, 0.f, 0.f);
		_float3 vScale = _float3(1.f, 1.f, 1.f);
		_float3 vRotation = _float3(0.f, 0.f, 0.f);
		_float m_fAngle = 0.f;

	}NONANIMDESC;

private:
	CNonAnim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNonAnim(const CNonAnim& rhs);
	virtual ~CNonAnim() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Setting_PickingSymbol(_int i) { m_iSymbolType = i; }

public:
	virtual _bool Picking(_float3* PickingPoint) override;
	virtual void PickingTrue();
	void Set_Picked();
	const char* Get_Modeltag() { return m_ModelDesc.pModeltag; }
	const NONANIMDESC Get_ModelDesc() {return m_ModelDesc;}
	void Set_ModelDesc(NONANIMDESC* NonAnimDesc) { memcpy(&m_ModelDesc, NonAnimDesc, sizeof(NONANIMDESC)); }
	void Turn(_float3 vAxis, _float fAngle);
private:
	CModel*					m_pModelCom = nullptr;
	NONANIMDESC				m_ModelDesc;
	_int					m_iSymbolType = -1;


private:
	virtual HRESULT Ready_Components(void* pArg) override;
	virtual HRESULT SetUp_ShaderResources() override; /* 셰이더 전역변수에 값을 전달한다. */
	virtual HRESULT SetUp_ShaderID() override;

public:
	static CNonAnim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END