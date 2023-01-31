#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END };
	enum BOUNDING { BOUNDING_ORIGINAL, BOUNDING_WORLD, BOUNDING_END };
public:
	typedef struct tagColliderDesc
	{
		_float3 vScale;
		_float3 vRotation;
		_float3 vPosition;
	}COLLIDERDESC;

	typedef struct tagOBB
	{
		_float3 vCenter;
		_float3 vAlignAxis[3];
		_float3 vCenterAxis[3];
	}OBBDESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& rhs);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eType);
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Tick(_fmatrix WorldMatrix);
	HRESULT Render(void);

public:
	_bool Collision(class CCollider* pTargetCollider);

	_bool Collision_AABB(class CCollider* pTargetCollider);
	_bool Collision_OBB(class CCollider* pTargetCollider);

public:
	void Change_Scale(_float3 vScale);

private:
	TYPE m_eType = TYPE_END;

	COLLIDERDESC m_ColliderDesc;

	BoundingBox* m_pAABB[BOUNDING_END] = { nullptr };
	BoundingOrientedBox* m_pOBB[BOUNDING_END] = { nullptr };
	BoundingSphere* m_pSPHERE[BOUNDING_END] = { nullptr };

	_bool m_isCollision = false;

#ifdef _DEBUG
private:
	ID3D11InputLayout* m_pInputLayout = nullptr;
	PrimitiveBatch<VertexPositionColor>* m_pBatch = nullptr;
	BasicEffect* m_pEffect = nullptr;
#endif // _DEBUG

private:
	_matrix Remove_Rotation(_fmatrix Matrix);
	_float3 Compute_Min(void);
	_float3 Compute_Max(void);

	OBBDESC Compute_OBBDesc(void);

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END