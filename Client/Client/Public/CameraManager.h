#pragma once
#include "Camera_Dynamic.h"
#include "Camera_Action.h"

BEGIN(Client)
class CBaseObj;
class CCameraManager final : public CBase
{
	DECLARE_SINGLETON(CCameraManager)
public:
	enum CAM_STATE { CAM_DYNAMIC, CAM_ACTION, CAM_END };
	enum LOAD_CAMERATYPE { TARGETMODE, ACTION };
public:
	CCameraManager();
	virtual ~CCameraManager() = default;

public: /* Get&Set*/ 
	CAM_STATE Get_CamState() { return m_eCamState; }
	CCamera* Get_CurrentCamera() { return m_pCurrentCamera; }
	void Set_CamState(CAM_STATE _eState);
	void Set_CurrentCamera(CCamera* _pCamera) { m_pCurrentCamera = _pCamera; }

public: /*For Action Camera*/
	HRESULT Play_ActionCamera(_tchar* wcCameraDataName, _matrix mWorldMatrix);
	vector<class CBaseObj*> Get_ActionObjectts() { return m_pActionObjects; }
	void Push_ActionList(class CBaseObj* pObject) { m_pActionObjects.push_back(pObject); }
	void Clear_ActionList();

public:
	HRESULT Ready_Camera(LEVEL eLevelIndex);

private:
	CCamera*					m_pCurrentCamera = nullptr;
	CAM_STATE					m_eCamState = CAM_DYNAMIC;
	LEVEL						m_eCurrentLevel;

	vector<class CBaseObj*>			m_pActionObjects;

public:
	virtual void Free() override;
};

END