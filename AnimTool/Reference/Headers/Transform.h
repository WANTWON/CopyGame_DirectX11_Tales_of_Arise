#pragma once
/* 월드스페이스에서의 상태를 보관한다.(월드행렬) */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final :	public CComponent
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_TRANSLATION, STATE_END };

public:
	typedef struct tagTransformDesc
	{
		_float fSpeedPerSec;
		_float fRotationPerSec;
	}TRANSFORMDESC;

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState) const { return XMLoadFloat4((_float4*)&m_WorldMatrix.m[eState][0]); }
	/* 리턴받은 행렬을 이용해 연산을 해야할 때. */
	_matrix Get_WorldMatrix(void) const { return XMLoadFloat4x4(&m_WorldMatrix); }
	/* 리턴받은 행렬보관해야할 때 */
	_float4x4 Get_World4x4(void) const { return m_WorldMatrix; }
	/* 리턴받은 행렬을 셰이더에 던지기위해 */
	_float4x4 Get_World4x4_TP(void) const
	{
		_float4x4 TransposeMatrix;

		XMStoreFloat4x4(&TransposeMatrix, XMMatrixTranspose(Get_WorldMatrix()));

		return TransposeMatrix;
	}
	const _float4x4* Get_World4x4Ptr(void) const { return &m_WorldMatrix; }

	_float3 Get_Scale(void);

	void Set_State(STATE eState, _float3 vState) { memcpy(&m_WorldMatrix.m[eState][0], &vState, sizeof(_float3)); }
	void Set_TransformDesc(const TRANSFORMDESC& TransformDesc) { m_TransformDesc = TransformDesc; }
	void Set_Scale(_float3 vScale);
	void Set_Rotation(_float3 vRotation);

public:
	virtual HRESULT Initialize_Prototype(void);
	virtual HRESULT Initialize(void* pArg);

public:
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void LookAt(_fvector vPoint);

private:
	_float4x4 m_WorldMatrix;
	TRANSFORMDESC m_TransformDesc;

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;
};

END