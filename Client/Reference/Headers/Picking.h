#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
	DECLARE_SINGLETON(CPicking)
public:
	CPicking();
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, _uint iWinCX, _uint iWinCY, ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	void Tick(); 
	void Transform_ToLocalSpace(class CTransform * pTransform);
	void Compute_LocalRayInfo(_float3 * pRayDir, _float3 * pRayPos, CTransform * pTransform);

public:
	_vector Get_RayPos() { return XMLoadFloat3(&m_vRayPos_Local); }
	_vector Get_RayDir() { return XMLoadFloat3(&m_vRayDir_Local); }

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	HWND				m_hWnd;
	_uint				m_iWinCX, m_iWinCY;

	_float3					m_vRayDir, m_vRayPos;
	_float3					m_vRayDir_Local, m_vRayPos_Local;
public:
	virtual void Free() override;
};

END