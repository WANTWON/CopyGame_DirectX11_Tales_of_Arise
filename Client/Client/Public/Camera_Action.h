#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)

class CCamera_Action final : public CCamera
{
public:
	typedef struct tagCameraTool
	{
		_float4					vEyePosition;
		_float4					vAtPosition;
		_float					fStartTime;
		_float					fEndTime;
		_bool					bNewSection = false;
		_bool					bLerp = false;
	}TOOLDESC;

	typedef struct tagActionCamera
	{
		TOOLDESC					ActionSettingDesc;
		CAMERADESC					CameraDesc;

	}ACTIONCAMDESC;
	

private:
	CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Action(const CCamera_Action& rhs);
	virtual ~CCamera_Action() = default;

public:
	void					Add_CamData(TOOLDESC CamDesc);
	vector<TOOLDESC>		Get_AllCamData() { return m_CamDatas; }
	void					Remove_Camdata(_int iIndex);
	void					Set_Play(_bool type);
	_bool					Get_Play() { return m_bPlay; }
	void					Set_PlayTime(_float fTime) { m_fPlayTime = fTime; m_fTime = 0.f; }
	void					Set_TargetMatrix(_matrix matTarget) { m_matTarget = matTarget; }
	void					Set_DataName(_tchar* Name) { m_DataName = Name; }
	_tchar*					Get_DataName() { return m_DataName; }

public:
	void					Set_ShakingMode(_bool type, _float fPower = 1.f, _float fMinusPower = 0.1f);
	int						PlayCamera(_float fTimeDelta);
	void					Shaking_Camera(_float fTimeDelta);



public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	_tchar*		m_DataName = nullptr;
	_float		m_fPlayTime = 0.f;
	_bool		m_bPlay = false;
	_float		m_fTime = 0.f;
	_float		m_fShakingTime = 0.f;
	_int		m_iIndex = 0;

	vector<TOOLDESC> m_CamDatas;
	_matrix			 m_matTarget;
	_vector		m_vInitPosition;
	_vector		m_vInitAt;

	_bool			m_bShakingMode = false;
	_float			m_fVelocity = 1.f;
	_float			m_fMinusVelocity = 1.f;
	_vector			m_vShakingStartPos = { 0.f, 0.f, 0.f, 0.f };
	_int			m_iShakingCount = 0;

	_float			m_fLookOffsetY = 0.f;
	_float			m_fCameraOffsetY = 0.f;
	_float			m_fCameraOffsetX = 0.f;

	_vector			m_vNewPos;

	_bool			m_bLerp = false;
public:
	static CCamera_Action* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END