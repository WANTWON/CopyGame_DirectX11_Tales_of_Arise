#pragma once

#include "VIBuffer_Instance.h"

BEGIN(Engine)

class ENGINE_DLL  CVIBuffer_Point_Instance final : public CVIBuffer_Instance
{
private:
	CVIBuffer_Point_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Point_Instance(const CVIBuffer_Point_Instance& rhs);
	virtual ~CVIBuffer_Point_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(_uint iNumInstance) override;
	virtual HRESULT Initialize(void* pArg) override;
public:
	void Update(_float fTimeDelta);

private:
	_float*				m_pSpeedPerSec = nullptr;


public:
	static CVIBuffer_Point_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END