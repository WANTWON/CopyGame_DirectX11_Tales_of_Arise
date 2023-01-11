#pragma once

#include "BaseObj.h"
BEGIN(Engine)
class CModel;
class CNavigation;
END


BEGIN(Client)

class CBladeTrap final : public CBaseObj
{
public:
	enum STATE { TURN, TURN_ST, TUNR_ED, IDLE};

private:
	CBladeTrap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBladeTrap() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual int Tick(_float fTimeDelta) override;
	virtual void Late_Tick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Change_Animation(_float fTimeDelta);
	virtual HRESULT Ready_Components(void* pArg = nullptr) override;
	virtual HRESULT SetUp_ShaderResources() override;
	

private:
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	DWORD					m_dwAttackTime = GetTickCount();

	_bool					m_bStart = false;
	_bool					m_bMove = false;
	_int					m_iMoveCount = 0;

	_vector					m_vDir = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	STATE					m_eState = IDLE;
	STATE					m_ePreState = TUNR_ED;

public:
	static CBladeTrap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END