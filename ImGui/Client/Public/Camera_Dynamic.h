#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
END

BEGIN(Client)
class CBaseObj;
class CCamera_Dynamic final : public CCamera
{
public:
	enum CAM_MODE { CAM_DEBUG, CAM_TARGET};

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
		_uint						iTest;
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
	int						PlayCamera(_float fTimeDelta);
	int						DebugVCamera(_float fTimeDelta);

public:
	void Add_CamData(TOOLDESC CamDesc) { m_CamDatas.push_back(CamDesc); }
	void Set_CamData(_int iIndex, TOOLDESC CamDesc) { memcpy(&m_CamDatas[iIndex], &CamDesc, sizeof(TOOLDESC)); }
	TOOLDESC Get_CamData(_int iIndex) { return m_CamDatas[iIndex]; }
	vector<TOOLDESC> Get_AllCamData() { return m_CamDatas; }
	void Remove_Camdata(_int iIndex) { m_CamDatas.erase(m_CamDatas.begin() + iIndex); };

public:
	void					Set_CamMode(CAM_MODE eMode) { m_eCamMode = eMode; }
	void					Set_Play(_bool type);
	void					Set_PlayTime(_float fTime) { m_fPlayTime = fTime; m_fTime = 0.f; }
	_bool					Get_Play() { return m_bPlay; }
	_float					Get_PlayTime() { return m_fPlayTime; }
	_tchar*					Get_CameraName() { return m_CameraName; }
	_uint					Get_CamMode() { return m_eCamMode; }
	
public:
	void Set_Target(CBaseObj* pTarget) { m_pTarget = pTarget; }

	
private:
	_uint		m_eCamMode = CAM_DEBUG;
	_vector		m_vInitPos = { 0.f,0.f,0.f,1.f };
	_vector		m_vInitAt = { 0.f,0.f,0.f,1.f };

	CBaseObj* m_pTarget = nullptr;

	_float		m_fPlayTime = 0.f;
	_bool		m_bPlay = false;
	_tchar		m_CameraName[MAX_PATH] = TEXT("");
	_float		m_fTime = 0.f;
	_int		m_iIndex = 0;
	_bool		m_bLerp = false;
	vector<TOOLDESC> m_CamDatas;

public:
	static CCamera_Dynamic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END