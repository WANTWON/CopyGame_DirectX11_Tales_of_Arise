#include "stdafx.h"
#include "..\Public\Camera_Manager.h"
#include "GameInstance.h"
#include "Model.h"
#include "GameObject.h"
#include "PipeLine.h"


IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{
	m_vecCameras.reserve(0);
}


HRESULT CCamera_Manager::Add_Camera()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	
	CNonAnim::NONANIMDESC  NonAnimDesc;
	CTransform* pTransform = (CTransform*)pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform"));

	if (pTransform == nullptr)
		return E_FAIL;

	XMStoreFloat3(&NonAnimDesc.vPosition, pTransform->Get_State(CTransform::STATE_TRANSLATION));
	char pModeltag[MAX_PATH] = "Boulder.fbx";
	strcpy(NonAnimDesc.pModeltag, pModeltag);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, TEXT("Layer_CameraModel"), &NonAnimDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CCamera_Manager::Add_Camera(_float3 position)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CNonAnim::NONANIMDESC  NonAnimDesc;
	NonAnimDesc.vPosition = position;
	char pModeltag[MAX_PATH] = "Boulder.fbx";
	strcpy(NonAnimDesc.pModeltag, pModeltag);

	if (FAILED(pGameInstance->Add_GameObject(TEXT("Prototype_GameObject_NonAnim"), LEVEL_GAMEPLAY, TEXT("Layer_CameraModel"), &NonAnimDesc)))
	{
		RELEASE_INSTANCE(CGameInstance);
		return E_FAIL;
	}


	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CCamera_Manager::Out_CreatedCamera(CNonAnim * pGameObject)
{
	auto& iter = m_vecCameras.begin();
	while (iter != m_vecCameras.end())
	{
		if (*iter == pGameObject)
			iter = m_vecCameras.erase(iter);
		else
			++iter;
	}
}

void CCamera_Manager::Update_CamPosition(_float3 position)
{
	if (m_vecCameras.size() != 0)
	{
		_vector vPosition = XMLoadFloat3(&position);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_vecCameras[m_vecCameras.size() - 1]->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
}

void CCamera_Manager::Clear_Camreras()
{
	for (auto& iter : m_vecCameras)
		iter->Set_Dead(true);
	m_vecCameras.clear();

	m_iCamIndex = 0;
}

void CCamera_Manager::Free()
{
	m_vecCameras.clear();
}
