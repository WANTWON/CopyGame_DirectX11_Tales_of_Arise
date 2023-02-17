#pragma once

#include "Component.h"

/* 충돌체 */

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum BOUNDING { BOUNDING_ORIGINAL, BOUNDING_WORLD, BOUNDING_END };

public: /* 내 콜라이더가 생성될 때 취해야할 초기 상태. */
	typedef struct tagColliderDesc
	{
		_float3		vScale;
		_float3		vRotation;
		_float3		vPosition;
	}COLLIDERDESC;

	typedef struct tagOBB
	{
		_float3		vCenter;
		_float3		vAlignAxis[3];
		_float3		vCenterAxis[3];		
	}OBBDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg);

public:
	void Update(_fmatrix WorldMatrix);
	_float Get_SphereRadius(void) { return m_pSphere[BOUNDING_WORLD]->Radius; }
	HRESULT Set_ColliderDesc(void* pArg);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG

public:
	_bool Collision(class CCollider* pTargetCollider);

	_bool Collision_AABB(class CCollider* pTargetCollider);
	_bool Collision_OBB(class CCollider* pTargetCollider);



private:
	TYPE					m_eType = TYPE_END;
	
	COLLIDERDESC			m_ColliderDesc;

	BoundingBox*			m_pAABB[BOUNDING_END] = { nullptr } ;
	BoundingOrientedBox*	m_pOBB[BOUNDING_END] = { nullptr };
	BoundingSphere*			m_pSphere[BOUNDING_END] = { nullptr };

	_bool					m_isCollision = false;

#ifdef _DEBUG
private:
	ID3D11InputLayout*								m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>*			m_pBatch = nullptr;
	BasicEffect*									m_pEffect = nullptr;
	ID3D11DepthStencilState*						m_pDepthStencilState = nullptr;
#endif // _DEBUG



private:
	_matrix Remove_Rotation(_fmatrix Matrix);
	_float3 Compute_Min();
	_float3 Compute_Max();

	OBBDESC Compute_OBBDesc();


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
