#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
END


BEGIN(Client)

class CFootSwitch final : public CBaseObj
{
public:
	enum STATE { PRESS, PRESSING, IDLE};

private:
	CFootSwitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFootSwitch() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	_bool  Get_IsBoxMade() { return m_bMadeBox; }

private:
	void Change_Animation(_float fTimeDelta);
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;


private:
	CModel*					m_pModelCom = nullptr;
	_bool					m_bMadeBox = false;
	_bool					m_bFirst = false;

	STATE m_eState = IDLE;
	STATE m_ePreState = PRESS;

public:
	static CFootSwitch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END