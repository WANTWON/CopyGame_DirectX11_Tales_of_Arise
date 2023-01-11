#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_2D final : public CCamera
{
public:
	typedef struct tagCameraDesc_Derived
	{
		_float4						InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
		_float						fMinXPos = 0.f;
		_float						fMaxXPos = 0.f;
		CCamera::CAMERADESC			CameraDesc;
	}CAMERADESC_DERIVED;

private:
	CCamera_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_2D(const CCamera_2D& rhs);
	virtual ~CCamera_2D() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

public:
	void Camera_2D(_float fTimeDelta);
	
private:
	CAMERADESC_DERIVED		m_CameraDesc;
	_float4					m_vDistance = _float4(0, 10, -10, 0.f);
	_float4					m_fTargetPos = _float4(0.f, 0.f, 0.f, 1.f);

public:
	static CCamera_2D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END