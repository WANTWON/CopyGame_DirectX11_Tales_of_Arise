#include "stdafx.h"
#include "..\Public\Portal.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Level.h"

CPortal::CPortal(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CBaseObj(pDevice, pContext)
{
}

CPortal::CPortal(const CPortal & rhs)
	: CBaseObj(rhs)
{
}

HRESULT CPortal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPortal::Initialize(void * pArg)
{
	if (pArg != nullptr)
		memcpy(&m_PortalDesc, pArg, sizeof(PORTALDESC));

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (pArg != nullptr)
	{
		_vector vPosition = XMLoadFloat3(&m_PortalDesc.m_ModelDesc.vPosition);
		vPosition = XMVectorSetW(vPosition, 1.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		Set_Scale(m_PortalDesc.m_ModelDesc.vScale);

	}

	CCollision_Manager::Get_Instance()->Add_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);

	return S_OK;
}

int CPortal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	return OBJ_NOEVENT;
}

void CPortal::Late_Tick(_float fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);

	if (Check_IsinFrustum(2.f) == false)
		return;

	m_pSPHERECom->Update(m_pTransformCom->Get_WorldMatrix());

	CPlayer* pPlayer = CPlayerManager::Get_Instance()->Get_ActivePlayer();
	
	if (m_pSPHERECom->Collision(pPlayer->Get_Collider()))
	{
		if (CGameInstance::Get_Instance()->Key_Up(DIK_SPACE))
		{
			CLevel* pCurrentLevel = CGameInstance::Get_Instance()->Get_CurrentLevel();
			pCurrentLevel->Set_NextLevel(true, m_PortalDesc.iNextLevel);
		}
	}

}

HRESULT CPortal::Ready_Components(void * pArg)
{
	LEVEL iLevel = (LEVEL)CGameInstance::Get_Instance()->Get_DestinationLevelIndex();

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Components(TEXT("Com_Renderer"), LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 1.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	CCollider::COLLIDERDESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(CCollider::COLLIDERDESC));
	ColliderDesc.vScale = _float3(7.f, 7.f, 7.f);
	ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
	ColliderDesc.vPosition = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Components(TEXT("Com_SPHERE"), LEVEL_STATIC, TEXT("Prototype_Component_Collider_SPHERE"), (CComponent**)&m_pSPHERECom, &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortal::SetUp_ShaderResources()
{
	return S_OK;
}

CPortal * CPortal::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		ERR_MSG(TEXT("Failed to Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CPortal::Clone(void * pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CPortal"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPortal::Free()
{
	__super::Free();

	CCollision_Manager::Get_Instance()->Out_CollisionGroup(CCollision_Manager::COLLISION_TRIGGER, this);

}
