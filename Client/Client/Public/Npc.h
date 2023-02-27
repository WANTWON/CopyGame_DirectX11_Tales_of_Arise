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
public:
	typedef struct Npctag
	{
		NONANIMDESC Modeldesc;
		_uint eNpcType = 0;

	}NPCDESC;
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
	virtual HRESULT Render_EdgeDetection() override;

public: /*For.State*/
	virtual _bool	Is_AnimationLoop(_uint eAnimId) PURE;


public: /*For.Navigation*/
	void Check_Navigation();
	void Compute_CurrentIndex();
	_uint Get_Npctype() { return m_NpcDesc.eNpcType; }

protected: /* For Component */
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	NPCDESC					m_NpcDesc;
	_bool	m_bIsFirst = true;
	_bool  m_bCollision = false;

	_bool m_bIsFirst_conversation = false;
protected:
	virtual HRESULT Ready_Components(void* pArg) PURE;
	virtual HRESULT SetUp_ShaderResources();

public:
	virtual void Free() override;
};

END