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

HRESULT CCameraManager::Play_ActionCamera(_tchar * wcCameraDataName, _matrix mWorldMatrix)
{
	

	/* Load Effect File. */
	HANDLE hFileCamera = nullptr;
	_tchar LoadPathEffect[MAX_PATH] = TEXT("../../../Bin/Data/CameraData/");
	wcscat_s(LoadPathEffect, MAX_PATH, wcCameraDataName);

	hFileCamera = CreateFile(LoadPathEffect, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileCamera == INVALID_HANDLE_VALUE)
		return E_FAIL;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	DWORD dwByte = 0;
	_uint iCameraCount = 0;
	_int eCameraType = 0;
	ReadFile(hFileCamera, &eCameraType, sizeof(_int), &dwByte, nullptr);


	if (eCameraType == CCameraManager::TARGETMODE)
	{
		if (m_eCamState != CAM_DYNAMIC)
		{
			CCamera* pCamera = dynamic_cast<CCamera*>(CGameInstance::Get_Instance()->Get_Object(m_eCurrentLevel, TEXT("Layer_Camera"), CAM_DYNAMIC));
			if (nullptr == pCamera)
				return E_FAIL;
			m_eCamState = CAM_DYNAMIC;
			m_pCurrentCamera = pCamera;
		}
		dynamic_cast<CCamera_Dynamic*>(m_pCurrentCamera)->Set_TargetMatrix(mWorldMatrix);
		dynamic_cast<CCamera_Dynamic*>(m_pCurrentCamera)->Set_CamMode(CCamera_Dynamic::CAM_TARGETMODE);

		CCamera_Dynamic::TOOLDESC CamToolData;

		/* Read how many Effects there are in this File. */
		_float fPlayTime = 0.f;
		ReadFile(hFileCamera, &fPlayTime, sizeof(_float), &dwByte, nullptr);
		dynamic_cast<CCamera_Dynamic*>(m_pCurrentCamera)->Set_PlayTime(fPlayTime);

		ReadFile(hFileCamera, &iCameraCount, sizeof(_uint), &dwByte, nullptr);
		/* For every Effect in this File: */
		for (_uint i = 0; i < iCameraCount; i++)
		{

			ReadFile(hFileCamera, &CamToolData, sizeof(CCamera_Dynamic::TOOLDESC), &dwByte, nullptr);
			dynamic_cast<CCamera_Dynamic*>(m_pCurrentCamera)->Add_CamData(CamToolData);
		}

	}
	else
	{
		
		CCamera* pCamera = dynamic_cast<CCamera*>(CGameInstance::Get_Instance()->Get_Object(m_eCurrentLevel, TEXT("Layer_Camera"), CAM_ACTION));
		if (nullptr == pCamera)
			return E_FAIL;
		m_eCamState = CAM_ACTION;
		m_pCurrentCamera = pCamera;
		
		dynamic_cast<CCamera_Action*>(m_pCurrentCamera)->Set_TargetMatrix(mWorldMatrix);

		CCamera_Action::TOOLDESC CamToolData;

		/* Read how many Effects there are in this File. */
		_float fPlayTime = 0.f;
		ReadFile(hFileCamera, &fPlayTime, sizeof(_float), &dwByte, nullptr);
		dynamic_cast<CCamera_Action*>(m_pCurrentCamera)->Set_PlayTime(fPlayTime);

		ReadFile(hFileCamera, &iCameraCount, sizeof(_uint), &dwByte, nullptr);
		/* For every Effect in this File: */
		for (_uint i = 0; i < iCameraCount; i++)
		{

			ReadFile(hFileCamera, &CamToolData, sizeof(CCamera_Action::TOOLDESC), &dwByte, nullptr);
			dynamic_cast<CCamera_Action*>(m_pCurrentCamera)->Add_CamData(CamToolData);
		}
	}
	

	RELEASE_INSTANCE(CGameInstance);
	CloseHandle(hFileCamera);

	if (eCameraType == CCameraManager::TARGETMODE)
		dynamic_cast<CCamera_Dynamic*>(m_pCurrentCamera)->Set_Play(true);
	else
		dynamic_cast<CCamera_Action*>(m_pCurrentCamera)->Set_Play(true);

	return S_OK;
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
