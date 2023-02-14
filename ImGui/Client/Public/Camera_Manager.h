#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "Camera_Action.h"
#include "Camera_Dynamic.h"
#include "NonAnim.h"

BEGIN(Client)
class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)
public:
	enum CAM_MODE { DYNAMIC, ACTION };
	enum CAMERASYMBOL { CAM_EYE, CAM_AT, CAM_END};
	

public:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	void		Set_CamMode(CAM_MODE eMode) { m_eCamMode = eMode; }
	CAM_MODE	Get_CamMode() { return m_eCamMode; }

public:
	HRESULT			Add_Camera();
	void			Remove_Camera(_int iIndex);
	void			Clear_Camreras();
	CCamera_Action* Get_CurrentCamera() { return m_pCurrentCamera; }
	_uint			Get_SymBolType() { return m_iSymBolType; }


	void	Add_CameraSymbol(CAMERASYMBOL eSymbol, CBaseObj* pObject) { m_pClickedSymbol[eSymbol] = pObject; }
	void	Set_CameraSymbolPosition(CAMERASYMBOL eSymbol, _vector vPosition);
	HRESULT Add_ClickedSymbol(_float4 vClickPos, CAMERASYMBOL Symboltype);
	void	Click_Position(_float4 vPosition);
	void	Clear_ClickPosition();
private:
	CAM_MODE										m_eCamMode = DYNAMIC;

private:
	_int											m_iCamIndex = 0;
	_uint											m_iSymBolType = CAM_EYE;

	vector<_float4>									m_vClickedPoints;
	CBaseObj*										m_pClickedSymbol[CAM_END] = { nullptr };
	CCamera_Action*									m_pCurrentCamera = nullptr;

	_vector					m_vEyePosition = {0.f,0.f,0.f,1.f};
	_vector					m_vAtPosition = { 0.f,0.f,0.f,1.f };
	_float					m_fStartTime = 0.f;
	_float					m_fEndTime = 1.f;

public:
	void Free() override;
};

END