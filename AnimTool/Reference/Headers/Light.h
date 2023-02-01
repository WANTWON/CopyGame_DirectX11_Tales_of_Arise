#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight final : public CBase
{
private:
	CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLight() = default;

public:
	const LIGHTDESC* Get_LightDesc(void) const { return &m_LightDesc; }
	void Set_LightDesc(LIGHTDESC _tLightDesc) { m_LightDesc = _tLightDesc; }

public:
	HRESULT Intialize(const LIGHTDESC& LightDesc);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	LIGHTDESC m_LightDesc;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	virtual void Free(void) override;
};

END