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

class CDoor final : public CGameObject
{
public:
	typedef struct tagDoorDesc
	{
		_uint iDoorType;
		PIVOT tPivot;
		NONOBJECT eType;
		_float3 ColliderSize;
		ROOMTYPE eRoomType;
		_uint iIndex;
	}DOORDESC;
private:
	CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoor(const CDoor& rhs);
	virtual ~CDoor() = default;

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render(void);

public:
	DOORDESC Get_tagMove(void) { return m_tMove; }
	_uint Get_DoorType(void) { return m_iDoorType; }
	void Change_Collider(_float3 vScale);
	void Change_RoomType(ROOMTYPE eType) { m_tMove.eRoomType = eType; }
	void Change_Index(_int iIndex) { m_tMove.iIndex = iIndex; }

private:
	CShader* m_pShaderCom = nullptr;
	CRenderer* m_pRendererCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
	CModel* m_pModelCom = nullptr;

	CCollider* m_pAABBCom = nullptr;

private:
	DOORDESC m_tMove;
	_uint m_iDoorType = -1;

private:
	HRESULT Ready_Components(void);
	HRESULT SetUp_ShaderResources(void);

public:
	static CDoor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pAvg = nullptr);
	virtual void Free(void) override;
};

END