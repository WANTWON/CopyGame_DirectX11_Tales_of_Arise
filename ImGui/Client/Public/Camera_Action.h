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
	
	enum CAM_MODE { CAM_DEBUG, CAM_ACTION };

private:
	CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Action(const CCamera_Action& rhs);
	virtual ~CCamera_Action() = default;

public:
	void Add_CamData(TOOLDESC CamDesc);
	void Set_CamData(_int iIndex, TOOLDESC CamDesc) { memcpy(&m_CamDatas[iIndex], &CamDesc, sizeof(TOOLDESC)); }
	void Set_PositionToCurrentData(_int iIndex);
	TOOLDESC Get_CamData(_int iIndex) { return m_CamDatas[iIndex]; }
	vector<TOOLDESC> Get_AllCamData() { return m_CamDatas; }
	void Remove_Camdata(_int iIndex) { m_CamDatas.erase(m_CamDatas.begin() + iIndex); };
	void Remove_AllCamdata() { m_CamDatas.clear(); };

public:
	void					Set_CamMode(CAM_MODE eMode) { m_eCamMode = eMode; }
	void					Set_Play(_bool type);
	void					Set_PlayTime(_float fTime) { m_fPlayTime = fTime; m_fTime = 0.f; }
	_bool					Get_Play() { return m_bPlay; }
	_bool					Get_Finished() { return m_bFinished; }
	_float					Get_PlayTime() { return m_fPlayTime; }
	_tchar*					Get_CameraName() { return m_CameraName; }
	int						PlayCamera(_float fTimeDelta);
	_float4					Get_CurrentEye() { return m_CameraDesc.vEye; }
	_float4					Get_CurrentAt() { return m_CameraDesc.vAt; }
	int						DebugVCamera(_float fTimeDelta);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	_uint		m_eCamMode = CAM_DEBUG;
	_bool		m_bIsSelected = true;
	_float		m_fPlayTime = 0.f;
	_bool		m_bPlay = false;
	_tchar		m_CameraName[MAX_PATH] = TEXT("");
	_float		m_fTime = 0.f;
	_int		m_iIndex = 0;
	_vector		m_vInitPosition;
	_vector		m_vInitAt;
	_bool		m_bFinished = false;

	vector<TOOLDESC> m_CamDatas;

	
public:
	static CCamera_Action* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END