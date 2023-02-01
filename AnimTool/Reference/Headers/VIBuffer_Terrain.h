#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumVerticesX, _uint iNumVerticesZ);
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _bool Picking(_matrix WorldMatrix, _float3* pPickPoint = nullptr) override;

private:
	_uint m_iNumVerticesX = 0;
	_uint m_iNumVerticesZ = 0;

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumVerticesX, _uint iNumVerticesZ);
	virtual CComponent* Clone(void* pArg);
	virtual void Free(void) override;
};

END