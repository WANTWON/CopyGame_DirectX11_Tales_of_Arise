#pragma once

#include "Client_Defines.h"
#include "BaseObj.h"


BEGIN(Engine)

class CNavigation;
class CModel;

END


BEGIN(Client)

class CPlayer final : public CBaseObj
{	
public:
	enum PARTS { PARTS_WEAPON, PARTS_END };
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual int Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public: /*For Navigation*/
	void	Change_Navigation(LEVEL eLevel);
	void	Compute_CurrentIndex(LEVEL eLevel);

private:
	CModel*					m_pModelCom = nullptr;
	CNavigation*			m_pNavigationCom = nullptr;
	vector<CNavigation*>	m_vecNavigaitions;

private:
	vector<class CGameObject*>			m_Parts;

private:
	HRESULT Ready_Parts();
	HRESULT Ready_Components(void* pArg);
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END