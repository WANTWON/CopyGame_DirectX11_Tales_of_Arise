#include "stdafx.h"
#include "..\Public\Camera_Action.h"
#include "GameInstance.h"
#include "CameraManager.h"


CCamera_Action::CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera(pDevice, pContext)
{
}

CCamera_Action::CCamera_Action(const CCamera_Action & rhs)
	: CCamera(rhs)
{
}

void CCamera_Action::Add_CamData(TOOLDESC CamDesc)
{
	
	m_CamDatas.push_back(CamDesc);
}

void CCamera_Action::Remove_Camdata(_int iIndex)
{
	m_CamDatas.erase(m_CamDatas.begin() + iIndex);
}


void CCamera_Action::Set_Play(_bool type)
{
	 m_bPlay = type; 
	 if (m_bPlay)
	 {
		 m_vInitPosition = m_pTransform->Get_State(CTransform::STATE_TRANSLATION);
		 m_vInitAt = m_vInitPosition + m_pTransform->Get_State(CTransform::STATE_LOOK);
	 }
	
}

int CCamera_Action::PlayCamera(_float fTimeDelta)
{

	if (m_CamDatas[m_iIndex].bNewSection == true)
	{
		m_vInitPosition = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
		m_vInitAt = XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition);
		m_pTransform->Set_State(CTransform::STATE_TRANSLATION, m_vInitPosition);
		m_pTransform->LookAt(m_vInitAt);
		m_iIndex++;
		m_fTime = m_CamDatas[m_iIndex].fStartTime*m_fPlayTime;

	}


	m_fTime += fTimeDelta;

	_float fStartTime = m_CamDatas[m_iIndex].fStartTime* m_fPlayTime;
	_float fEndTime = m_CamDatas[m_iIndex].fEndTime* m_fPlayTime;
	_float fValue = (m_fTime - fStartTime) / (fEndTime - fStartTime);


	_vector vPositionStart = m_vInitPosition;
	_vector vPositionEnd = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
	_vector vCatRom1 = vPositionStart;
	_vector vCatRom2 = vPositionEnd;


	_vector vAtStart = m_vInitAt;
	_vector vAtEnd = XMVectorSetW(XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition), 1.f);
	_vector vPosition;

	if (m_CamDatas[m_iIndex].bLerp == true)
		vPosition = XMVectorLerp(vPositionStart, vPositionEnd, fValue);
	else
		vPosition = XMVectorCatmullRom(vCatRom1, vPositionStart, vPositionEnd, vCatRom2, fValue);



	_vector vAt = XMVectorLerp(vAtStart, vAtEnd, fValue);
	m_pTransform->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	m_pTransform->LookAt(vAt);


	if (m_fTime >= fEndTime)
	{
		m_vInitPosition = XMLoadFloat4(&m_CamDatas[m_iIndex].vEyePosition);
		m_vInitAt = XMLoadFloat4(&m_CamDatas[m_iIndex].vAtPosition);

		m_iIndex++;

		if (m_iIndex >= m_CamDatas.size())
		{
			m_CamDatas.clear();
			m_iIndex = 0;
			m_bPlay = false;
			CCameraManager::Get_Instance()->Set_CamState(CCameraManager::CAM_DYNAMIC);
			CCameraManager::Get_Instance()->Clear_ActionList();
		}
	}

	return OBJ_NOEVENT;
}

HRESULT CCamera_Action::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Action::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(&((ACTIONCAMDESC*)pArg)->CameraDesc)))
		return E_FAIL;
	
	
	return S_OK;
}

int CCamera_Action::Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_ACTION)
		return OBJ_NOEVENT;

	__super::Tick(fTimeDelta);

	/* Debug Camera*/
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (m_bPlay)
	{
		PlayCamera(fTimeDelta);
	}


	RELEASE_INSTANCE(CGameInstance);

	if (FAILED(Bind_OnPipeLine()))
		return OBJ_NOEVENT;

	return OBJ_NOEVENT;
}

void CCamera_Action::Late_Tick(_float fTimeDelta)
{
	if (CCameraManager::Get_Instance()->Get_CamState() != CCameraManager::CAM_ACTION)
		return;
	__super::Late_Tick(fTimeDelta);

}

HRESULT CCamera_Action::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	
	return S_OK;
}

CCamera_Action * CCamera_Action::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera_Action*	pInstance = new CCamera_Action(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CCamera_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Action::Clone(void* pArg)
{
	CCamera_Action*	pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CCamera_Action"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Action::Free()
{
	__super::Free();

	
}
