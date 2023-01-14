#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_SwordTrail final : public CVIBuffer
{
public:
	typedef struct Trail_Point
	{
		_float3		vPointUp;
		_float3		vPointDown;

		_float4x4	vTrailMatrix;

	}TRAILPOINT;

protected:
	CVIBuffer_SwordTrail(ID3D11Device*	pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_SwordTrail(const CVIBuffer_SwordTrail& rhs);
	virtual ~CVIBuffer_SwordTrail() = default;

private:
	_int		m_iLimitPointCount = 30;
	list<_float3>		m_TrailPoint;

	_uint		m_iEndIndex = 0;
	_uint		m_iCatmullRomCount = 0;
	_uint		m_iVtxCount = 0;
	_uint		m_iCatmullRomIndex[4] = { 0 };
public:

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	HRESULT Update(TRAILPOINT TrailPoint);

public:
	static CVIBuffer_SwordTrail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
