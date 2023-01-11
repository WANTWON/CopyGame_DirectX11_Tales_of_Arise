#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{
public:
	enum CAMERAMODE {CAM_PLAYER, CAM_SHAKING, CAM_TERRAIN, CAM_ITEMGET, 
		CAM_TALK, CAM_ROOM, CAM_CRANE_GAME, CAM_TARGET, CAM_ZOOM, CAM_END };

	typedef struct tagCameraDesc_Derived
	{
		_float4						InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
		_uint						iTest;
		CCamera::CAMERADESC			CameraDesc;
	}CAMERADESC_DERIVED;

private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	void Set_CamMode(CAMERAMODE _eCamMode, _float fPower, _float fVelocity, _float fMinusVelocity)
	{
		if (m_eCamMode == CAM_PLAYER)
			m_ePreCamMode = CAM_PLAYER;
		if (m_eCamMode == CAM_TERRAIN)
			m_ePreCamMode = CAM_TERRAIN;
		if (m_eCamMode == CAM_ITEMGET)
			m_ePreCamMode = CAM_ITEMGET;
		if (m_eCamMode == CAM_TALK)
			m_ePreCamMode = CAM_TALK;
		if (m_eCamMode == CAM_ROOM)
			m_ePreCamMode = CAM_ROOM;

		m_eCamMode = _eCamMode; m_fPower = fPower; m_fVelocity = fVelocity; m_fMinusVelocity = fMinusVelocity;
	
	}
	void Set_CamMode(CAMERAMODE _eCamMode);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

public:
	void Player_Camera(_float fTimeDelta);
	void Shaking_Camera(_float fTimeDelta, _float fPower);
	void Terrain_Camera(_float fTimeDelta);
	void Room_Camera(_float fTimeDelta);
	void Item_Camera(_float fTimeDelta);
	void Talk_Camera(_float fTimeDelta);
	void CraneGame_Camera(_float fTimeDelta);
	void Target_Camera(_float fTimeDelta);


public:
	void Set_Position(_vector vPosition);
	void Set_TargetPosition(_vector vPosition) { XMStoreFloat4(&m_fTargetPos, vPosition); }
	void Set_ZoomValue(_float fValue) { m_fZoom = fValue; }

private:
	CAMERAMODE		m_ePreCamMode = CAM_PLAYER;
	CAMERAMODE		m_eCamMode = CAM_PLAYER;
	_float4			m_vDistance = _float4(0, 10, -10, 0.f);
	_double			m_lMouseWheel = 0;
	_float4			m_fTargetPos = _float4(0.f, 0.f, 0.f, 1.f);
	_float			m_fZoom = 0.f;

	_bool			m_bSetCamLook = true;
	DWORD			m_dwSettingTime = GetTickCount();

	/* For Shaking Camera */
	_float m_fPower = 0.f;
	_float m_fVelocity = 0.f;
	_float m_fMinusVelocity = 0.f;
	_float m_fTime = 0.f;
	_int   m_iShakingCount = 0; 

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END