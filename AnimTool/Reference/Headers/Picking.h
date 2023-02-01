#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
private:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(void);

	void Transform_ToLocalSpace(_matrix WorldMatrixInverse);
	_bool Intersect_InWorldSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3* pOut);
	_bool Intersect_InLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3* pOut);

private:
	HWND m_hWnd;
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	_float3 m_vRayDir, m_vRayPos;
	_float3 m_vRayDir_Local, m_vRayPos_Local;

public:
	virtual void Free(void);
};

END	