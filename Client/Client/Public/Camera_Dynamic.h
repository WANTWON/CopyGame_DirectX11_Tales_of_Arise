#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)
class CMonster;
class CCamera_Dynamic final : public CCamera
{
public:

	enum CAMERAMODE {CAM_DEBUG, CAM_PLAYER,
		CAM_BATTLEZONE, CAM_BATTLE_CLEAR, CAM_LOCKON, CAM_LOCKOFF,
		CAM_AIBOOSTON, CAM_AIBOOSTOFF, CAM_TARGETMODE, CAM_TARGETMODE_OFF,
		CAM_ROOM,
		CAM_END};
	

	typedef struct tagCameraTool
	{
		_float					fRadian = 0.f;
		_float					fLength = 0.f;
		_float					fYoffset = 0.f;
		_float4					vLook;
		_float					fStartTime;
		_float					fEndTime;

	}TOOLDESC;



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
	void Set_Target(class CBaseObj* pTarget) { m_pTarget = pTarget; }
	void Set_CamMode(CAMERAMODE _eCamMode);
	void Set_Position(_vector vPosition);
	void Set_TargetPosition(_vector vPosition) { m_vTargetPos = vPosition; }
	void Set_Zoom(_bool type) { m_bZoom = type; }
	CAMERAMODE Get_CamMode() { return m_eCamMode; }
	CBaseObj* Get_Target() { return  m_pTarget; }

public:
	void					Add_CamData(TOOLDESC CamDesc) { m_CamDatas.push_back(CamDesc); }
	vector<TOOLDESC>		Get_AllCamData() { return m_CamDatas; }
	void					Remove_Camdata(_int iIndex) { m_CamDatas.erase(m_CamDatas.begin() + iIndex); };
	void					Set_Play(_bool type);
	void					Set_PlayTime(_float fTime) { m_fPlayTime = fTime; m_fTime = 0.f; }
	void					Set_TargetMatrix(_matrix matTarget) { m_matTarget = matTarget; }
	void					Set_ShakingMode(_bool type, _float fPower = 1.f, _float fMinusPower = 0.1f);
public:
	

public:
	void Debug_Camera(_float fTimeDelta);
	void Player_Camera(_float fTimeDelta);
	void Battle_Camera(_float fTimeDelta);
	void BattleClear_Camera(_float fTimeDelta);
	void LockOn_Camera(_float fTimeDelta);
	void LockOff_Camera(_float fTimeDelta);

	void AIBoostOn_Camera(_float fTimeDelta);
	void AIBoostOff_Camera(_float fTimeDelta);

	void Change_LockOn(_uchar eKeyID);
	void TargetTool_Camera(_float fTimeDelta);
	void TargetTool_CameraOff(_float fTimeDelta);

	void Room_Camera(_float fTimeDelta);
private:
	void ZoomSetting(_float fDistance, _float fSpeed);
	void Shaking_Camera(_float fTimeDelta);

private:
	class CBaseObj*	m_pTarget = nullptr;
	CAMERAMODE		m_ePreCamMode = CAM_PLAYER;
	CAMERAMODE		m_eCamMode = CAM_PLAYER;
	
	_vector			m_vDistance = { 0.f,0.f,0.f,0.f };
	_long			m_lMouseWheel = 0;
	_vector			m_vTargetPos = { 0.f,0.f,0.f,0.f };
	_vector			m_vLasrDirwithPlayer = { 0.f,0.f,0.f,0.f };
	_vector			m_vLastLookPos = { 0.f,0.f,0.f,0.f };
	_float			m_fTime = 0.f;
	_float			m_fLookOffsetY = 4.f;
	_float			m_fCameraOffsetY = 5.f;
	_float			m_fAngle = 0.f;
	_double			m_fVec = 1.f;
	_vector			m_OriginPos = { 0.f,0.f,0.f,0.f };
	_vector			m_vNewPos;

	_float			fLength = 8.f;
	_long			m_MoveValue = 0;
	_long			XMouseMove = 0;
	_long			YMouseMove = 0;

	_bool			m_bLerp = false;
	_bool			m_bStop = false;

	_bool			m_bTurn = false;
	_bool			m_bZoom = false;
	_float			m_fZoomOffset = 0.f;

	_bool			m_bShakingMode = false;
	_float			m_fVelocity = 1.f;
	_float			m_fMinusVelocity = 1.f;
	_vector			m_vShakingStartPos = { 0.f, 0.f, 0.f, 0.f };
	_int			m_iShakingCount = 0;
	
	_bool			m_bBlurResetted = false;

private:
	_float		m_fPlayTime = 0.f;
	_bool		m_bPlay = false;
	_int		m_iIndex = 0;
	_vector		m_vInitPos = { 0.f,0.f,0.f,0.f };

	vector<TOOLDESC> m_CamDatas;
	_matrix			 m_matTarget;

private:
	vector<CMonster*> vecRightMonster;
	vector<CMonster*> vecLeftMonster;

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END