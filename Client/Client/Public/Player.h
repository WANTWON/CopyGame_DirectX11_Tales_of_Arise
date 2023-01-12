#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)

class CShader;
class CCollider;
class CRenderer;
class CTransform;
class CNavigation;
class CModel;

END


BEGIN(Client)

class CPlayer final : public CGameObject
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
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader*				m_pShaderCom = nullptr;	
	CRenderer*				m_pRendererCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CModel*					m_pModelCom = nullptr;

	CNavigation*			m_pNavigationCom = nullptr;

	CCollider*				m_pAABBCom = nullptr;
	CCollider*				m_pOBBCom = nullptr;
	CCollider*				m_pSPHERECom = nullptr;

private:
	vector<class CGameObject*>			m_Parts;


private:
	HRESULT Ready_Parts();
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */


public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END