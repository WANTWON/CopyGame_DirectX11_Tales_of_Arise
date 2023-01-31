#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)
class CNavigation;
class CModel;
END


BEGIN(Client)

class CNpc abstract : public CBaseObj
{
protected:
	CNpc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpc(const CNpc& rhs);
	virtual ~CNpc() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth() override;

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId) PURE;


public: /*For.Navigation*/
	void Check_Navigation();
	void Compute_CurrentIndex();

protected: /* For Component */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	_bool	m_bIsFirst = true;

protected:
	virtual HRESULT Ready_Components(void* pArg) PURE;
	virtual HRESULT SetUp_ShaderResources();

public:
	virtual void Free() override;
};

END