#pragma once
#include "Base.h"
#include "Client_Defines.h"
#include "NonAnim.h"

BEGIN(Client)
class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

public:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Add_Camera();
	HRESULT Add_Camera(_float3 position);
	void	Add_CreatedCamera(class CNonAnim* pCamera) { m_vecCameras.push_back(pCamera); }
	void	Out_CreatedCamera(CNonAnim* pGameObject);
	void	Update_CamPosition(_float3 position);
	void	Clear_Camreras();

public:
	_int						  Get_CurrentCamIndex() { return m_iCamIndex; }
	void						  Set_CilckedCamIndex(_uint iIndex) { m_iCamIndex = iIndex; }
	const vector<class CNonAnim*> Get_Cameras() const { return m_vecCameras; }
	_int						  Get_CameraSize() { return (_int)m_vecCameras.size();}
	CNonAnim*					  Get_CurrentCam() { if(m_vecCameras.size() != 0 ) return m_vecCameras[m_iCamIndex]; }
	CNonAnim*					  Get_LastCam() { return m_vecCameras.back(); }

private:
	_int					m_iCamIndex = 0;
	vector<class CNonAnim*> m_vecCameras;	

public:
	void Free() override;
};

END