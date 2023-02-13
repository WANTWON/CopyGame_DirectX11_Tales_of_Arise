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
	void					Set_PlayTime(_float fTime) { m_fPlayTime = fTime; m_fTime = 0.f; }
	void					Set_TargetMatrix(_matrix matTarget) { m_matTarget = matTarget; }
public:
	int						PlayCamera(_float fTimeDelta);
	
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual int Tick(_float fTimeDelta)override;
	virtual void Late_Tick(_float fTimeDelta)override;
	virtual HRESULT Render() override;

private:
	_float		m_fPlayTime = 0.f;
	_bool		m_bPlay = false;
	_float		m_fTime = 0.f;
	_int		m_iIndex = 0;

	vector<TOOLDESC> m_CamDatas;
	_matrix			 m_matTarget;

	
public:
	static CCamera_Action* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
	virtual void Free() override;
};

END