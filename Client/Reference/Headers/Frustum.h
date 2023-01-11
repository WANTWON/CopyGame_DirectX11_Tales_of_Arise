#pragma once

#include "Base.h"

/* 절두체의 정보를 포함하고 있는 클래스다. */

BEGIN(Engine)

class CFrustum final : public CBase
{
	DECLARE_SINGLETON(CFrustum)
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	void Initialize(); /* 투영스페이스 상의 점 여덟개를 정의한다. */
	void Transform_ToWorldSpace(); /* 여덟개 점을 월드로 변환한다. */

public:
	void Transform_ToLocalSpace(_fmatrix WorldMatrix);
	/* 월드스페이스절두체 안에 있니?? */
	_bool isIn_WorldFrustum(_fvector vPosition, _float fRange ) const ;
	_bool isIn_LocalFrustum(_fvector vPosition, _float fRange) const ;

private:
	_float3				m_vOriginalPoints[8];
	_float3				m_vWorldPoints[8];

	
	_float4				m_WorldPlane[6];
	_float4				m_LocalPlane[6];

private:
	void Make_Plane(_In_ const _float3* pPoints, _Out_ _float4* pPlanes);
public:
	virtual void Free() override;
};

END