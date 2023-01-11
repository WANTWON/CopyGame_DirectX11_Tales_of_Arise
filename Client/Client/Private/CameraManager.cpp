#include "stdafx.h"
#include "..\Public\CameraManager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CCameraManager);

CCameraManager::CCameraManager()
{
}


void CCameraManager::Set_CamState(CAM_STATE _eState)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CCamera* pCamera = dynamic_cast<CCamera*>(pGameInstance->Get_Object(m_eCurrentLevel, TEXT("Layer_Camera"), _eState));
	if (nullptr == pCamera)
		return;
	m_eCamState = _eState;
	m_pCurrentCamera = pCamera;
}


HRESULT CCameraManager::Ready_Camera(LEVEL eLevelIndex)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	CCamera* pCamera = dynamic_cast<CCamera*>(pGameInstance->Get_Object(eLevelIndex, TEXT("Layer_Camera")));

	if (nullptr == pCamera)
		return E_FAIL;

	m_pCurrentCamera = pCamera;
	m_eCamState = CAM_DYNAMIC;
	m_eCurrentLevel = eLevelIndex;
	return S_OK;
}

void CCameraManager::Free()
{

}
