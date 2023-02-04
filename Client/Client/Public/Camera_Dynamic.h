#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_Dynamic final : public CCamera
{
public:

	enum CAMERAMODE {CAM_DEBUG, CAM_PLAYER, CAM_BATTLEZONE, CAM_BATTLE_CLEAR, CAM_END};
	
	typedef struct tagCameraDesc_Derived
	{
		_float4						InitPostion = _float4(0.f, 0.f, 0.f, 1.f);
		_float4						vDistance = _float4(0, 10, -10, 0.f);
		CCamera::CAMERADESC			CameraDesc;
	}CAMERADESC_DERIVED;

private:
	CCamera_Dynamic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Dynamic(const CCamera_Dynamic& rhs);
	virtual ~CCamera_Dynamic() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

public:
	void Set_CamMode(CAMERAMODE _eCamMode);
	void Set_Position(_vector vPosition);
	void Set_TargetPosition(_vector vPosition) { m_fTargetPos = vPosition; }
	void Set_Zoom(_bool type) { m_bZoom = type; }

public:
	void Debug_Camera(_float fTimeDelta);
	void Player_Camera(_float fTimeDelta);
	void Battle_Camera(_float fTimeDelta);
	void BattleClear_Camera(_float fTimeDelta);

private:
	class CBaseObj*	m_pTarget = nullptr;
	CAMERAMODE		m_ePreCamMode = CAM_PLAYER;
	CAMERAMODE		m_eCamMode = CAM_PLAYER;
	
	_float4			m_vDistance = _float4(0, 10, -10, 0.f);
	_long			m_lMouseWheel = 0;
	_vector			m_fTargetPos = { 0.f,0.f,0.f,0.f };
	_float			m_fTime = 0.f;
	_float			m_fOffsetPosY = 4.f;
	_float			m_fAngle = 0.f;
	_float			m_fYAngle = 0.f;
	_double			m_fVec = 1.f;
	_vector			m_OriginPos = { 0.f,0.f,0.f,0.f };
	_vector			m_vNewPos;

	_long			m_MoveValue = 0;
	_long			XMouseMove = 0;
	_long			YMouseMove = 0;

	_bool			m_bLerp = false;
	_bool			m_bStop = false;

	_bool			m_bTurn = false;
	_bool			m_bZoom = false;

	_float			m_fCameraY = 5.f;
	_float			m_fZoom = 0.f;

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END