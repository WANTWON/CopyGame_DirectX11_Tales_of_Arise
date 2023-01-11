#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHTDESC* Get_LightDesc(_uint iIndex);
	list<class CLight*>* GetLights() { return &m_Lights; }
	_uint Get_LightSize() { return (_uint)m_Lights.size(); }
	void Set_LightDesc(_uint iIndex, LIGHTDESC* pLightDesc);

public:
	_float4x4	Get_ShadowLightView();
	HRESULT		Set_ShadowLightView(_float4 vEye, _float4 vAt);

public:
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHTDESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	void Clear_AllLight();
	void Clear_Light(_uint iIndex);
	

private:
	list<class CLight*>				m_Lights;
	_float4x4						m_matShadowLightView;

public:
	virtual void Free() override;
};

END