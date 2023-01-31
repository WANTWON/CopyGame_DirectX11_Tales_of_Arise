#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRenderer;
class CCollider;
class CTransform;
class CModel;
END

BEGIN(Client)

class CRoom final :	public CGameObject
{
public:
	typedef struct tagRoom
	{
		PIVOT tPivot;
		ROOMTYPE eType;
		_float3 ColliderSize;
	}ROOM;

private:
	CRoom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoom(const CRoom& rhs);
	virtual ~CRoom() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

public:
	ROOM Get_tagRoom(void) { return m_tRoom; }
	void TurnOn_Render(void) { m_isRender = !m_isRender; }
	void Change_Collider(_float3 vScale);

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;

	CCollider* m_pAABBCom = nullptr;

private:
	ROOM m_tRoom;
	_bool m_isRender = false;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);

public:
	static CRoom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pAvg = nullptr);
	virtual void Free(void) override;
};

END